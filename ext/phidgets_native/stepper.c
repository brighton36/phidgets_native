#include "phidgets_native.h"

VALUE stepper_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetStepperHandle stepper = 0;
  ensure(CPhidgetStepper_create(&stepper));
  info->handle = (CPhidgetHandle)stepper;
  return rb_call_super(1, &serial);
}
