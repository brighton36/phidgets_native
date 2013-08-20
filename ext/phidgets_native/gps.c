#include "phidgets_native.h"

int CCONV gps_on_position_change(CPhidgetGPSHandle gps, void *userptr, double latitude, double longitude, double altitude) {
  PhidgetInfo *info = userptr;
  GpsInfo *gps_info = info->type_info;

  // Calculate the sample time:
  sample_tick(gps_info->sample_rate, NULL);

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
  sample_tick(gps_info->sample_rate, NULL);

  // I'm fairly certain that status is always either 1 or 0
  gps_info->is_fixed = (status) ? true : false;

	return 0;
}

int CCONV gps_on_detach(CPhidgetHandle phidget, void *userptr) {
  PhidgetInfo *info = userptr;
  GpsInfo *gps_info = info->type_info;

  gps_info->is_fixed = false;
  gps_info->is_latitude_known = false;
  gps_info->is_longitude_known = false;
  gps_info->is_altitude_known = false;
  gps_info->is_heading_known = false;
  gps_info->is_velocity_known = false;
  gps_info->is_now_at_utc_known = false;
  
  sample_zero(gps_info->sample_rate);

  return 0;
}

void gps_on_free(void *type_info) {
  GpsInfo *gps_info = type_info;

  if (gps_info->sample_rate)
    sample_free(gps_info->sample_rate);
  if (gps_info)
    xfree(gps_info);

  return;
}

