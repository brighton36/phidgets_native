#include "phidgets_native.h"

VALUE advancedservo_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetAdvancedServoHandle advancedservo = 0;
  ensure(CPhidgetAdvancedServo_create(&advancedservo));
  info->handle = (CPhidgetHandle)advancedservo;
  return rb_call_super(1, &serial);
}
