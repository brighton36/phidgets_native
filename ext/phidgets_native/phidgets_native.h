#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <ruby.h>
#include <phidget21.h>

// We needed a define here since some compilers won't let you set an array size
// via a const.
#define COMPASS_CORRECTION_LENGTH 13

static double const MICROSECONDS_IN_SECOND = 1000000.0;
static int const DEGREES_IN_CIRCLE = 360;
static int const DEFAULT_SPATIAL_DATA_RATE = 16;

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

  // Sample tracking.
  int sample_rate;       // NOTE: These are in Hz
  int samples_in_second;    // A counter which resets when the second changes

  // This is used for calculating deltas for both sample tracking, and gyro adjustment
  unsigned long last_second;

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

  // This is used by the gyro:
  double last_microsecond;
} SpatialInfo;

typedef struct gps_info {
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
int ensure(int result);
int report(int result);

// Phidget Module
VALUE phidget_enable_logging(int argc, VALUE *argv, VALUE class);
VALUE phidget_disable_logging(VALUE class);
VALUE phidget_log(VALUE class, VALUE log_level, VALUE message);
VALUE phidget_all(VALUE class);

// Phidget::Device
PhidgetInfo *device_info(VALUE self);
void *device_type_info(VALUE self);
void device_free(PhidgetInfo *info);
void device_sample(PhidgetInfo *info, CPhidget_Timestamp *ts);
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
VALUE device_sample_rate(VALUE self);

// Phidget::Spatial
void spatial_on_free(void *type_info);
int CCONV spatial_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV spatial_on_detach(CPhidgetHandle phid, void *userptr);
int CCONV spatial_on_data(CPhidgetSpatialHandle spatial, void *userptr, CPhidgetSpatial_SpatialEventDataHandle *data, int count);
int spatial_set_compass_correction_by_array(CPhidgetSpatialHandle phid, double *correction);
VALUE spatial_initialize(VALUE self, VALUE serial);
VALUE spatial_close(VALUE self);
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

// Phidget::InterfaceKit
VALUE interfacekit_initialize(VALUE self, VALUE serial);

// Phidget::Gps
VALUE gps_initialize(VALUE self, VALUE serial);
VALUE gps_close(VALUE self);
int CCONV gps_on_attach(CPhidgetHandle phid, void *userptr);
int CCONV gps_on_detach(CPhidgetHandle phidget, void *userptr);
int CCONV gps_on_position_change(CPhidgetGPSHandle gps, void *userptr, double latitude, double longitude, double altitude);
int CCONV gps_on_fix_change(CPhidgetGPSHandle gps, void *userptr, int status);
void gps_on_free(void *type_info);
VALUE gps_latitude(VALUE self);
VALUE gps_longitude(VALUE self);
VALUE gps_altitude(VALUE self);
VALUE gps_heading(VALUE self);
VALUE gps_velocity(VALUE self);
VALUE gps_is_fixed(VALUE self);
VALUE gps_now_at_utc(VALUE self);

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
