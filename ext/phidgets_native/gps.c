#include "phidgets_native.h"

int CCONV gps_on_position_change(CPhidgetGPSHandle gps, void *userptr, double latitude, double longitude, double altitude) {
  PhidgetInfo *info = userptr;
  GpsInfo *gps_info = info->type_info;

  // Calculate the sample time:
  device_sample(info, NULL);

  // Get the GPS time:
	GPSDate date;
	GPSTime time;

	if ( (CPhidgetGPS_getDate(gps, &date) == EPHIDGET_OK) && 
    (CPhidgetGPS_getTime(gps, &time) == EPHIDGET_OK) ) {
    gps_info->now_at_utc.tm_sec = time.tm_sec;
    gps_info->now_at_utc.tm_min = time.tm_min;
    gps_info->now_at_utc.tm_hour = time.tm_hour;
    gps_info->now_at_utc.tm_mday = date.tm_mday;
    gps_info->now_at_utc.tm_mon = date.tm_mon;
    gps_info->now_at_utc.tm_year = date.tm_year;
    gps_info->now_at_utc_ms = (time.tm_ms >= 1000) ? 0 : time.tm_ms;
    gps_info->is_now_at_utc_known = true;
  } else
    gps_info->is_now_at_utc_known = false;
  
  // Get Position values:
  if (gps_info->latitude != PUNK_DBL) {
    gps_info->is_latitude_known = true;
    gps_info->latitude = latitude;
  } else
    gps_info->is_latitude_known = false;

  if (gps_info->longitude != PUNK_DBL) {
    gps_info->is_longitude_known = true;
    gps_info->longitude = longitude;
  } else
    gps_info->is_longitude_known = false;

  if (gps_info->altitude != PUNK_DBL) {
    gps_info->is_altitude_known = true;
    gps_info->altitude = altitude;
  } else
    gps_info->is_altitude_known = false;

  double heading, velocity;
  if (CPhidgetGPS_getHeading(gps, &heading) == EPHIDGET_OK) {
    gps_info->heading = heading;
    gps_info->is_heading_known = true;
  } else
    gps_info->is_heading_known = false;

  if (CPhidgetGPS_getVelocity(gps, &velocity) == EPHIDGET_OK) {
    gps_info->velocity = velocity;
    gps_info->is_velocity_known = true;
  } else
    gps_info->is_velocity_known = false;

	return 0;
}

int CCONV gps_on_fix_change(CPhidgetGPSHandle gps, void *userptr, int status) {
  PhidgetInfo *info = userptr;
  GpsInfo *gps_info = info->type_info;

  // Calculate the sample time:
  device_sample(info, NULL);

  // I'm fairly certain that status is always either 1 or 0
  gps_info->is_fixed = (status) ? true : false;

	return 0;
}

VALUE gps_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);
  
  GpsInfo *gps_info = ALLOC(GpsInfo); 
  memset(gps_info, 0, sizeof(GpsInfo));

  CPhidgetGPSHandle gps = 0;

  ensure(CPhidgetGPS_create(&gps));

  ensure(CPhidgetGPS_set_OnPositionChange_Handler( gps, gps_on_position_change, info));
 	ensure(CPhidgetGPS_set_OnPositionFixStatusChange_Handler(gps, gps_on_fix_change, info));

  info->handle = (CPhidgetHandle)gps;
  info->on_type_detach = gps_on_detach;
  info->on_type_free = gps_on_free;
  info->type_info = gps_info;

  return rb_call_super(1, &serial);
}

int CCONV gps_on_detach(CPhidgetHandle phidget, void *userptr) {
  PhidgetInfo *info = userptr;
  GpsInfo *gps_info = info->type_info;

  // Zero out the polled values, which happens to be everything so far:
  memset(gps_info, 0, sizeof(GpsInfo));
  
  return 0;
}

void gps_on_free(void *type_info) {
  GpsInfo *gps_info = type_info;

  xfree(gps_info);
  return;
}

VALUE gps_latitude(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  return (gps_info->is_latitude_known) ? DBL2NUM(gps_info->latitude) : Qnil;
}

VALUE gps_longitude(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  return (gps_info->is_longitude_known) ? DBL2NUM(gps_info->longitude) : Qnil;
}

VALUE gps_altitude(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  return (gps_info->is_altitude_known) ? DBL2NUM(gps_info->altitude) : Qnil;
}

VALUE gps_heading(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  return (gps_info->is_heading_known) ? DBL2NUM(gps_info->heading) : Qnil;
}

VALUE gps_velocity(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  return (gps_info->is_velocity_known) ? DBL2NUM(gps_info->velocity) : Qnil;
}

VALUE gps_is_fixed(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  return (gps_info->is_fixed) ? Qtrue : Qfalse;
}

VALUE gps_now_at_utc(VALUE self) {
  GpsInfo *gps_info = device_type_info(self);

  if (!gps_info->is_now_at_utc_known) return Qnil;

  VALUE c_Time = rb_const_get(rb_cObject, rb_intern("Time"));

  char *now_as_string = ALLOC_N(char, 28);
  sprintf(now_as_string, "%04d-%02d-%02d %02d:%02d:%02d.%03d UTC",
    gps_info->now_at_utc.tm_year,
    gps_info->now_at_utc.tm_mon,
    gps_info->now_at_utc.tm_mday,
    gps_info->now_at_utc.tm_hour,
    gps_info->now_at_utc.tm_min,
    gps_info->now_at_utc.tm_sec,
    gps_info->now_at_utc_ms);

  VALUE now = rb_funcall(c_Time, rb_intern("parse"), 2, 
    rb_str_new2(now_as_string),
    rb_str_new2("%Y-%m-%d %H:%M:%S.%3N") );
 
  xfree(now_as_string);

  return now;
}
