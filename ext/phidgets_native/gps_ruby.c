#include "phidgets_native.h"

/* 
 * Document-class: PhidgetsNative::GPS < PhidgetsNative::Device
 *
 * This class provides functionality specific to the "GPS" device class. 
 * Primarily, this includes reporting of latitude/longitude, velocity/heading 
 * and time.
 */

void Init_phidgets_native_gps(VALUE m_Phidget) {
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

  VALUE c_Gps = rb_define_class_under(m_Phidget,"GPS",c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_Gps, "initialize", gps_initialize, 1);

  /*
   * Document-method: close
   * call-seq:
   *   close -> nil
   *
   * This method will unregister the phidget event handlers, and free up all
   * API resources associated with the phidget. This is an optional, but useful
   * way to remove the object's overhead before the GC kicks in and actually 
   * frees the resource.
   */
  rb_define_method(c_Gps, "close", gps_close, 0);

  /*
   * Document-method: latitude
   * call-seq:
   *   latitude -> Float
   *
   * Returns the current latititude, as reported by the most recent gps update.
   */
  rb_define_method(c_Gps, "latitude", gps_latitude, 0);
  
  /*
   * Document-method: longitude
   * call-seq:
   *   longitude -> Float
   *
   * Returns the current longitude, as reported by the most recent gps update.
   */
  rb_define_method(c_Gps, "longitude", gps_longitude, 0);

  /*
   * Document-method: altitude
   * call-seq:
   *   altitude -> Float
   *
   * Returns the current altitude in meters, as reported by the most recent gps update.
   */
  rb_define_method(c_Gps, "altitude", gps_altitude, 0);

  /*
   * Document-method: heading
   * call-seq:
   *   heading -> Float
   *
   * Returns the current heading in degrees, as reported by the most recent gps update.
   */
  rb_define_method(c_Gps, "heading", gps_heading, 0);

  /*
   * Document-method: velocity
   * call-seq:
   *   velocity -> Float
   *
   * Returns the current velocity in km/h, as reported by the most recent gps update.
   */
  rb_define_method(c_Gps, "velocity", gps_velocity, 0);

  /*
   * Document-method: is_fixed?
   * call-seq:
   *   is_fixed? -> Boolean
   *
   * Returns true or false, indicating whether the gps is fixed (aka "locked").
   */
  rb_define_method(c_Gps, "is_fixed?", gps_is_fixed, 0);

  /*
   * Document-method: now_at_utc
   * call-seq:
   *   now_at_utc -> Time
   *
   * Returns the GPS-reported time, in the UTC zone.
   */
  rb_define_method(c_Gps, "now_at_utc", gps_now_at_utc, 0);

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

VALUE gps_close(VALUE self) {
  PhidgetInfo *info = device_info(self);

  ensure(CPhidgetGPS_set_OnPositionChange_Handler((CPhidgetGPSHandle)info->handle, NULL, NULL));
 	ensure(CPhidgetGPS_set_OnPositionFixStatusChange_Handler((CPhidgetGPSHandle)info->handle, NULL, NULL));

  return rb_call_super(0,NULL);
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
