#include "phidgets_native.h"

const char MSG_RATIOMETRIC_MUST_BE_BOOL[] = "ratiometric can only be either true or false";
const char MSG_SENSOR_INDEX_MUST_BE_FIXNUM[] = "analog sensor offset must be fixnum";
const char MSG_SENSOR_INDEX_TOO_HIGH[] = "no sensor available at the provided offset";
const char MSG_OUTPUT_INDEX_MUST_BE_FIXNUM[] = "output sensor offset must be fixnum";
const char MSG_OUTPUT_INDEX_TOO_HIGH[] = "no output available at the provided offset";
const char MSG_OUTPUT_VALUE_MUST_BE_BOOL[] = "output value must be true or false";

const char MSG_DATA_RATE_VALUE_MUST_BE_FIXNUM[] = "rate must be a fixnum";
const char MSG_DATA_RATE_EXCEEDS_LIMIT[] = "provided rate exceeds allowed limit";
const char MSG_CHANGE_TRIG_VALUE_MUST_BE_FIXNUM[] = "threshold must be a fixnum";
const char MSG_CHANGE_TRIG_EXCEEDS_LIMIT[] = "provided threshold exceeds allowed limit";

/* 
 * Document-class: PhidgetsNative::InterfaceKit < PhidgetsNative::Device
 *
 * This class provides functionality specific to the "InterfaceKit" device class. 
 * Primarily, this phidget reports the values of analog and digital inputs, and
 * provides interfaces for outputing to analog and digital components.
 */

void Init_phidgets_native_interfacekit(VALUE m_Phidget) {
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

  VALUE c_InterfaceKit = rb_define_class_under(m_Phidget,"InterfaceKit",c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_InterfaceKit, "initialize", interfacekit_initialize, 1);

  /*
   * Document-method: close
   * call-seq:
   *   close -> nil
   *
   * This method will unregister the phidget event handlers, and free up all
   * API resources associated with the phidget. This is an optional, but useful
   * way to remove the object's overhead before the GC kicks in and actually 
   * frees the resource.
   */
  rb_define_method(c_InterfaceKit, "close", interfacekit_close, 0);

  /*
   * Document-method: input_sample_rates
   * call-seq:
   *   input_sample_rates -> Array
   *
   * The interface kit object tracks the sample rate of each input individually. 
   * This method returns an array containing the sample rates of each digital sensor.
   */
  rb_define_method(c_InterfaceKit, "input_sample_rates", interfacekit_input_sample_rates, 0);

  /*
   * Document-method: sensor_sample_rates
   * call-seq:
   *   sensor_sample_rates -> Array
   *
   * The interface kit object tracks the sample rate of each sensor individually. 
   * This method returns an array containing the sample rates of each analog sensor.
   */
  rb_define_method(c_InterfaceKit, "sensor_sample_rates", interfacekit_sensor_sample_rates, 0);

  /*
   * Document-method: input_count
   * call-seq:
   *   input_count -> Fixnum
   *
   * Returns an integer which represents the number of digital inputs provided
   * by this interface kit, or nil if the device has yet to be connected.
   */
  rb_define_method(c_InterfaceKit, "input_count", interfacekit_input_count, 0);

  /*
   * Document-method: output_count
   * call-seq:
   *   output_count -> Fixnum
   *
   * Returns an integer which represents the number of digital outputs provided
   * by this interface kit, or nil if the device has yet to be connected.
   */
  rb_define_method(c_InterfaceKit, "output_count", interfacekit_output_count, 0);

  /*
   * Document-method: sensor_count
   * call-seq:
   *   sensor_count -> Fixnum
   *
   * Returns an integer which represents the number of analog inputs provided
   * by this interface kit, or nil if the device has yet to be connected.
   */
  rb_define_method(c_InterfaceKit, "sensor_count", interfacekit_sensor_count, 0);

  /*
   * Document-method: ratiometric?
   * call-seq:
   *   ratiometric? -> Boolean
   *
   * Returns true if the analog sensors are currently in ratiometric mode. False
   * if the controller is set to a stable voltage reference. Read more about this
   * feature in the {Analog Inputs Primer}[http://www.phidgets.com/docs/Analog_Input_Primer#Ratiometric_Configuration]
   */
  rb_define_method(c_InterfaceKit, "ratiometric?", interfacekit_is_ratiometric, 0);

  /*
   * Document-method: ratiometric=
   * call-seq:
   *   ratiometric=(Boolean state) -> Boolean
   *
   * Sets the analog sensor voltage reference to ratiometric mode on true, or
   * stable on false. Read more about this feature in the 
   * {Analog Inputs Primer}[http://www.phidgets.com/docs/Analog_Input_Primer#Ratiometric_Configuration]
   */
  rb_define_method(c_InterfaceKit, "ratiometric=", interfacekit_ratiometric_set, 1);

  /*
   * Document-method: data_rates_max
   * call-seq:
   *   data_rates_max -> Array
   *
   * Returns an array of ints which specify the maximum data_rate value that can 
   * be configured on each analog input sensor, or nil if the device has yet to 
   * be connected.
   */
  rb_define_method(c_InterfaceKit, "data_rates_max", interfacekit_data_rates_max, 0);

  /*
   * Document-method: data_rates_min
   * call-seq:
   *   data_rates_min -> Array
   *
   * Returns an array of ints which specify the minimum data_rate value that can 
   * be configured on each analog input sensor, or nil if the device has yet to 
   * be connected.
   */
  rb_define_method(c_InterfaceKit, "data_rates_min", interfacekit_data_rates_min, 0);

  /*
   * Document-method: data_rates
   * call-seq:
   *   data_rates -> Array
   *
   * Returns an array of either ints or nils, which specify the current data_rate 
   * for each analog input sensor. A nil indicates that this input is in change_trigger
   * mode. If the whole return value is nil, this means that the device hasn't been
   * connected yet.
   */
  rb_define_method(c_InterfaceKit, "data_rates", interfacekit_data_rates, 0);

  /*
   * Document-method: change_triggers
   * call-seq:
   *   change_triggers -> Array
   *
   * Returns an array of either ints or nils, which specify the current change_trigger
   * threshold for each analog input sensor. A nil indicates that this input is 
   * in data_rate mode. If the whole return value is nil, this means that the 
   * device hasn't been connected yet.
   */
  rb_define_method(c_InterfaceKit, "change_triggers", interfacekit_change_triggers, 0);

  /*
   * Document-method: inputs
   * call-seq:
   *   inputs -> Array
   *
   * Returns an array of bools, which specify the current digital input states. 
   * A return of nil indicates that this device is unplugged
   */
  rb_define_method(c_InterfaceKit, "inputs", interfacekit_inputs, 0);

  /*
   * Document-method: outputs
   * call-seq:
   *   outputs -> Array
   *
   * Returns an array of bools, which specify the current digital output states.
   * A return of nil indicates that this device is unplugged
   */
  rb_define_method(c_InterfaceKit, "outputs", interfacekit_outputs, 0);

  /*
   * Document-method: sensors
   * call-seq:
   *   sensors -> Array
   *
   * Returns an array of ints, which specify the current analog sensor states. A 
   * return of nil indicates that this device is unplugged
   */
  rb_define_method(c_InterfaceKit, "sensors", interfacekit_sensors, 0);

  /*
   * Document-method: sensor_raw
   * call-seq:
   *   sensor_raw(Fixnum) -> Fixnum
   *
   * This method returns the "raw" value of the sensor at the provided offset.
   * For more information on raw sensor values read up on the 
   * CPhidgetInterfaceKit_getSensorRawValue[http://www.phidgets.com/documentation/web/cdoc/group__phidifkit.html] 
   * function. 
   */
  rb_define_method(c_InterfaceKit, "sensor_raw", interfacekit_sensor_raw, 1);

  /*
   * Document-method: output
   * call-seq:
   *   output(Fixnum, Boolean) -> Boolean
   *
   * This method sets the value of the specified digital output at the provided 
   * Fixnum offset to the Boolean state (true is on, false is off). The return
   * value is simply what was specified for the input state.
   * For more information on output control, you can read up on the
   * CPhidgetInterfaceKit_setOutputState[http://www.phidgets.com/documentation/web/cdoc/group__phidifkit.html] 
   * function. 
   */
  rb_define_method(c_InterfaceKit, "output", interfacekit_output_set, 2);

  /*
   * Document-method: data_rate
   * call-seq:
   *   data_rate(Fixnum index, Fixnum rate) -> Fixnum
   *
   * This method sets the data rate for the analog sensor at the provided index
   * to the rate specified. This rate must be between data_rate_min[index] and
   * data_rate_max[index]. The return value is simply what was specified for 
   * the rate. Keep in mind that setting a data rate, reverts the change_trigger
   * for that input to nil.
   * For more information on how analog inputs are polled, you can read up on the
   * Phidget {Analog Input Primer}[http://www.phidgets.com/docs/Analog_Input_Primer] 
   */
   rb_define_method(c_InterfaceKit, "data_rate", interfacekit_data_rate_set, 2);

  /*
   * Document-method: change_trigger
   * call-seq:
   *   change_trigger(Fixnum index, Fixnum threshold) -> Fixnum
   *
   * This method sets the change trigger for the analog sensor at the provided 
   * index the specified threshold. This threshould must be between 1 and 1000.
   * The return value is simply what was specified for the rate. Keep in mind 
   * that setting a data rate, reverts the data_rate for that input to nil.
   * For more information on how analog inputs are polled, you can read up on the
   * Phidget {Analog Input Primer}[http://www.phidgets.com/docs/Analog_Input_Primer] 
   */
  rb_define_method(c_InterfaceKit, "change_trigger", interfacekit_change_trigger_set, 2);
}

VALUE interfacekit_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  
  InterfaceKitInfo *ifkit_info = ALLOC(InterfaceKitInfo); 
  memset(ifkit_info, 0, sizeof(InterfaceKitInfo));
  ifkit_info->is_ratiometric = false;
  ifkit_info->is_data_rates_known = false;

  CPhidgetInterfaceKitHandle interfacekit = 0;
  ensure(CPhidgetInterfaceKit_create(&interfacekit));

  info->handle = (CPhidgetHandle)interfacekit;
  info->on_type_attach = interfacekit_on_attach;
  info->on_type_detach = interfacekit_on_detach;
  info->on_type_free = interfacekit_on_free;
  info->type_info = ifkit_info;

  ensure(CPhidgetInterfaceKit_set_OnInputChange_Handler(interfacekit, interfacekit_on_digital_change, info));
  ensure(CPhidgetInterfaceKit_set_OnSensorChange_Handler(interfacekit, interfacekit_on_analog_change, info));

  return rb_call_super(1, &serial);
}

VALUE interfacekit_close(VALUE self) {
  PhidgetInfo *info = device_info(self);

  ensure(CPhidgetInterfaceKit_set_OnInputChange_Handler((CPhidgetInterfaceKitHandle) info->handle, NULL, NULL));
  ensure(CPhidgetInterfaceKit_set_OnSensorChange_Handler((CPhidgetInterfaceKitHandle) info->handle, NULL, NULL));

  return rb_call_super(0,NULL);
}

VALUE interfacekit_sensor_sample_rates(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  if (!ifkit_info->is_analog_input_count_known) return Qnil;

  int *rates_in_hz = ALLOC_N(int, ifkit_info->analog_input_count);
  for(int i=0; i<ifkit_info->analog_input_count; i++)
    rates_in_hz[i] = ifkit_info->analog_sample_rates[i].in_hz;

  VALUE ret = int_array_to_rb(rates_in_hz, ifkit_info->analog_input_count);

  xfree(rates_in_hz);

  return ret;
}

VALUE interfacekit_input_sample_rates(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  if (!ifkit_info->is_digital_input_count_known) return Qnil;

  int *rates_in_hz = ALLOC_N(int, ifkit_info->digital_input_count);
  for(int i=0; i<ifkit_info->digital_input_count; i++)
    rates_in_hz[i] = ifkit_info->digital_sample_rates[i].in_hz;

  VALUE ret = int_array_to_rb(rates_in_hz, ifkit_info->digital_input_count);

  xfree(rates_in_hz);

  return ret;
}

VALUE interfacekit_input_count(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_digital_input_count_known) ? 
    INT2FIX(ifkit_info->digital_input_count) : Qnil;
}

VALUE interfacekit_output_count(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_digital_output_count_known) ? 
    INT2FIX(ifkit_info->digital_output_count) : Qnil;
}

VALUE interfacekit_sensor_count(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_analog_input_count_known) ? 
    INT2FIX(ifkit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_is_ratiometric(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_ratiometric) ? Qtrue : Qfalse;
}

VALUE interfacekit_ratiometric_set(VALUE self, VALUE is_ratiometric) {
  PhidgetInfo *info = device_info(self);
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  if (TYPE(is_ratiometric) == T_TRUE) 
    ifkit_info->is_ratiometric = true;
  else if (TYPE(is_ratiometric) == T_FALSE)
    ifkit_info->is_ratiometric = false;
  else
    rb_raise(rb_eTypeError, MSG_RATIOMETRIC_MUST_BE_BOOL);

  if (info->is_attached)
    ensure(interfacekit_assert_ratiometric_state( info ));

  return is_ratiometric;
}

VALUE interfacekit_data_rates_max(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_data_rates_known) ? 
    int_array_to_rb(ifkit_info->data_rates_max, ifkit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_data_rates_min(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_data_rates_known) ? 
    int_array_to_rb(ifkit_info->data_rates_min, ifkit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_data_rates(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_data_rates_known) ? 
    int_array_zeronils_to_rb(ifkit_info->data_rates, ifkit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_change_triggers(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return (ifkit_info->is_data_rates_known) ? 
    int_array_to_rb(ifkit_info->sensor_change_triggers, ifkit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_inputs(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return phidgetbool_array_to_rb(ifkit_info->digital_input_states, 
    ifkit_info->digital_input_count);
}

VALUE interfacekit_outputs(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return phidgetbool_array_to_rb(ifkit_info->digital_output_states,
    ifkit_info->digital_output_count);
}

VALUE interfacekit_sensors(VALUE self) {
  InterfaceKitInfo *ifkit_info = device_type_info(self);

  return int_array_to_rb(ifkit_info->analog_input_states, 
    ifkit_info->analog_input_count);
}

VALUE interfacekit_sensor_raw(VALUE self, VALUE index) {
  PhidgetInfo *info = device_info(self);
  InterfaceKitInfo *ifkit_info = info->type_info;

  int ret;

  if ((TYPE(index) != T_FIXNUM))
    rb_raise(rb_eTypeError, MSG_SENSOR_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  if ((ifkit_info->analog_input_count == 0) || (index_int > (ifkit_info->analog_input_count-1)))
    rb_raise(rb_eTypeError, MSG_SENSOR_INDEX_TOO_HIGH);

  ensure(CPhidgetInterfaceKit_getSensorRawValue(
    (CPhidgetInterfaceKitHandle)info->handle,index_int,&ret));

  return INT2FIX(ret);
}

VALUE interfacekit_output_set(VALUE self, VALUE index, VALUE is_on) {
  PhidgetInfo *info = device_info(self);
  InterfaceKitInfo *ifkit_info = info->type_info;

  if (TYPE(index) != T_FIXNUM)
    rb_raise(rb_eTypeError, MSG_OUTPUT_INDEX_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  if ((ifkit_info->digital_output_count == 0) || (index_int > (ifkit_info->digital_output_count-1)))
    rb_raise(rb_eTypeError, MSG_OUTPUT_INDEX_TOO_HIGH);

  int phidget_bool;
  
  if (TYPE(is_on) == T_TRUE) 
    phidget_bool = PTRUE;
  else if (TYPE(is_on) == T_FALSE)
    phidget_bool = PFALSE;
  else
    rb_raise(rb_eTypeError, MSG_OUTPUT_VALUE_MUST_BE_BOOL);

  ensure(CPhidgetInterfaceKit_setOutputState(
    (CPhidgetInterfaceKitHandle)info->handle, index_int, phidget_bool));

  ifkit_info->digital_output_states[index_int] = phidget_bool;

  return is_on;
}

VALUE interfacekit_data_rate_set(VALUE self, VALUE index, VALUE rate) {
  PhidgetInfo *info = device_info(self);
  InterfaceKitInfo *ifkit_info = info->type_info;

  if (TYPE(index) != T_FIXNUM)
    rb_raise(rb_eTypeError, MSG_SENSOR_INDEX_MUST_BE_FIXNUM);

  if (TYPE(rate) != T_FIXNUM)
    rb_raise(rb_eTypeError, MSG_DATA_RATE_VALUE_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  if ((ifkit_info->analog_input_count == 0) || (index_int > (ifkit_info->analog_input_count-1)))
    rb_raise(rb_eTypeError, MSG_SENSOR_INDEX_TOO_HIGH);

  int rate_int = FIX2INT(rate);
  if ((rate_int > ifkit_info->data_rates_min[index_int]) || (rate_int < ifkit_info->data_rates_max[index_int]))
    rb_raise(rb_eTypeError, MSG_DATA_RATE_EXCEEDS_LIMIT);
  
  ensure(CPhidgetInterfaceKit_setDataRate(
    (CPhidgetInterfaceKitHandle)info->handle, index_int, rate_int));

  ifkit_info->data_rates[index_int] = rate_int;
  ifkit_info->sensor_change_triggers[index_int] = 0;

  return rate;
}

VALUE interfacekit_change_trigger_set(VALUE self, VALUE index, VALUE rate_thresh) {
  PhidgetInfo *info = device_info(self);
  InterfaceKitInfo *ifkit_info = info->type_info;

  if (TYPE(index) != T_FIXNUM)
    rb_raise(rb_eTypeError, MSG_SENSOR_INDEX_MUST_BE_FIXNUM);

  if (TYPE(rate_thresh) != T_FIXNUM)
    rb_raise(rb_eTypeError, MSG_CHANGE_TRIG_VALUE_MUST_BE_FIXNUM);

  int index_int = FIX2INT(index);
  if ((ifkit_info->analog_input_count == 0) || (index_int > (ifkit_info->analog_input_count-1)))
    rb_raise(rb_eTypeError, MSG_SENSOR_INDEX_TOO_HIGH);

  int rate_thresh_int = FIX2INT(rate_thresh);
  // These limits are specified in the phidget documentation (and, just kind of make sense)
  if ((rate_thresh_int < 1) || (rate_thresh_int > 1000))
    rb_raise(rb_eTypeError, MSG_CHANGE_TRIG_EXCEEDS_LIMIT);
  
  ensure(CPhidgetInterfaceKit_setSensorChangeTrigger(
    (CPhidgetInterfaceKitHandle)info->handle, index_int, rate_thresh_int));

  ifkit_info->data_rates[index_int] = 0;
  ifkit_info->sensor_change_triggers[index_int] = rate_thresh_int;

  return rate_thresh;
}
