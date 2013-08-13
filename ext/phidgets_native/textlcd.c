#include "phidgets_native.h"

VALUE textlcd_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetTextLCDHandle textlcd = 0;
  ensure(CPhidgetTextLCD_create(&textlcd));
  info->handle = (CPhidgetHandle)textlcd;
  return rb_call_super(1, &serial);
}
