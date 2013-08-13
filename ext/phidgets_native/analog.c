#include "phidgets_native.h"

VALUE analog_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetAnalogHandle analog = 0;
  ensure(CPhidgetAnalog_create(&analog));
  info->handle = (CPhidgetHandle)analog;
  return rb_call_super(1, &serial);
}
