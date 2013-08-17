#include "phidgets_native.h"

/* 
 * Document-class: PhidgetsNative::Bridge < PhidgetsNative::Device
 *
 * This class is a stub, and is currently in need of an actual implementation.
 * Nonetheless, all of the methods from its parent class PhidgetsNative::Device are 
 * available.
 */
void Init_phidgets_native_bridge(VALUE m_Phidget) {
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

 	VALUE c_Bridge = rb_define_class_under(m_Phidget, "Bridge", c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_Bridge, "initialize", bridge_initialize, 1);
}

VALUE bridge_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetBridgeHandle bridge = 0;
  ensure(CPhidgetBridge_create(&bridge));
  info->handle = (CPhidgetHandle)bridge;
  return rb_call_super(1, &serial);
}
