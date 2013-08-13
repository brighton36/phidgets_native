#include "phidgets_native.h"

VALUE temperaturesensor_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetTemperatureSensorHandle temperaturesensor = 0;
  ensure(CPhidgetTemperatureSensor_create(&temperaturesensor));
  info->handle = (CPhidgetHandle)temperaturesensor;
  return rb_call_super(1, &serial);
}
