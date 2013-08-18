#include "phidgets_native.h"

void spatial_on_free(void *type_info) {
  SpatialInfo *spatial_info = type_info;
  if (spatial_info->acceleration)
    xfree(spatial_info->acceleration);
  if (spatial_info->acceleration_min)
    xfree(spatial_info->acceleration_min);
  if (spatial_info->acceleration_max)
    xfree(spatial_info->acceleration_max);
  if (spatial_info->gyroscope)
    xfree(spatial_info->gyroscope);
  if (spatial_info->gyroscope_min)
    xfree(spatial_info->gyroscope_min);
  if (spatial_info->gyroscope_max)
    xfree(spatial_info->gyroscope_max);
  if (spatial_info->compass)
    xfree(spatial_info->compass);
  if (spatial_info->compass_min)
    xfree(spatial_info->compass_min);
  if (spatial_info->compass_max)
    xfree(spatial_info->compass_max);
  if (spatial_info)
    xfree(spatial_info);

  return;
}

int spatial_set_compass_correction_by_array(CPhidgetSpatialHandle phid, double *cc) {
  return report(CPhidgetSpatial_setCompassCorrectionParameters( phid,
    cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7], cc[8], cc[9], 
    cc[10], cc[11], cc[12] ));
}

int CCONV spatial_on_attach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  SpatialInfo *spatial_info = info->type_info;

  // Accelerometer Attributes:
  report(CPhidgetSpatial_getAccelerationAxisCount((CPhidgetSpatialHandle)phid, &spatial_info->accelerometer_axes));
  report(CPhidgetSpatial_getGyroAxisCount((CPhidgetSpatialHandle)phid, &spatial_info->gyro_axes));
  report(CPhidgetSpatial_getCompassAxisCount((CPhidgetSpatialHandle)phid, &spatial_info->compass_axes));
  report(CPhidgetSpatial_getDataRateMax((CPhidgetSpatialHandle)phid, &spatial_info->data_rate_max));
  report(CPhidgetSpatial_getDataRateMin((CPhidgetSpatialHandle)phid, &spatial_info->data_rate_min));

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
    report(CPhidgetSpatial_getAccelerationMin((CPhidgetSpatialHandle)phid, i, &spatial_info->acceleration_min[i]));
    report(CPhidgetSpatial_getAccelerationMax((CPhidgetSpatialHandle)phid, i, &spatial_info->acceleration_max[i]));
  }

  for(int i=0; i < spatial_info->compass_axes; i++) {
    report(CPhidgetSpatial_getMagneticFieldMin((CPhidgetSpatialHandle)phid, i, &spatial_info->compass_min[i]));
    report(CPhidgetSpatial_getMagneticFieldMax((CPhidgetSpatialHandle)phid, i, &spatial_info->compass_max[i]));
  }

  for(int i=0; i < spatial_info->gyro_axes; i++) {
    report(CPhidgetSpatial_getAngularRateMin((CPhidgetSpatialHandle)phid, i, &spatial_info->gyroscope_min[i]));
    report(CPhidgetSpatial_getAngularRateMax((CPhidgetSpatialHandle)phid, i, &spatial_info->gyroscope_max[i]));
  }

	// Set the data rate for the spatial events in milliseconds. 
  // Note that 1000/16 = 62.5 Hz
	report(CPhidgetSpatial_setDataRate((CPhidgetSpatialHandle)phid, spatial_info->data_rate));

  // Strictly speaking, this is entirely optional:
  if (spatial_info->is_compass_correction_known)
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
  spatial_info->is_acceleration_known = false;
  spatial_info->is_gyroscope_known = false;
  spatial_info->is_compass_known = false;

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

    spatial_info->is_acceleration_known = true;

    // Sometimes the compass will return nonesense in the form of this constant
    // I'm fairly certain that simply checking the first element of the array 
    // will suffice, and when this is the case, we just keep the prior values:
    if (data[i]->magneticField[0] == PUNK_DBL)
      spatial_info->is_compass_known = false;
    else {
      for(int j=0; j < spatial_info->compass_axes; j++)
        spatial_info->compass[j] = data[i]->magneticField[j];

      spatial_info->is_compass_known = true;
    } 

    // Gyros get handled slightly different:
    // NOTE: Other people may have a better way to do this, but this is the method
    // I grabbed from the phidget sample. Maybe I should report these in radians...
    double timestamp = data[i]->timestamp.seconds + data[i]->timestamp.microseconds/MICROSECONDS_IN_SECOND;

    if (spatial_info->last_microsecond > 0) {
      double timechange = timestamp - spatial_info->last_microsecond;

      for(int j=0; j < spatial_info->gyro_axes; j++)
        spatial_info->gyroscope[j] += data[i]->angularRate[j] * timechange;
    }

    spatial_info->is_gyroscope_known = true;

    // We'll need this on the next go around:
    spatial_info->last_microsecond = timestamp;
  }

  return 0;
}

