#include "phidgets_native.h"

VALUE encoder_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetEncoderHandle encoder = 0;
  ensure(CPhidgetEncoder_create(&encoder));
  info->handle = (CPhidgetHandle)encoder;
  return rb_call_super(1, &serial);
}
