#include "phidgets_native.h"

VALUE ir_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetIRHandle ir = 0;
  ensure(CPhidgetIR_create(&ir));
  info->handle = (CPhidgetHandle)ir;
  return rb_call_super(1, &serial);
}
