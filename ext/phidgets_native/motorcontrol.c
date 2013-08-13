#include "phidgets_native.h"

VALUE motorcontrol_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetMotorControlHandle motorcontrol = 0;
  ensure(CPhidgetMotorControl_create(&motorcontrol));
  info->handle = (CPhidgetHandle)motorcontrol;
  return rb_call_super(1, &serial);
}
