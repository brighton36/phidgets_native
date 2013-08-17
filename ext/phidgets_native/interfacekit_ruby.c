#include "phidgets_native.h"

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
}

VALUE interfacekit_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  
  InterfaceKitInfo *interfacekit_info = ALLOC(InterfaceKitInfo); 
  memset(interfacekit_info, 0, sizeof(InterfaceKitInfo));

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

