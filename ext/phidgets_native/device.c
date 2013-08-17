#include "phidgets_native.h"

PhidgetInfo *device_info(VALUE self) {
  PhidgetInfo *info;
  Data_Get_Struct( self, PhidgetInfo, info );

  return info;
}

void *device_type_info(VALUE self) {
  PhidgetInfo *info;
  Data_Get_Struct( self, PhidgetInfo, info );

  return info->type_info;
}

/*
 * The first time this is called, we're a little wonk. Mostly this is because
 * we're not including microseconds. If we included microseconds, then we could
 * process a block if the seconds component were different than the last, and our
 * microseconds were greater than the starting microsecond amount.
 */
void device_sample(PhidgetInfo *info, CPhidget_Timestamp *ts) {
  unsigned long now_seconds;

  info->samples_in_second++;

  if (ts == NULL) {
    // We can pull this ourself using the system times:
    struct timeval now;
    gettimeofday(&now, NULL);

    now_seconds = (unsigned long) now.tv_sec;
  } else
    // Seems like the module has a timestamp for us to use:
    now_seconds = (unsigned long) ts->seconds;

  // Sample tracking
  // We need the > 0 for the case of the first time we've ever entered this loop
  if ( info->last_second == 0)
    info->last_second = now_seconds;
  else if ( info->last_second != now_seconds ) {
    info->sample_rate = (double) info->samples_in_second / 
        (double) (now_seconds - info->last_second);
    info->samples_in_second = 0;
    info->last_second = now_seconds;

  }

  return;
}

int CCONV device_on_attach(CPhidgetHandle phid, void *userptr)
{
  int serialNo;
  report(CPhidget_getSerialNumber(phid, &serialNo));

  // Populate our data structures with what we know about this device:
  PhidgetInfo *info = userptr;
  info->is_attached = true;
  
  gettimeofday(&info->attached_at, &info->attached_at_tz);

  // Phidget Attributes:
  report(CPhidget_getDeviceType(phid, &info->type));
  report(CPhidget_getDeviceVersion(phid, &info->version));
  report(CPhidget_getDeviceClass(phid, &info->device_class));
  report(CPhidget_getDeviceID(phid, &info->device_id));
  report(CPhidget_getDeviceLabel(phid, &info->label));
  report(CPhidget_getDeviceName(phid, &info->name));

  return (info->on_type_attach) ? (*info->on_type_attach)(phid, info) : 0;
}

int CCONV device_on_detach(CPhidgetHandle phid, void *userptr) {
  int serialNo;
  report(CPhidget_getSerialNumber(phid, &serialNo));

  PhidgetInfo *info = userptr;
  info->is_attached = false;

  // These would be misleading to report if there's no device:
  info->sample_rate = 0;
  info->samples_in_second = 0;
  info->last_second = 0;

  return (info->on_type_detach) ? (*info->on_type_detach)(phid, info) : 0;
}

int CCONV device_on_error(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *unknown) {
  CPhidget_log(PHIDGET_LOG_ERROR, "N/A", "PhidgetNative OnError: %s", unknown);
  return 0;
}

void device_free(PhidgetInfo *info) {
  if (info) {
    if (info->handle) {
	    report(CPhidget_close((CPhidgetHandle)info->handle));
	    report(CPhidget_delete((CPhidgetHandle)info->handle));
    }

    if (info->on_type_free)
      (*info->on_type_free)(info->type_info);

    xfree(info);
  }
}

