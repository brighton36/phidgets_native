#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#include <ruby.h>
#include <phidget21.h>

// We needed a define here since some compilers won't let you set an array size
// via a const.
#define COMPASS_CORRECTION_LENGTH 13

static double const MICROSECONDS_IN_SECOND = 1000000.0;
static int const DEGREES_IN_CIRCLE = 360;
static int const DEFAULT_SPATIAL_DATA_RATE = 16;

static float const SPATIAL_AHRS_BETA = 0.1f;	// 2 * proportional gain (Kp)

// Make sure one of these is non-zero and the other is 0
static int const DEFAULT_INTERFACEKIT_DATA_RATE = 8;
static int const DEFAULT_INTERFACEKIT_CHANGE_TRIGGER = 0;
// /Make-sure

// 50 milliseconds:
static int const INTERFACEKIT_RATIOMETRIC_RESET_USECS = 50000;

typedef struct sample_rate {
  int in_hz;             // This is really the finished product
  int samples_in_second; // A counter which resets when the second changes

  // This is used for calculating the deltas 
  unsigned long last_second;
} SampleRate;

typedef struct phidget_info {
  CPhidgetHandle handle;
  int  serial;
  const char *type;
  const char *name;
  const char *label;
  int version;
  CPhidget_DeviceClass device_class;
  CPhidget_DeviceID device_id;

  // Attachment Tracking:
  bool is_attached;

  // This is expressed in the number of seconds and microseconds since the epoch:
  struct timeval attached_at;
  struct timezone attached_at_tz;

  // Used by the device drivers to track state:
  void *type_info;

  // Event Handlers
  int (*on_type_attach)(CPhidgetHandle phid, void *userptr);
  int (*on_type_detach)(CPhidgetHandle phid, void *userptr);
  void (*on_type_free)(void *type_info);

} PhidgetInfo;

typedef struct spatial_info {
  // Compass Correction Params:
  bool is_compass_correction_known;
  double compass_correction[COMPASS_CORRECTION_LENGTH];

  SampleRate *sample_rate;

  // Poll interval
  int data_rate;

  // Device limits:
  int accelerometer_axes;
  int compass_axes;
  int gyro_axes;
  int data_rate_max;
  int data_rate_min;
  double *acceleration_min;
  double *acceleration_max;
  double *compass_min;
  double *compass_max;
  double *gyroscope_min;
  double *gyroscope_max;

  // Runtime Values
  bool is_acceleration_known;
  double *acceleration;

  bool is_compass_known;
  double *compass;

  bool is_gyroscope_known;
  double *gyroscope;
  double gyroscope_dcm[3][3];

  // This is used by the gyro:
  double last_microsecond;

  // quaternion elements representing the estimated orientation
  float orientation_q[4];

  // This tells us whether it's our first pass
  bool is_ahrs_initialized;
} SpatialInfo;

typedef struct gps_info {
  SampleRate *sample_rate;

  bool is_fixed;

  bool is_latitude_known;
  double latitude;

  bool is_longitude_known;
  double longitude;

  bool is_altitude_known;
  double altitude;

  bool is_heading_known;
  double heading;

  bool is_velocity_known;
  double velocity;
 
  bool is_now_at_utc_known;
	struct tm now_at_utc;
  short now_at_utc_ms;

} GpsInfo;

typedef struct interfacekit_info {
  // For the inputs:
  SampleRate *analog_sample_rates;
  SampleRate *digital_sample_rates;

  bool is_digital_input_count_known;
  int digital_input_count;

  bool is_digital_output_count_known;
  int digital_output_count;

  bool is_analog_input_count_known;
  int analog_input_count;

  // This is kind of a weird thing to track, probably it will never be needed.
  // I use it to determine whether the input counts have changed between device
  // plug events.
  int analog_input_count_prior;
  int digital_input_count_prior;
  int digital_output_count_prior;

  bool is_dual_ratiometric_mode;
  bool *is_ratiometric;
  int ratiometric_changed_usec;

  // This tracks whether it's time to change the ratiometric state, for the case
  // of a dual-mode ratiometric change:
  bool *updated_since_last_ratiometric_cycle;


  // This flag works for all of the below properties
  bool is_data_rates_known;
  int *data_rates_max;
  int *data_rates_min;
  int *sensor_change_triggers;
  int *data_rates;
  // /is_data_rates_known

  int *digital_input_states;
  int *digital_output_states;
  int *analog_input_states;

} InterfaceKitInfo;

typedef struct advancedservo_info {
  SampleRate *sample_rates;

  bool is_motor_count_known;
  int motor_count;
  int motor_count_prior;

  double *acceleration_max;
  double *acceleration_min;
  double *velocity_max;
  double *velocity_min;

  double *current;
  double *position;
  double *velocity;

} AdvancedServoInfo;

void Init_phidgets_native();
void Init_phidgets_native_module();
void Init_phidgets_native_device(VALUE m_Phidget);
void Init_phidgets_native_accelerometer(VALUE m_Phidget);
void Init_phidgets_native_advancedservo(VALUE m_Phidget);
void Init_phidgets_native_analog(VALUE m_Phidget);
void Init_phidgets_native_bridge(VALUE m_Phidget);
void Init_phidgets_native_encoder(VALUE m_Phidget);
void Init_phidgets_native_frequencycounter(VALUE m_Phidget);
void Init_phidgets_native_gps(VALUE m_Phidget);
void Init_phidgets_native_interfacekit(VALUE m_Phidget);
void Init_phidgets_native_ir(VALUE m_Phidget);
void Init_phidgets_native_led(VALUE m_Phidget);
void Init_phidgets_native_motorcontrol(VALUE m_Phidget);
void Init_phidgets_native_phsensor(VALUE m_Phidget);
void Init_phidgets_native_rfid(VALUE m_Phidget);
void Init_phidgets_native_servo(VALUE m_Phidget);
void Init_phidgets_native_spatial(VALUE m_Phidget);
void Init_phidgets_native_stepper(VALUE m_Phidget);
void Init_phidgets_native_temperaturesensor(VALUE m_Phidget);
void Init_phidgets_native_textlcd(VALUE m_Phidget);
void Init_phidgets_native_textled(VALUE m_Phidget);
void Init_phidgets_native_weightsensor(VALUE m_Phidget);

// Common:
VALUE double_array_to_rb(double *dbl_array, int length);
VALUE int_array_to_rb(int *int_array, int length);
VALUE int_array_zeronils_to_rb(int *int_array, int length);
VALUE phidgetbool_array_to_rb(int *bool_array, int length);
VALUE double3x3_to_matrix_rb(double m3x3[][3]);
int ensure(int result);
int report(int result);
SampleRate *sample_create();
int sample_free(SampleRate *sample_rate);
int sample_zero(SampleRate *sample_rate);
int sample_tick(SampleRate *sample_rate, CPhidget_Timestamp *ts);
float inv_sqrt(float x);
void quat_mult(float a[4], float b[4], float ret[4]);
void quat_norm(float a[4]);
void quat_to_dcm(float q[4], double dcm[][3]);
void quat_to_euler(float q[4], float e[3]);
void quat_from_axis_and_angle(double axis[3], double angle, float fRetQ[4]);

// Phidget Module
VALUE phidget_enable_logging(int argc, VALUE *argv, VALUE class);
VALUE phidget_disable_logging(VALUE class);
VALUE phidget_log(VALUE class, VALUE log_level, VALUE message);
VALUE phidget_all(VALUE class);

// PhidgetsNative::Device
PhidgetInfo *device_info(VALUE self);
void *device_type_info(VALUE self);
void device_free(PhidgetInfo *info);
int CCONV device_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV device_on_detach(CPhidgetHandle phid, void *userptr);
int CCONV device_on_error(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *unknown);

VALUE device_allocate(VALUE class);
VALUE device_initialize(VALUE self, VALUE serial);
VALUE device_close(VALUE self);
VALUE device_wait_for_attachment(VALUE self, VALUE timeout);
VALUE device_is_attached(VALUE self);
VALUE device_device_class(VALUE self);
VALUE device_device_id(VALUE self);
VALUE device_type(VALUE self);
VALUE device_name(VALUE self);
VALUE device_label(VALUE self);
VALUE device_serial_number(VALUE self);
VALUE device_version(VALUE self);

// PhidgetsNative::Spatial
void spatial_on_free(void *type_info);
int CCONV spatial_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV spatial_on_detach(CPhidgetHandle phid, void *userptr);
int CCONV spatial_on_data(CPhidgetSpatialHandle spatial, void *userptr, CPhidgetSpatial_SpatialEventDataHandle *data, int count);
int spatial_set_compass_correction_by_array(CPhidgetSpatialHandle phid, double *correction);
void spatial_ahrs_update(SpatialInfo *spatial_info, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void spatial_ahrs_update_imu(SpatialInfo *spatial_info, float gx, float gy, float gz, float ax, float ay, float az);
void spatial_ahrs_init(SpatialInfo *spatial_info, double ax, double ay, double az, double mx, double my, double mz);
void spatial_orientation_from_accel_and_compass(double dRetEuler[3], double ax, double ay, double az, double mx, double my, double mz);
void spatial_madgeq_to_openglq(float *fMadgQ, float *fRetQ);

VALUE spatial_initialize(VALUE self, VALUE serial);
VALUE spatial_close(VALUE self);
VALUE spatial_sample_rate(VALUE self);
VALUE spatial_accelerometer_axes(VALUE self);
VALUE spatial_compass_axes(VALUE self);
VALUE spatial_gyro_axes(VALUE self);

VALUE spatial_accelerometer_min(VALUE self);
VALUE spatial_accelerometer_max(VALUE self);
VALUE spatial_compass_min(VALUE self);
VALUE spatial_compass_max(VALUE self);
VALUE spatial_gyro_min(VALUE self);
VALUE spatial_gyro_max(VALUE self);

VALUE spatial_accelerometer(VALUE self);
VALUE spatial_compass(VALUE self);
VALUE spatial_gyro(VALUE self);

VALUE spatial_zero_gyro(VALUE self);
VALUE spatial_compass_correction_set(VALUE self, VALUE compass_correction);
VALUE spatial_compass_correction_get(VALUE self);
VALUE spatial_reset_compass_correction(VALUE self);

VALUE spatial_data_rate_min(VALUE self);
VALUE spatial_data_rate_max(VALUE self);
VALUE spatial_data_rate_set(VALUE self, VALUE data_rate);
VALUE spatial_data_rate_get(VALUE self);

VALUE spatial_direction_cosine_matrix(int argc, VALUE *argv, VALUE self);
VALUE spatial_accelerometer_to_roll_and_pitch(VALUE self);
VALUE spatial_accelerometer_to_euler(VALUE self);
VALUE spatial_accelerometer_to_dcm(VALUE self);
VALUE spatial_gravity_to_roll_and_pitch(VALUE self);
VALUE spatial_compass_bearing(VALUE self);
VALUE spatial_compass_bearing_to_euler(VALUE self);
VALUE spatial_compass_bearing_to_dcm(VALUE self);
VALUE spatial_gyro_to_dcm(VALUE self);
VALUE spatial_orientation_to_quaternion(VALUE self);
VALUE spatial_orientation_to_dcm(VALUE self);
VALUE spatial_orientation_to_euler(VALUE self);

int euler_to_3x3dcm(double *mRet, double around_x, double around_y, double around_z, const char *in_order);

// PhidgetsNative::InterfaceKit
void interfacekit_on_free(void *type_info);
int CCONV interfacekit_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV interfacekit_on_detach(CPhidgetHandle phid, void *userptr);
int interfacekit_on_digital_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int inputState);
int interfacekit_on_analog_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int sensorValue);
int interfacekit_assert_ratiometric_state(PhidgetInfo *info);
bool interfacekit_ratiometric_state_is_uniform(InterfaceKitInfo *ifkit_info);
void interfacekit_raise_ratiometric_not_uniform();
int interfacekit_maximize_data_rate(PhidgetInfo *info);
int interfacekit_assert_dual_ratiometric_mode(PhidgetInfo *info);
int interfacekit_assert_sensor_rates(PhidgetInfo *info);
int interfacekit_stamp_ratiometric_change(InterfaceKitInfo *ifkit_info);
int interfacekit_flip_ratiometric_state(PhidgetInfo *info);
bool interfacekit_is_time_to_flip_ratiometric_state(PhidgetInfo *info);
VALUE interfacekit_initialize(VALUE self, VALUE serial);
VALUE interfacekit_close(VALUE self);
VALUE interfacekit_sensor_sample_rates(VALUE self);
VALUE interfacekit_input_sample_rates(VALUE self);
VALUE interfacekit_input_count(VALUE self);
VALUE interfacekit_output_count(VALUE self);
VALUE interfacekit_sensor_count(VALUE self);
VALUE interfacekit_is_ratiometric(VALUE self);
VALUE interfacekit_ratiometric_set(VALUE self, VALUE is_ratiometric);
VALUE interfacekit_ratiometric(int argc, VALUE* argv, VALUE self);
VALUE interfacekit_data_rates_max(VALUE self);
VALUE interfacekit_data_rates_min(VALUE self);
VALUE interfacekit_data_rates(VALUE self);
VALUE interfacekit_change_triggers(VALUE self);
VALUE interfacekit_inputs(VALUE self);
VALUE interfacekit_outputs(VALUE self);
VALUE interfacekit_sensors(VALUE self);
VALUE interfacekit_sensor_raw(VALUE self, VALUE index);
VALUE interfacekit_output_set(VALUE self, VALUE index, VALUE is_on);
VALUE interfacekit_data_rate_set(VALUE self, VALUE index, VALUE rate);
VALUE interfacekit_change_trigger_set(VALUE self, VALUE index, VALUE rate_thresh);

// PhidgetsNative::Gps
void gps_on_free(void *type_info);
int CCONV gps_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV gps_on_detach(CPhidgetHandle phidget, void *userptr);
int CCONV gps_on_position_change(CPhidgetGPSHandle gps, void *userptr, double latitude, double longitude, double altitude);
int CCONV gps_on_fix_change(CPhidgetGPSHandle gps, void *userptr, int status);
VALUE gps_initialize(VALUE self, VALUE serial);
VALUE gps_close(VALUE self);
VALUE gps_sample_rate(VALUE self);
VALUE gps_latitude(VALUE self);
VALUE gps_longitude(VALUE self);
VALUE gps_altitude(VALUE self);
VALUE gps_heading(VALUE self);
VALUE gps_velocity(VALUE self);
VALUE gps_is_fixed(VALUE self);
VALUE gps_now_at_utc(VALUE self);

// PhidgetsNative::AdvancedServo
void advancedservo_on_free(void *type_info);
int CCONV advancedservo_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV advancedservo_on_detach(CPhidgetHandle phid, void *userptr);
int CCONV advancedservo_on_velocity_change(CPhidgetAdvancedServoHandle phid, void *userPtr, int index, double velocity);
int CCONV advancedservo_on_position_change(CPhidgetAdvancedServoHandle phid, void *userPtr, int index, double position);
int CCONV advancedservo_on_current_change(CPhidgetAdvancedServoHandle phid, void *userPtr, int index, double current);
VALUE advancedservo_initialize(VALUE self, VALUE serial);
VALUE advancedservo_close(VALUE self);
VALUE advancedservo_motor_count(VALUE self);
VALUE advancedservo_sample_rates(VALUE self);
VALUE advancedservo_acceleration_max(VALUE self);
VALUE advancedservo_acceleration_min(VALUE self);
VALUE advancedservo_velocity_max(VALUE self);
VALUE advancedservo_velocity_min(VALUE self);
VALUE advancedservo_currents(VALUE self);
VALUE advancedservo_positions(VALUE self);
VALUE advancedservo_velocities(VALUE self);
VALUE advancedservo_acceleration(int argc, VALUE* argv, VALUE self);
VALUE advancedservo_velocity_limit(int argc, VALUE* argv, VALUE self);
VALUE advancedservo_position_max(int argc, VALUE* argv, VALUE self);
VALUE advancedservo_position_min(int argc, VALUE* argv, VALUE self);
VALUE advancedservo_servo_type(int argc, VALUE* argv, VALUE self);
VALUE advancedservo_position_set(VALUE self, VALUE index, VALUE value);
VALUE advancedservo_is_speed_ramping(VALUE self, VALUE index);
VALUE advancedservo_speed_ramping_set(VALUE self, VALUE index, VALUE value);
VALUE advancedservo_is_engaged(VALUE self, VALUE index);
VALUE advancedservo_engaged_set(VALUE self, VALUE index, VALUE value);
VALUE advancedservo_is_stopped(VALUE self, VALUE index);
VALUE advancedservo_servo_parameters_set(VALUE self, VALUE index, VALUE min_us, VALUE max_us, VALUE degrees, VALUE velocity_max);

// Stub initializers:
VALUE accelerometer_initialize(VALUE self, VALUE serial);
VALUE advancedservo_initialize(VALUE self, VALUE serial);
VALUE encoder_initialize(VALUE self, VALUE serial);
VALUE ir_initialize(VALUE self, VALUE serial);
VALUE led_initialize(VALUE self, VALUE serial);
VALUE motorcontrol_initialize(VALUE self, VALUE serial);
VALUE phsensor_initialize(VALUE self, VALUE serial);
VALUE rfid_initialize(VALUE self, VALUE serial);
VALUE servo_initialize(VALUE self, VALUE serial);
VALUE stepper_initialize(VALUE self, VALUE serial);
VALUE temperaturesensor_initialize(VALUE self, VALUE serial);
VALUE textlcd_initialize(VALUE self, VALUE serial);
VALUE textled_initialize(VALUE self, VALUE serial);
VALUE weightsensor_initialize(VALUE self, VALUE serial);
VALUE analog_initialize(VALUE self, VALUE serial);
VALUE bridge_initialize(VALUE self, VALUE serial);
VALUE frequencycounter_initialize(VALUE self, VALUE serial);
