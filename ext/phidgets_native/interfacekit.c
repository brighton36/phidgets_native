#include "phidgets_native.h"

VALUE interfacekit_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetInterfaceKitHandle interfacekit = 0;
  ensure(CPhidgetInterfaceKit_create(&interfacekit));
  info->handle = (CPhidgetHandle)interfacekit;
  return rb_call_super(1, &serial);
}

