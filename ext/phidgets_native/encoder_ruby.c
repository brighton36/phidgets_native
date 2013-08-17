#include "phidgets_native.h"

/* 
 * Document-class: PhidgetsNative::Encoder < PhidgetsNative::Device
 *
 * This class is a stub, and is currently in need of an actual implementation.
 * Nonetheless, all of the methods from its parent class PhidgetsNative::Device are 
 * available.
 */

void Init_phidgets_native_encoder(VALUE m_Phidget) {
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

 	VALUE c_Encoder = rb_define_class_under(m_Phidget, "Encoder", c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_Encoder, "initialize", encoder_initialize, 1);
  
}

VALUE encoder_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetEncoderHandle encoder = 0;
  ensure(CPhidgetEncoder_create(&encoder));
  info->handle = (CPhidgetHandle)encoder;
  return rb_call_super(1, &serial);
}
