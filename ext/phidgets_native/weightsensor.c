#include "phidgets_native.h"

VALUE weightsensor_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetWeightSensorHandle weightsensor = 0;
  ensure(CPhidgetWeightSensor_create(&weightsensor));
  info->handle = (CPhidgetHandle)weightsensor;
  return rb_call_super(1, &serial);
}
