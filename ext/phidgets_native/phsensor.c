#include "phidgets_native.h"

VALUE phsensor_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetPHSensorHandle phsensor  = 0;
  ensure(CPhidgetPHSensor_create(&phsensor));
  info->handle = (CPhidgetHandle)phsensor ;
  return rb_call_super(1, &serial);
}
