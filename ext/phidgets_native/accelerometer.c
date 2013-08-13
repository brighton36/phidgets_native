#include "phidgets_native.h"

VALUE accelerometer_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);

  CPhidgetAccelerometerHandle accelerometer = 0;
  ensure(CPhidgetAccelerometer_create(&accelerometer));

  info->handle = (CPhidgetHandle)accelerometer;
  return rb_call_super(1, &serial);
}
