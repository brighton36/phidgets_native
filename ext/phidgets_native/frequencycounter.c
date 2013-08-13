#include "phidgets_native.h"

VALUE frequencycounter_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  CPhidgetFrequencyCounterHandle frequencycounter = 0;
  ensure(CPhidgetFrequencyCounter_create(&frequencycounter));
  info->handle = (CPhidgetHandle)frequencycounter;
  return rb_call_super(1, &serial);
}
