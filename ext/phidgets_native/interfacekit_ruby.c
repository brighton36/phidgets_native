#include "phidgets_native.h"

const char MSG_RATIOMETRIC_MUST_BE_BOOL[] = "ratiometric can only be either true or false";

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
   * Document-method: sample_rate
   * call-seq:
   *   sample_rate -> FixNum
   *
   * For most Phidgets, an event handler processes the device state changes at
   * some regular interval. For these devices, this method will return the rate
   * of state changes measured in Hz.
   */
  rb_define_method(c_InterfaceKit, "sample_rate", interfacekit_sample_rate, 0);

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
   TODO:
  rb_define_method(c_InterfaceKit, "output_set", interfacekit_output_set, 2);
  rb_define_method(c_InterfaceKit, "data_rate_set", interfacekit_data_rate_set, 2);
  rb_define_method(c_InterfaceKit, "change_trigger_set", interfacekit_change_trigger_set, 2);
  */
}

VALUE interfacekit_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  
  InterfaceKitInfo *interfacekit_info = ALLOC(InterfaceKitInfo); 
  memset(interfacekit_info, 0, sizeof(InterfaceKitInfo));
  interfacekit_info->is_ratiometric = false;
  interfacekit_info->is_data_rates_known = false;
  interfacekit_info->sample_rate = sample_create();

  CPhidgetInterfaceKitHandle interfacekit = 0;
  ensure(CPhidgetInterfaceKit_create(&interfacekit));

  ensure(CPhidgetInterfaceKit_set_OnInputChange_Handler(interfacekit, interfacekit_on_digital_change, info));
  ensure(CPhidgetInterfaceKit_set_OnSensorChange_Handler(interfacekit, interfacekit_on_analog_change, info));

  info->handle = (CPhidgetHandle)interfacekit;
  info->on_type_attach = interfacekit_on_attach;
  info->on_type_detach = interfacekit_on_detach;
  info->on_type_free = interfacekit_on_free;
  info->type_info = interfacekit_info;

  return rb_call_super(1, &serial);
}

VALUE interfacekit_close(VALUE self) {
  PhidgetInfo *info = device_info(self);

  ensure(CPhidgetInterfaceKit_set_OnInputChange_Handler((CPhidgetInterfaceKitHandle) info->handle, NULL, NULL));
  ensure(CPhidgetInterfaceKit_set_OnSensorChange_Handler((CPhidgetInterfaceKitHandle) info->handle, NULL, NULL));

  return rb_call_super(0,NULL);
}

VALUE interfacekit_sample_rate(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return INT2FIX(interfacekit_info->sample_rate->in_hz);
}

VALUE interfacekit_input_count(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_digital_input_count_known) ? 
    INT2FIX(interfacekit_info->digital_input_count) : Qnil;
}

VALUE interfacekit_output_count(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_digital_output_count_known) ? 
    INT2FIX(interfacekit_info->digital_output_count) : Qnil;
}

VALUE interfacekit_sensor_count(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_analog_input_count_known) ? 
    INT2FIX(interfacekit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_is_ratiometric(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_ratiometric) ? Qtrue : Qfalse;
}

VALUE interfacekit_ratiometric_set(VALUE self, VALUE is_ratiometric) {
  PhidgetInfo *info = device_info(self);
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  if (TYPE(is_ratiometric) == T_TRUE) 
    interfacekit_info->is_ratiometric = true;
  else if (TYPE(is_ratiometric) == T_FALSE)
    interfacekit_info->is_ratiometric = false;
  else
    rb_raise(rb_eTypeError, MSG_RATIOMETRIC_MUST_BE_BOOL);

  if (info->is_attached)
    ensure(interfacekit_assert_ratiometric_state( info ));

  return is_ratiometric;
}

VALUE interfacekit_data_rates_max(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_data_rates_known) ? 
    int_array_to_rb(interfacekit_info->data_rates_max, interfacekit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_data_rates_min(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_data_rates_known) ? 
    int_array_to_rb(interfacekit_info->data_rates_min, interfacekit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_data_rates(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_data_rates_known) ? 
    int_array_zeronils_to_rb(interfacekit_info->data_rates, interfacekit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_change_triggers(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return (interfacekit_info->is_data_rates_known) ? 
    int_array_to_rb(interfacekit_info->sensor_change_triggers, interfacekit_info->analog_input_count) : Qnil;
}

VALUE interfacekit_inputs(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return phidgetbool_array_to_rb(interfacekit_info->digital_input_states, 
    interfacekit_info->digital_input_count);
}

VALUE interfacekit_outputs(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return phidgetbool_array_to_rb(interfacekit_info->digital_output_states,
    interfacekit_info->digital_output_count);
}

VALUE interfacekit_sensors(VALUE self) {
  InterfaceKitInfo *interfacekit_info = device_type_info(self);

  return int_array_to_rb(interfacekit_info->analog_input_states, 
    interfacekit_info->analog_input_count);
}
