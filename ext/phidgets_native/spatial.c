#include "phidgets_native.h"

const char MSG_COMPASS_CORRECTION_NOT_ARRAY[] = "compass correction must be a 13 element array";
const char MSG_COMPASS_CORRECTION_MUST_BE_FLOAT[] = "compass correction elements must be float or fixnum";
const char MSG_DATA_RATE_MUST_BE_NUM[] = "data rate must be fixnum";

void spatial_on_free(void *type_info) {
  SpatialInfo *spatial_info = type_info;
  xfree(spatial_info->acceleration);
  xfree(spatial_info->acceleration_min);
  xfree(spatial_info->acceleration_max);
  xfree(spatial_info->gyroscope);
  xfree(spatial_info->gyroscope_min);
  xfree(spatial_info->gyroscope_max);
  xfree(spatial_info->compass);
  xfree(spatial_info->compass_min);
  xfree(spatial_info->compass_max);
  xfree(spatial_info);

  return;
}

int spatial_set_compass_correction_by_array(CPhidgetSpatialHandle phid, double *cc) {
  return ensure(CPhidgetSpatial_setCompassCorrectionParameters( phid,
    cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7], cc[8], cc[9], 
    cc[10], cc[11], cc[12] ));
}

int CCONV spatial_on_attach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  SpatialInfo *spatial_info = info->type_info;

  // Accelerometer Attributes:
  ensure(CPhidgetSpatial_getAccelerationAxisCount((CPhidgetSpatialHandle)phid, &spatial_info->accelerometer_axes));
  ensure(CPhidgetSpatial_getGyroAxisCount((CPhidgetSpatialHandle)phid, &spatial_info->gyro_axes));
  ensure(CPhidgetSpatial_getCompassAxisCount((CPhidgetSpatialHandle)phid, &spatial_info->compass_axes));
  ensure(CPhidgetSpatial_getDataRateMax((CPhidgetSpatialHandle)phid, &spatial_info->data_rate_max));
  ensure(CPhidgetSpatial_getDataRateMin((CPhidgetSpatialHandle)phid, &spatial_info->data_rate_min));

  // Dealloc if we're alloc'd, this will prevent memory leaks on device re-attachment:
  if (spatial_info->acceleration) xfree(spatial_info->acceleration);
  if (spatial_info->acceleration_min) xfree(spatial_info->acceleration_min);
  if (spatial_info->acceleration_max) xfree(spatial_info->acceleration_max);
  if (spatial_info->compass) xfree(spatial_info->compass);
  if (spatial_info->compass_min) xfree(spatial_info->compass_min);
  if (spatial_info->compass_max) xfree(spatial_info->compass_max);
  if (spatial_info->gyroscope) xfree(spatial_info->gyroscope);
  if (spatial_info->gyroscope_min) xfree(spatial_info->gyroscope_min);
  if (spatial_info->gyroscope_max) xfree(spatial_info->gyroscope_max);

  // Allocate space for our extents:
  spatial_info->acceleration = ALLOC_N(double, spatial_info->accelerometer_axes); 
  spatial_info->acceleration_min = ALLOC_N(double, spatial_info->accelerometer_axes); 
  spatial_info->acceleration_max = ALLOC_N(double, spatial_info->accelerometer_axes); 
  spatial_info->compass = ALLOC_N(double, spatial_info->compass_axes); 
  spatial_info->compass_min = ALLOC_N(double, spatial_info->compass_axes); 
  spatial_info->compass_max = ALLOC_N(double, spatial_info->compass_axes); 
  spatial_info->gyroscope = ALLOC_N(double, spatial_info->gyro_axes); 
  spatial_info->gyroscope_min = ALLOC_N(double, spatial_info->gyro_axes); 
  spatial_info->gyroscope_max = ALLOC_N(double, spatial_info->gyro_axes); 

  // Accelerometer
  for(int i=0; i < spatial_info->accelerometer_axes; i++) {
    ensure(CPhidgetSpatial_getAccelerationMin((CPhidgetSpatialHandle)phid, i, &spatial_info->acceleration_min[i]));
    ensure(CPhidgetSpatial_getAccelerationMax((CPhidgetSpatialHandle)phid, i, &spatial_info->acceleration_max[i]));
  }

  for(int i=0; i < spatial_info->compass_axes; i++) {
    ensure(CPhidgetSpatial_getMagneticFieldMin((CPhidgetSpatialHandle)phid, i, &spatial_info->compass_min[i]));
    ensure(CPhidgetSpatial_getMagneticFieldMax((CPhidgetSpatialHandle)phid, i, &spatial_info->compass_max[i]));
  }
  for(int i=0; i < spatial_info->gyro_axes; i++) {
    ensure(CPhidgetSpatial_getAngularRateMin((CPhidgetSpatialHandle)phid, i, &spatial_info->gyroscope_min[i]));
    ensure(CPhidgetSpatial_getAngularRateMax((CPhidgetSpatialHandle)phid, i, &spatial_info->gyroscope_max[i]));
  }

	// Set the data rate for the spatial events in milliseconds. 
  // Note that 1000/16 = 62.5 Hz
	ensure(CPhidgetSpatial_setDataRate((CPhidgetSpatialHandle)phid, spatial_info->data_rate));

  // Strictly speaking, this is entirely optional:
  if (spatial_info->has_compass_correction)
    spatial_set_compass_correction_by_array( (CPhidgetSpatialHandle)phid, 
        spatial_info->compass_correction);

  return 0;
}


int CCONV spatial_on_detach(CPhidgetHandle phidget, void *userptr) {
  PhidgetInfo *info = userptr;
  SpatialInfo *spatial_info = info->type_info;

  // These would be misleading to report if there's no device:
  memset(spatial_info->acceleration, 0, sizeof(double) * spatial_info->accelerometer_axes);
  memset(spatial_info->gyroscope, 0, sizeof(double) * spatial_info->gyro_axes);
  memset(spatial_info->compass, 0, sizeof(double) * spatial_info->compass_axes);
  spatial_info->last_microsecond = 0;

  return 0;
}

//callback that will run at datarate
//data - array of spatial event data structures that holds the spatial data packets that were sent in this event
//count - the number of spatial data event packets included in this event
int CCONV spatial_on_data(CPhidgetSpatialHandle spatial, void *userptr, CPhidgetSpatial_SpatialEventDataHandle *data, int count)
{
  PhidgetInfo *info = userptr;
  SpatialInfo *spatial_info = info->type_info;

  int i;
  for(i = 0; i < count; i++) {
    device_sample(info, &data[i]->timestamp);

    // Set the values to where they need to be:
    for(int j=0; j < spatial_info->accelerometer_axes; j++)
      spatial_info->acceleration[j] = data[i]->acceleration[j];

    // Sometimes the compass will return nonesense in the form of this constant
    // I'm fairly certain that simply checking the first element of the array 
    // will suffice, and when this is the case, we just keep the prior values:
    if (data[i]->magneticField[0] != PUNK_DBL)
      for(int j=0; j < spatial_info->compass_axes; j++)
        spatial_info->compass[j] = data[i]->magneticField[j];

    // Gyros get handled slightly different:
    // NOTE: Other people may have a better way to do this, but this is the method
    // I grabbed from the phidget sample. Maybe I should report these in radians...
    double timestamp = data[i]->timestamp.seconds + data[i]->timestamp.microseconds/MICROSECONDS_IN_SECOND;

    if (spatial_info->last_microsecond > 0) {
      double timechange = timestamp - spatial_info->last_microsecond;

      for(int j=0; j < spatial_info->gyro_axes; j++)
        spatial_info->gyroscope[j] += data[i]->angularRate[j] * timechange;
    }

    // We'll need this on the next go around:
    spatial_info->last_microsecond = timestamp;
  }

  return 0;
}

VALUE spatial_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);

  SpatialInfo *spatial_info = ALLOC(SpatialInfo); 
  memset(spatial_info, 0, sizeof(SpatialInfo));

  spatial_info->data_rate = DEFAULT_SPATIAL_DATA_RATE;

  // Setup a spatial handle
  CPhidgetSpatialHandle spatial = 0;
  ensure(CPhidgetSpatial_create(&spatial));
	ensure(CPhidgetSpatial_set_OnSpatialData_Handler(spatial, spatial_on_data, info));

  info->handle = (CPhidgetHandle)spatial;
  info->on_type_attach = spatial_on_attach;
  info->on_type_detach = spatial_on_detach;
  info->on_type_free = spatial_on_free;
  info->type_info = spatial_info;

  return rb_call_super(1, &serial);
}

VALUE spatial_close(VALUE self) {
  PhidgetInfo *info = device_info(self);

  ensure(CPhidgetSpatial_set_OnSpatialData_Handler((CPhidgetSpatialHandle)info->handle, NULL, NULL));

  return rb_call_super(0,NULL);
}

VALUE spatial_accelerometer_axes(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->accelerometer_axes) ? 
    INT2FIX(spatial_info->accelerometer_axes) : Qnil;
}  

VALUE spatial_compass_axes(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->compass_axes) ? INT2FIX(spatial_info->compass_axes) : Qnil;
}  

VALUE spatial_gyro_axes(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->gyro_axes) ? INT2FIX(spatial_info->gyro_axes) : Qnil;
}  

VALUE spatial_accelerometer(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->acceleration, spatial_info->accelerometer_axes);
}  

VALUE spatial_accelerometer_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->acceleration_min, spatial_info->accelerometer_axes);
}  

VALUE spatial_accelerometer_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->acceleration_max, spatial_info->accelerometer_axes);
}  

VALUE spatial_gyro(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  double *gyroscope_in_degrees;
  VALUE ret; 

  gyroscope_in_degrees = ALLOC_N(double, spatial_info->gyro_axes); 
  for(int i=0; i < spatial_info->gyro_axes; i++)
    gyroscope_in_degrees[i] = fmod(spatial_info->gyroscope[i], DEGREES_IN_CIRCLE); 

  ret = double_array_to_rb(gyroscope_in_degrees, spatial_info->gyro_axes);
  xfree(gyroscope_in_degrees);

  return ret;
}  

VALUE spatial_gyro_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->gyroscope_min, spatial_info->gyro_axes);
}  

VALUE spatial_gyro_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->gyroscope_max, spatial_info->gyro_axes);
}  

VALUE spatial_compass(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->compass, spatial_info->compass_axes);
}  

VALUE spatial_compass_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->compass_min, spatial_info->compass_axes);
}  

VALUE spatial_compass_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->compass_max, spatial_info->compass_axes);
}  

VALUE spatial_zero_gyro(VALUE self) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  ensure(CPhidgetSpatial_zeroGyro((CPhidgetSpatialHandle) info->handle));
  memset(spatial_info->gyroscope, 0, sizeof(double) * spatial_info->gyro_axes);

  return Qnil;
}

VALUE spatial_reset_compass_correction(VALUE self) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  ensure(CPhidgetSpatial_resetCompassCorrectionParameters((CPhidgetSpatialHandle) info->handle));
  memset(spatial_info->compass_correction, 0, sizeof(double) * COMPASS_CORRECTION_LENGTH );
  spatial_info->has_compass_correction = false; 

  return Qnil;
}

VALUE spatial_compass_correction_set(VALUE self, VALUE compass_correction) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  if ( (TYPE(compass_correction) != T_ARRAY) || (RARRAY_LEN(compass_correction) != COMPASS_CORRECTION_LENGTH) ) {
    rb_raise(rb_eTypeError, MSG_COMPASS_CORRECTION_NOT_ARRAY);
    return Qnil;
  } else {
    for (int i=0; i<RARRAY_LEN(compass_correction); i++) {
      VALUE cc_element = rb_ary_entry(compass_correction, i);

      if ( TYPE(cc_element) == T_FLOAT)
        spatial_info->compass_correction[i] = NUM2DBL(cc_element);
      else {
        rb_raise(rb_eTypeError, MSG_COMPASS_CORRECTION_MUST_BE_FLOAT);
        return Qnil;
      }
    }

    spatial_info->has_compass_correction = true; 
    if (info->is_attached)
      spatial_set_compass_correction_by_array( 
        (CPhidgetSpatialHandle)info->handle, spatial_info->compass_correction);
  }

  return compass_correction;
}

VALUE spatial_compass_correction_get(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->has_compass_correction) ? 
    double_array_to_rb(spatial_info->compass_correction, COMPASS_CORRECTION_LENGTH) : Qnil;
}

VALUE spatial_data_rate_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->data_rate_max) ? INT2FIX(spatial_info->data_rate_max) : Qnil;
}  

VALUE spatial_data_rate_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->data_rate_min) ? INT2FIX(spatial_info->data_rate_min) : Qnil;
}  

VALUE spatial_data_rate_set(VALUE self, VALUE data_rate) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  if ( TYPE(data_rate) != T_FIXNUM ) {
    rb_raise(rb_eTypeError, MSG_DATA_RATE_MUST_BE_NUM);
    return Qnil;
  } else {
    spatial_info->data_rate = FIX2INT(data_rate);

    if (info->is_attached)
      ensure(CPhidgetSpatial_setDataRate((CPhidgetSpatialHandle)info->handle, 
          spatial_info->data_rate));
  }

  return data_rate;
}

VALUE spatial_data_rate_get(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return INT2FIX(spatial_info->data_rate);
}
