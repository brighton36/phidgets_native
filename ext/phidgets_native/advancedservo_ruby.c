#include "phidgets_native.h"

const char MSG_INDEX_MUST_BE_FIXNUM[] = "servo index must be fixnum";
const char MSG_VALUE_MUST_BE_FLOAT[] = "value must be a floating point number";
const char MSG_VALUE_MUST_BE_BOOL[] = "value must be either true or false";
const char MSG_SERVO_MIN_US_MUST_BE_FLOAT[] = "min_us must be a floating point number";
const char MSG_SERVO_MAX_US_MUST_BE_FLOAT[] = "max_us must be a floating point number";
const char MSG_SERVO_DEGREES_MUST_BE_FLOAT[] = "degrees_us must be a floating point number";
const char MSG_SERVO_VELOCITY_MAX_MUST_BE_FLOAT[] = "velocity_max must be a floating point number";
const char MSG_TYPE_MUST_BE_SYMBOL[] = "type must be a symbol";
const char MSG_UNRECOGNIZED_SERVO_TYPE[] = "unrecognized servo type, double check your spelling and/or revisit the rdoc";

/* 
 * Document-class: PhidgetsNative::AdvancedServo < PhidgetsNative::Device
 *
 * This class is a stub, and is currently in need of an actual implementation.
 * Nonetheless, all of the methods from its parent class PhidgetsNative::Device are 
 * available.
 */

void Init_phidgets_native_advancedservo(VALUE m_Phidget) {
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

 	VALUE c_AdvancedServo = rb_define_class_under(m_Phidget, "AdvancedServo", c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_AdvancedServo, "initialize", advancedservo_initialize, 1);

  /*
   * Document-method: motor_count
   * call-seq:
   *   motor_count -> Fixnum
   *
   * Returns an integer which represents the number of motor interfaces provided
   * by this advanced servo controller, or nil if the device has yet to be connected.
   */
  rb_define_method(c_AdvancedServo, "motor_count", advancedservo_motor_count, 0);

  /*
   * Document-method: sample_rates
   * call-seq:
   *   sample_rates -> Array
   *
   * The advancedservo object tracks the sample rate of each servo individually. 
   * This method returns an array containing the sample rates of each servo
   */
  rb_define_method(c_AdvancedServo, "sample_rates", advancedservo_sample_rates, 0);

  /*
   * Document-method: acceleration_max
   * call-seq:
   *   acceleration_max -> Array
   *
   * Returns an array of Floats which specify the maximum acceleration value that can 
   * be configured on each servo, or nil if the device has yet to be connected.
   */
  rb_define_method(c_AdvancedServo, "acceleration_max", advancedservo_acceleration_max, 0);

  /*
   * Document-method: acceleration_min
   * call-seq:
   *   acceleration_min -> Array
   *
   * Returns an array of Floats which specify the minimum acceleration value that can 
   * be configured on each servo, or nil if the device has yet to be connected.
   */
  rb_define_method(c_AdvancedServo, "acceleration_min", advancedservo_acceleration_min, 0);

  /*
   * Document-method: velocity_max
   * call-seq:
   *   velocity_max -> Array
   *
   * Returns an array of Floats which specify the maximum velocity value that can 
   * be configured on each servo, or nil if the device has yet to be connected.
   */
  rb_define_method(c_AdvancedServo, "velocity_max", advancedservo_velocity_max, 0);

  /*
   * Document-method: velocity_min
   * call-seq:
   *   velocity_min -> Array
   *
   * Returns an array of Floats which specify the minimum velocity value that can 
   * be configured on each servo, or nil if the device is disconnected.
   */
  rb_define_method(c_AdvancedServo, "velocity_min", advancedservo_velocity_min, 0);

  /*
   * Document-method: currents
   * call-seq:
   *   currents -> Array
   *
   * Returns an array of Floats which represent the current draw of each servo
   * attached to the system, or nil if the device is disconnected.
   */
  rb_define_method(c_AdvancedServo, "currents", advancedservo_currents, 0);

  /*
   * Document-method: positions
   * call-seq:
   *   positions -> Array
   *
   * Returns an array of Floats which represent the positions of each servo
   * attached to the system, or nil if the device has yet to be connected.
   */
  rb_define_method(c_AdvancedServo, "positions", advancedservo_positions, 0);

  /*
   * Document-method: velocities
   * call-seq:
   *   velocities -> Array
   *
   * Returns an array of Floats which represent the velocities of each servo
   * attached to the system, or nil if the device is disconnected.
   */
  rb_define_method(c_AdvancedServo, "velocities", advancedservo_velocities, 0);

  /*
   * Document-method: acceleration
   * call-seq:
   *   acceleration(Fixnum index, Float value = nil) -> Float
   *
   * If a value is passed, the acceleration of a servo is set to the specified
   * value at the provided index.
   * Returns a float representing the "acceleration" of the servo at the provided
   * index. This value comes directly from the 
   * CPhidgetAdvancedServo_getAcceleration[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "acceleration", advancedservo_acceleration, -1);

  /*
   * Document-method: velocity_limit
   * call-seq:
   *   velocity_limit(Fixnum index, Float value = nil) -> Float
   *
   * If a value is passed, the velocity_limit of a servo is set to the specified
   * value at the provided index.
   * Returns a float representing the "velocity limit" of the servo at the provided
   * index. This value comes directly from the 
   * CPhidgetAdvancedServo_getVelocityLimit[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "velocity_limit", advancedservo_velocity_limit, -1);

  /*
   * Document-method: position
   * call-seq:
   *   position(Fixnum index, Float value = nil) -> Float
   *
   * Sets the servo position of the servo at the provided index.
   * Returns the passed position. This value comes directly from the 
   * CPhidgetAdvancedServo_setPosition[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "position", advancedservo_position_set, 2);

  /*
   * Document-method: position_max
   * call-seq:
   *   position_max(Fixnum index, Float value = nil) -> Float
   *
   * If a value is passed, the position_max of a servo is set to the specified
   * value at the provided index.
   * Returns a float representing the "maximum position" of the servo at the provided
   * index. This value comes directly from the 
   * CPhidgetAdvancedServo_getPositionMax[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "position_max", advancedservo_position_max, -1);

  /*
   * Document-method: position_min
   * call-seq:
   *   position_min(Fixnum index, Float value = nil) -> Float
   *
   * If a value is passed, the position_min of a servo is set to the specified
   * value at the provided index.
   * Returns a float representing the "minimum position" of the servo at the provided
   * index. This value comes directly from the 
   * CPhidgetAdvancedServo_getPositionMin[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "position_min", advancedservo_position_min, -1);

  /*
   * Document-method: servo_type
   * call-seq:
   *   servo_type(Fixnum index, Float value = nil) -> Symbol
   *
   * If a value is passed, the type of servo is set to the specified
   * value at the provided index.
   * Returns a symbol indicating the model/type of the servo at the provided
   * index. This value comes directly from the 
   * CPhidgetAdvancedServo_getServoType[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   *
   * Currently, the following servo types are returned:
   * * :default
   * * :raw_us_mode
   * * :hitec_hs322hd
   * * :hitec_hs5245mg
   * * :hitec_805bb
   * * :hitec_hs422
   * * :towerpro_mg90
   * * :hitec_hsr1425cr
   * * :hitec_hs785hb
   * * :hitec_hs485hb
   * * :hitec_hs645mg
   * * :hitec_815bb
   * * :firgelli_l12_30_50_06_r
   * * :firgelli_l12_50_100_06_r
   * * :firgelli_l12_50_210_06_r
   * * :firgelli_l12_100_50_06_r
   * * :firgelli_l12_100_100_06_r
   * * :springrc_sm_s2313m
   * * :springrc_sm_s3317m
   * * :springrc_sm_s3317sr
   * * :springrc_sm_s4303r
   * * :springrc_sm_s4315m
   * * :springrc_sm_s4315r
   * * :springrc_sm_s4505b
   * * :unknown
   *
   * NOTE: Servo types cannot be set to :unknown, though :unknown is possibly
   * returned if an unrecognized servo type is returned by the phidget libraries.
   */
  rb_define_method(c_AdvancedServo, "servo_type", advancedservo_servo_type, -1);

  /*
   * Document-method: speed_ramping?
   * call-seq:
   *   speed_ramping?(Fixnum index) -> Boolean
   *
   * Returns either true or false, depending on whether the servo at the provided
   * index is configured to speed ramp. This value comes directly from the 
   * CPhidgetAdvancedServo_getSpeedRampingOn[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "speed_ramping?", advancedservo_is_speed_ramping, 1);

  /*
   * Document-method: engaged?
   * call-seq:
   *   engaged?(Fixnum index) -> Boolean
   *
   * Returns either true or false, depending on whether the servo at the provided
   * index is engaged (aka - whether the motor is powered or not). This value 
   * comes directly from the 
   * CPhidgetAdvancedServo_getEngaged[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "engaged?", advancedservo_is_engaged, 1);

  /*
   * Document-method: stopped?
   * call-seq:
   *   stopped?(Fixnum index) -> Boolean
   *
   * Returns either true or false, depending on whether the servo at the provided
   * index is stopped (aka - whether the motor is currently moving or not). This 
   * value comes directly from the 
   * CPhidgetAdvancedServo_getStopped[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "stopped?", advancedservo_is_stopped, 1);


  /*
   * Document-method: speed_ramping
   * call-seq:
   *   speed_ramping(Fixnum index, Boolean) -> Boolean
   *
   * Sets the "speed ramping" of the servo at the provided index.
   * This value is sent directly to the
   * CPhidgetAdvancedServo_setSpeedRampingOn[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "speed_ramping", advancedservo_speed_ramping_set, 2);

  /*
   * Document-method: engaged
   * call-seq:
   *   engaged(Fixnum index, Boolean) -> Boolean
   *
   * Sets the "engagement" of the servo at the provided index.
   * This value is sent directly to the
   * CPhidgetAdvancedServo_setEngaged[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "engaged", advancedservo_engaged_set, 2);

  /*
   * Document-method: servo_parameters
   * call-seq:
   *   servo_parameters(Fixnum index, Float min_us, Float max_us, Float degrees, Float velocity_max) -> Qnil
   *
   * Sets the "servo parameters" of the servo at the provided index.
   * This value is sent directly to the
   * CPhidgetAdvancedServo_setServoParameters[http://www.phidgets.com/documentation/web/cdoc/group__phidadvservo.html] 
   * function.
   */
  rb_define_method(c_AdvancedServo, "servo_parameters", advancedservo_servo_parameters_set, 5);
}

VALUE advancedservo_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  
  AdvancedServoInfo *servo_info = ALLOC(AdvancedServoInfo); 
  memset(servo_info, 0, sizeof(AdvancedServoInfo));
  servo_info->is_motor_count_known = false;

  CPhidgetAdvancedServoHandle advancedservo = 0;
  ensure(CPhidgetAdvancedServo_create(&advancedservo));

  info->handle = (CPhidgetHandle)advancedservo;
  info->on_type_attach = advancedservo_on_attach;
  info->on_type_detach = advancedservo_on_detach;
  info->on_type_free = advancedservo_on_free;
  info->type_info = servo_info;

  ensure(CPhidgetAdvancedServo_set_OnVelocityChange_Handler(advancedservo, advancedservo_on_velocity_change, info));
  ensure(CPhidgetAdvancedServo_set_OnPositionChange_Handler(advancedservo, advancedservo_on_position_change, info));
  ensure(CPhidgetAdvancedServo_set_OnCurrentChange_Handler(advancedservo, advancedservo_on_current_change, info));

  return rb_call_super(1, &serial);
}

VALUE advancedservo_close(VALUE self) {
  PhidgetInfo *info = device_info(self);

  ensure(CPhidgetAdvancedServo_set_OnVelocityChange_Handler((CPhidgetAdvancedServoHandle)info->handle, NULL, NULL));
  ensure(CPhidgetAdvancedServo_set_OnPositionChange_Handler((CPhidgetAdvancedServoHandle)info->handle, NULL, NULL));
  ensure(CPhidgetAdvancedServo_set_OnCurrentChange_Handler((CPhidgetAdvancedServoHandle)info->handle, NULL, NULL));

  return rb_call_super(0,NULL);
}

VALUE advancedservo_motor_count(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->is_motor_count_known) ? 
    INT2FIX(servo_info->motor_count) : Qnil;
}

VALUE advancedservo_sample_rates(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  if (!servo_info->is_motor_count_known) return Qnil;

  int *rates_in_hz = ALLOC_N(int, servo_info->motor_count);
  for(int i=0; i<servo_info->motor_count; i++)
    rates_in_hz[i] = servo_info->sample_rates[i].in_hz;

  VALUE ret = int_array_to_rb(rates_in_hz, servo_info->motor_count);

  xfree(rates_in_hz);

  return ret;
}

VALUE advancedservo_acceleration_max(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->is_motor_count_known) ? 
    double_array_to_rb(servo_info->acceleration_max, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_acceleration_min(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->is_motor_count_known) ? 
    double_array_to_rb(servo_info->acceleration_min, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_velocity_max(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->is_motor_count_known) ? 
    double_array_to_rb(servo_info->velocity_max, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_velocity_min(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->is_motor_count_known) ? 
    double_array_to_rb(servo_info->velocity_min, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_currents(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->current) ? 
    double_array_to_rb(servo_info->current, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_positions(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->position) ? 
    double_array_to_rb(servo_info->position, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_velocities(VALUE self) {
  AdvancedServoInfo *servo_info = device_type_info(self);

  return (servo_info->velocity) ? 
    double_array_to_rb(servo_info->velocity, servo_info->motor_count) : Qnil;
}

VALUE advancedservo_acceleration(int argc, VALUE* argv, VALUE self) {
  VALUE index;
  VALUE value;

  rb_scan_args(argc, argv, "11", &index, &value);

  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  double value_dbl;

  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;

  if(NIL_P(value)) {
    // It's a get request:
    ensure(CPhidgetAdvancedServo_getAcceleration(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, &value_dbl ));

  } else {
    // Set request
    if ( (TYPE(value) != T_FLOAT) && (TYPE(value) != T_FIXNUM) )
      rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_FLOAT);

    value_dbl = NUM2DBL(value);

    ensure(CPhidgetAdvancedServo_setAcceleration(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, value_dbl ));
  }

  return DBL2NUM(value_dbl);
}

VALUE advancedservo_velocity_limit(int argc, VALUE* argv, VALUE self) {
  VALUE index;
  VALUE value;

  rb_scan_args(argc, argv, "11", &index, &value);

  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  double value_dbl;

  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;

  if(NIL_P(value)) {
    // It's a get request:
    ensure(CPhidgetAdvancedServo_getVelocityLimit(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, &value_dbl ));

  } else {
    // Set request
    if ( (TYPE(value) != T_FLOAT) && (TYPE(value) != T_FIXNUM) )
      rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_FLOAT);

    value_dbl = NUM2DBL(value);

    ensure(CPhidgetAdvancedServo_setVelocityLimit(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, value_dbl ));
  }

  return DBL2NUM(value_dbl);
}

VALUE advancedservo_position_max(int argc, VALUE* argv, VALUE self) {
  VALUE index;
  VALUE value;

  rb_scan_args(argc, argv, "11", &index, &value);

  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  double value_dbl;

  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;

  if(NIL_P(value)) {
    // It's a get request:
    ensure(CPhidgetAdvancedServo_getPositionMax(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, &value_dbl ));

  } else {
    // Set request
    if ( (TYPE(value) != T_FLOAT) && (TYPE(value) != T_FIXNUM) )
      rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_FLOAT);

    value_dbl = NUM2DBL(value);
    ensure(CPhidgetAdvancedServo_setPositionMax(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, value_dbl ));
  }

  return DBL2NUM(value_dbl);
}

VALUE advancedservo_position_min(int argc, VALUE* argv, VALUE self) {
  VALUE index;
  VALUE value;

  rb_scan_args(argc, argv, "11", &index, &value);

  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  double value_dbl;

  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;

  if(NIL_P(value)) {
    // It's a get request:
    ensure(CPhidgetAdvancedServo_getPositionMin(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, &value_dbl ));

  } else {
    // Set request
    if ( (TYPE(value) != T_FLOAT) && (TYPE(value) != T_FIXNUM) )
      rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_FLOAT);

    value_dbl = NUM2DBL(value);

    ensure(CPhidgetAdvancedServo_setPositionMin(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, value_dbl ));
  }

  return DBL2NUM(value_dbl);
}

VALUE advancedservo_is_speed_ramping(VALUE self, VALUE index) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);

  int is_ret;
  ensure(CPhidgetAdvancedServo_getSpeedRampingOn(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, &is_ret ));

  return (is_ret == PTRUE) ? Qtrue : Qfalse;
}

VALUE advancedservo_is_engaged(VALUE self, VALUE index) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);

  int is_ret;
  ensure(CPhidgetAdvancedServo_getEngaged(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, &is_ret ));

  return (is_ret == PTRUE) ? Qtrue : Qfalse;
}

VALUE advancedservo_is_stopped(VALUE self, VALUE index) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);

  int is_ret;
  ensure(CPhidgetAdvancedServo_getStopped(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, &is_ret ));

  return (is_ret == PTRUE) ? Qtrue : Qfalse;
}


VALUE advancedservo_position_set(VALUE self, VALUE index, VALUE value) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);
  if ( (TYPE(value) != T_FLOAT) && (TYPE(value) != T_FIXNUM) )
    rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_FLOAT);

  int index_int = FIX2INT(index);
  double value_dbl = NUM2DBL(value);

  double ret;
  ensure(CPhidgetAdvancedServo_setPosition(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, value_dbl ));

  return DBL2NUM(value_dbl);
}

VALUE advancedservo_speed_ramping_set(VALUE self, VALUE index, VALUE value) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);


  int index_int = FIX2INT(index);
  int value_int;

  if (TYPE(value) == T_TRUE) 
    value_int = PTRUE;
  else if (TYPE(value) == T_FALSE)
    value_int = PFALSE;
  else
    rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_BOOL);

  ensure(CPhidgetAdvancedServo_setSpeedRampingOn(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, value_int ));

  return (value_int == PTRUE) ? Qtrue : Qfalse;
}

VALUE advancedservo_engaged_set(VALUE self, VALUE index, VALUE value) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  int value_int;

  if (TYPE(value) == T_TRUE) 
    value_int = PTRUE;
  else if (TYPE(value) == T_FALSE)
    value_int = PFALSE;
  else
    rb_raise(rb_eTypeError, MSG_VALUE_MUST_BE_BOOL);

  ensure(CPhidgetAdvancedServo_setEngaged(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, value_int ));

  return (value_int == PTRUE) ? Qtrue : Qfalse;
}

VALUE advancedservo_servo_parameters_set(VALUE self, VALUE index, VALUE min_us, VALUE max_us, VALUE degrees, VALUE velocity_max) {
  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;
  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  if ( (TYPE(min_us) != T_FLOAT) && (TYPE(min_us) != T_FIXNUM) )
    rb_raise(rb_eTypeError, MSG_SERVO_MIN_US_MUST_BE_FLOAT);

  if ( (TYPE(max_us) != T_FLOAT) && (TYPE(max_us) != T_FIXNUM) )
    rb_raise(rb_eTypeError, MSG_SERVO_MAX_US_MUST_BE_FLOAT);

  if ( (TYPE(degrees) != T_FLOAT) && (TYPE(degrees) != T_FIXNUM) )
    rb_raise(rb_eTypeError, MSG_SERVO_DEGREES_MUST_BE_FLOAT);

  if ( (TYPE(velocity_max) != T_FLOAT) && (TYPE(velocity_max) != T_FIXNUM) )
    rb_raise(rb_eTypeError, MSG_SERVO_VELOCITY_MAX_MUST_BE_FLOAT);

  int index_int = FIX2INT(index);
  double min_us_dbl = NUM2DBL(min_us);
  double max_us_dbl = NUM2DBL(max_us);
  double degrees_dbl = NUM2DBL(degrees);
  double velocity_max_dbl = NUM2DBL(velocity_max);

  ensure(CPhidgetAdvancedServo_setServoParameters(
    (CPhidgetAdvancedServoHandle)info->handle, index_int, min_us_dbl, max_us_dbl, 
    degrees_dbl, velocity_max_dbl ));

  return Qnil;
}

VALUE advancedservo_servo_type(int argc, VALUE* argv, VALUE self) {
  VALUE index;
  VALUE value;

  rb_scan_args(argc, argv, "11", &index, &value);

  if (TYPE(index) != T_FIXNUM) rb_raise(rb_eTypeError, MSG_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);

  PhidgetInfo *info = device_info(self);

  if (!info->is_attached) return Qnil;

  VALUE ret;
  if(NIL_P(value)) {
    // It's a get request:
    CPhidget_ServoType servo_type;
    ensure(CPhidgetAdvancedServo_getServoType(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, &servo_type ));

    switch(servo_type) {
      case PHIDGET_SERVO_DEFAULT:
        ret = ID2SYM(rb_intern("default"));
        break;
      case PHIDGET_SERVO_RAW_us_MODE:
        ret = ID2SYM(rb_intern("raw_us_mode"));
        break;
      case PHIDGET_SERVO_HITEC_HS322HD:
        ret = ID2SYM(rb_intern("hitec_hs322hd"));
        break;
      case PHIDGET_SERVO_HITEC_HS5245MG:
        ret = ID2SYM(rb_intern("hitec_hs5245mg"));
        break;
      case PHIDGET_SERVO_HITEC_805BB:
        ret = ID2SYM(rb_intern("hitec_805bb"));
        break;
      case PHIDGET_SERVO_HITEC_HS422:
        ret = ID2SYM(rb_intern("hitec_hs422"));
        break;
      case PHIDGET_SERVO_TOWERPRO_MG90:
        ret = ID2SYM(rb_intern("towerpro_mg90"));
        break;
      case PHIDGET_SERVO_HITEC_HSR1425CR:
        ret = ID2SYM(rb_intern("hitec_hsr1425cr"));
        break;
      case PHIDGET_SERVO_HITEC_HS785HB:
        ret = ID2SYM(rb_intern("hitec_hs785hb"));
        break;
      case PHIDGET_SERVO_HITEC_HS485HB:
        ret = ID2SYM(rb_intern("hitec_hs485hb"));
        break;
      case PHIDGET_SERVO_HITEC_HS645MG:
        ret = ID2SYM(rb_intern("hitec_hs645mg"));
        break;
      case PHIDGET_SERVO_HITEC_815BB:
        ret = ID2SYM(rb_intern("hitec_815bb"));
        break;
      case PHIDGET_SERVO_FIRGELLI_L12_30_50_06_R:
        ret = ID2SYM(rb_intern("firgelli_l12_30_50_06_r"));
        break;
      case PHIDGET_SERVO_FIRGELLI_L12_50_100_06_R:
        ret = ID2SYM(rb_intern("firgelli_l12_50_100_06_r"));
        break;
      case PHIDGET_SERVO_FIRGELLI_L12_50_210_06_R:
        ret = ID2SYM(rb_intern("firgelli_l12_50_210_06_r"));
        break;
      case PHIDGET_SERVO_FIRGELLI_L12_100_50_06_R:
        ret = ID2SYM(rb_intern("firgelli_l12_100_50_06_r"));
        break;
      case PHIDGET_SERVO_FIRGELLI_L12_100_100_06_R:
        ret = ID2SYM(rb_intern("firgelli_l12_100_100_06_r"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S2313M:
        ret = ID2SYM(rb_intern("springrc_sm_s2313m"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S3317M:
        ret = ID2SYM(rb_intern("springrc_sm_s3317m"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S3317SR:
        ret = ID2SYM(rb_intern("springrc_sm_s3317sr"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S4303R:
        ret = ID2SYM(rb_intern("springrc_sm_s4303r"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S4315M:
        ret = ID2SYM(rb_intern("springrc_sm_s4315m"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S4315R:
        ret = ID2SYM(rb_intern("springrc_sm_s4315r"));
        break;
      case PHIDGET_SERVO_SPRINGRC_SM_S4505B:
        ret = ID2SYM(rb_intern("springrc_sm_s4505b"));
        break;
      default:
        ret = ID2SYM(rb_intern("unknown"));
        break;
    }

  } else {
    // Set request
    if (TYPE(value) != T_SYMBOL) rb_raise(rb_eTypeError, MSG_TYPE_MUST_BE_SYMBOL);

    ID type_id = SYM2ID(value);

    CPhidget_ServoType phidget_type;

    if (rb_intern("default") == type_id)
      phidget_type = PHIDGET_SERVO_DEFAULT;
    else if (rb_intern("raw_us_mode") == type_id)
      phidget_type = PHIDGET_SERVO_RAW_us_MODE;
    else if (rb_intern("hitec_hs322hd") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HS322HD;
    else if (rb_intern("hitec_hs5245mg") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HS5245MG;
    else if (rb_intern("hitec_805bb") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_805BB;
    else if (rb_intern("hitec_hs422") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HS422;
    else if (rb_intern("towerpro_mg90") == type_id)
      phidget_type = PHIDGET_SERVO_TOWERPRO_MG90;
    else if (rb_intern("hitec_hsr1425cr") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HSR1425CR;
    else if (rb_intern("hitec_hs785hb") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HS785HB;
    else if (rb_intern("hitec_hs485hb") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HS485HB;
    else if (rb_intern("hitec_hs645mg") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_HS645MG;
    else if (rb_intern("hitec_815bb") == type_id)
      phidget_type = PHIDGET_SERVO_HITEC_815BB;
    else if (rb_intern("firgelli_l12_30_50_06_r") == type_id)
      phidget_type = PHIDGET_SERVO_FIRGELLI_L12_30_50_06_R;
    else if (rb_intern("firgelli_l12_50_100_06_r") == type_id)
      phidget_type = PHIDGET_SERVO_FIRGELLI_L12_50_100_06_R;
    else if (rb_intern("firgelli_l12_50_210_06_r") == type_id)
      phidget_type = PHIDGET_SERVO_FIRGELLI_L12_50_210_06_R;
    else if (rb_intern("firgelli_l12_100_50_06_r") == type_id)
      phidget_type = PHIDGET_SERVO_FIRGELLI_L12_100_50_06_R;
    else if (rb_intern("firgelli_l12_100_100_06_r") == type_id)
      phidget_type = PHIDGET_SERVO_FIRGELLI_L12_100_100_06_R;
    else if (rb_intern("springrc_sm_s2313m") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S2313M;
    else if (rb_intern("springrc_sm_s3317m") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S3317M;
    else if (rb_intern("springrc_sm_s3317sr") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S3317SR;
    else if (rb_intern("springrc_sm_s4303r") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S4303R;
    else if (rb_intern("springrc_sm_s4315m") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S4315M;
    else if (rb_intern("springrc_sm_s4315r") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S4315R;
    else if (rb_intern("springrc_sm_s4505b") == type_id)
      phidget_type = PHIDGET_SERVO_SPRINGRC_SM_S4505B;
    else
      rb_raise(rb_eTypeError, MSG_UNRECOGNIZED_SERVO_TYPE);

    ensure(CPhidgetAdvancedServo_setServoType(
      (CPhidgetAdvancedServoHandle)info->handle, index_int, phidget_type ));
    
    ret = value;
  }

  return ret;
}
