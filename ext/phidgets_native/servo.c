#include "phidgets_native.h"

VALUE servo_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetServoHandle servo = 0;
  ensure(CPhidgetServo_create(&servo));
  info->handle = (CPhidgetHandle)servo;
  return rb_call_super(1, &serial);
}
