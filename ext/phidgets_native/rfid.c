#include "phidgets_native.h"

VALUE rfid_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetRFIDHandle rfid = 0;
  ensure(CPhidgetRFID_create(&rfid));
  info->handle = (CPhidgetHandle)rfid;
  return rb_call_super(1, &serial);
}
