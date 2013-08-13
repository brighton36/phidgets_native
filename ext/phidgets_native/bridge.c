#include "phidgets_native.h"

VALUE bridge_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetBridgeHandle bridge = 0;
  ensure(CPhidgetBridge_create(&bridge));
  info->handle = (CPhidgetHandle)bridge;
  return rb_call_super(1, &serial);
}
