#include "phidgets_native.h"

VALUE led_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetLEDHandle led = 0;
  ensure(CPhidgetLED_create(&led));
  info->handle = (CPhidgetHandle)led;
  return rb_call_super(1, &serial);
}
