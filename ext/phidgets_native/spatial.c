#include "phidgets_native.h"

const char DEFAULT_IN_ORDER_STR[] = "xyz";

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
  if (spatial_info->sample_rate)
    sample_free(spatial_info->sample_rate);
  if (spatial_info)
    xfree(spatial_info);

  return;
}

int spatial_set_compass_correction_by_array(CPhidgetSpatialHandle phid, double *cc) {
  return report(CPhidgetSpatial_setCompassCorrectionParameters( phid,
    cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7], cc[8], cc[9], 
    cc[10], cc[11], cc[12] ));
}

/*
 * This method will rotate the double[3][3] mRet around the axial rotations, in order of in_order )
 */
int euler_to_3x3dcm(double *mRet, double around_x, double around_y, double around_z, const char *in_order) {
  double mA[3][3];
  double mB[3][3];
  double mAccum[3][3]; 

  memcpy(&mAccum, mRet, sizeof(mAccum));

  unsigned long in_order_length = strlen((in_order) ? in_order : DEFAULT_IN_ORDER_STR); 

  for (unsigned long i = 0; i < in_order_length; i++) {
    switch (tolower((in_order) ? in_order[i] : DEFAULT_IN_ORDER_STR[i])) {
      case 'x':
        mB[0][0] = 1.0; mB[0][1] = 0.0;           mB[0][2] = 0.0;
        mB[1][0] = 0.0; mB[1][1] = cos(around_x); mB[1][2] = sin(around_x) * (-1.0);
        mB[2][0] = 0.0; mB[2][1] = sin(around_x); mB[2][2] = cos(around_x);
        break;
      case 'y':
        mB[0][0] = cos(around_y);          mB[0][1] = 0.0; mB[0][2] = sin(around_y);
        mB[1][0] = 0.0;                    mB[1][1] = 1.0; mB[1][2] = 0.0;
        mB[2][0] = sin(around_y) * (-1.0); mB[2][1] = 0.0; mB[2][2] = cos(around_y);
        break;
      case 'z':
        mB[0][0] = cos(around_z); mB[0][1] = sin(around_z) * (-1.0); mB[0][2] = 0.0;
        mB[1][0] = sin(around_z); mB[1][1] = cos(around_z);          mB[1][2] = 0.0;
        mB[2][0] = 0.0;           mB[2][1] = 0.0;                    mB[2][2] = 1.0;
        break;
      default:
        // Fatal Error - return without setting mRet
        return 1;
        break;
    }

    // Accumulate into ret, via multiplication
    memcpy(&mA, &mAccum, sizeof(mAccum));
    memset(&mAccum, 0, sizeof(mAccum));
 
    for (unsigned int i = 0; i < 3; i++)
      for (unsigned int j = 0; j < 3; j++)
        for (unsigned int k = 0; k < 3; k++)
          mAccum[i][j] += mA[i][k] * mB[k][j];
  }
  
  // Success:
  memcpy(mRet, &mAccum, sizeof(mAccum));

  return 0;
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

  // Initialize the gyro dcm:
  if (spatial_info->gyro_axes == 3) {
    double identity3x3[3][3] = {{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}}; 
    memcpy(&spatial_info->gyroscope_dcm, &identity3x3, sizeof(identity3x3));
  }

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
  memset(&spatial_info->gyroscope_dcm, 0, sizeof(spatial_info->gyroscope_dcm));

  spatial_info->last_microsecond = 0;
  spatial_info->is_acceleration_known = false;
  spatial_info->is_gyroscope_known = false;
  spatial_info->is_compass_known = false;

  sample_zero(spatial_info->sample_rate);

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
    sample_tick(spatial_info->sample_rate, &data[i]->timestamp);

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

      // TODO: Not sure that this is needed any more:
      for(int j=0; j < spatial_info->gyro_axes; j++)
        spatial_info->gyroscope[j] += data[i]->angularRate[j] * timechange;

      if (spatial_info->gyro_axes == 3)
        euler_to_3x3dcm((double *) &spatial_info->gyroscope_dcm, 
          (data[i]->angularRate[0] * timechange)/360.0*M_PI*2.0,
          (data[i]->angularRate[1] * timechange)/360.0*M_PI*2.0,
          (data[i]->angularRate[2] * timechange)/360.0*M_PI*2.0,
          NULL);
    }

    spatial_info->is_gyroscope_known = true;

    // We'll need this on the next go around:
    spatial_info->last_microsecond = timestamp;
  }

  return 0;
}

