#include "phidgets_native.h"

/* 
 * Document-class: PhidgetsNative::LED < PhidgetsNative::Device
 *
 * This class is a stub, and is currently in need of an actual implementation.
 * Nonetheless, all of the methods from its parent class PhidgetsNative::Device are 
 * available.
 */

void Init_phidgets_native_led(VALUE m_Phidget) {
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

 	VALUE c_LED = rb_define_class_under(m_Phidget, "LED", c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_LED, "initialize", led_initialize, 1);
}

VALUE led_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetLEDHandle led = 0;
  ensure(CPhidgetLED_create(&led));
  info->handle = (CPhidgetHandle)led;
  return rb_call_super(1, &serial);
}
