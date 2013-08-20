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

    if (info)
      xfree(info);
  }
}

