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

  // Ahrs Values:
  spatial_ahrs_init(spatial_info);

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

    // Gyro Time! these get handled slightly different...
    double timestamp = data[i]->timestamp.seconds + data[i]->timestamp.microseconds/MICROSECONDS_IN_SECOND;

    double timechange = (spatial_info->last_microsecond > 0) ? 
      timestamp - spatial_info->last_microsecond : 0;
    
    // This is kind of superceeded by the orientation_ functions, but I'm 
    // leaving it in for posterity:
    for(int j=0; j < spatial_info->gyro_axes; j++)
      spatial_info->gyroscope[j] += data[i]->angularRate[j] * timechange;

    spatial_info->is_gyroscope_known = true;

    // If we're dealing with 3 dimensional sensors, we can calculate orientation:
    if (spatial_info->gyro_axes == 3) {
      // This is a shortcut to keep things DRY below:
      double angular_rate_in_rad[3];
      for(int j=0; j < 3; j++)
        angular_rate_in_rad[j] = data[i]->angularRate[j] * M_PI/180;

      euler_to_3x3dcm((double *) &spatial_info->gyroscope_dcm, 
        angular_rate_in_rad[0] * timechange,
        angular_rate_in_rad[1] * timechange,
        angular_rate_in_rad[2] * timechange,
        NULL);

      // Madgwick Orientation time - Update the ahrs: 
      float gx = (float) angular_rate_in_rad[0];
      float gy = (float) angular_rate_in_rad[1];
      float gz = (float) angular_rate_in_rad[2];

      float ax = (float) data[i]->acceleration[0];
      float ay = (float) data[i]->acceleration[1];
      float az = (float) data[i]->acceleration[2];

      if (spatial_info->is_compass_known) 
        // We have enough info for a MARG update:
        spatial_ahrs_update(spatial_info, gx, gy, gz, ax, ay, az,
          (float) data[i]->magneticField[0], 
          (float) data[i]->magneticField[1], 
          (float) data[i]->magneticField[2]);
      else
        // We only have enough info for a IMU update:
        spatial_ahrs_update_imu(spatial_info, gx, gy, gz, ax, ay, az);
    }

    // We'll need this on the next go around:
    spatial_info->last_microsecond = timestamp;

  }

  return 0;
}

// NOTE: The spatial_ahrs_* functions were largely copied from Madgwick's 
// Quaternion implementation of the 'DCM filter' 
// https://code.google.com/p/imumargalgorithm30042010sohm/
void spatial_ahrs_init(SpatialInfo *spatial_info) {
  // Quaternion elements representing the estimated orientation
  spatial_info->orientation_q[0] = 1.0;
  spatial_info->orientation_q[1] = 0.0;
  spatial_info->orientation_q[2] = 0.0;
  spatial_info->orientation_q[3] = 0.0;

  return;
}

void spatial_ahrs_update(SpatialInfo *spatial_info, 
  float gx, float gy, float gz, 
  float ax, float ay, float az, 
  float mx, float my, float mz) {

  // Just a shorthand meant to make this a bit easier to work with, and transact
  // commits (eventually)
  float q0 = spatial_info->orientation_q[0];
  float q1 = spatial_info->orientation_q[1];
  float q2 = spatial_info->orientation_q[2];
  float q3 = spatial_info->orientation_q[3];

  float sample_rate_in_hz = 1000.0f / spatial_info->data_rate;

	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = inv_sqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Normalise magnetometer measurement
		recipNorm = inv_sqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * mx;
		_2q0my = 2.0f * q0 * my;
		_2q0mz = 2.0f * q0 * mz;
		_2q1mx = 2.0f * q1 * mx;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
		hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		recipNorm = inv_sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= SPATIAL_AHRS_BETA * s0;
		qDot2 -= SPATIAL_AHRS_BETA * s1;
		qDot3 -= SPATIAL_AHRS_BETA * s2;
		qDot4 -= SPATIAL_AHRS_BETA * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sample_rate_in_hz);
	q1 += qDot2 * (1.0f / sample_rate_in_hz);
	q2 += qDot3 * (1.0f / sample_rate_in_hz);
	q3 += qDot4 * (1.0f / sample_rate_in_hz);

	// Normalise quaternion
	recipNorm = inv_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;

	spatial_info->orientation_q[0] = q0;
	spatial_info->orientation_q[1] = q1;
	spatial_info->orientation_q[2] = q2;
	spatial_info->orientation_q[3] = q3;
}

void spatial_ahrs_update_imu(SpatialInfo *spatial_info, 
    float gx, float gy, float gz, 
    float ax, float ay, float az) {

  // Just a shorthand meant to make this a bit easier to work with, and transact
  // commits (eventually)
  float q0 = spatial_info->orientation_q[0];
  float q1 = spatial_info->orientation_q[1];
  float q2 = spatial_info->orientation_q[2];
  float q3 = spatial_info->orientation_q[3];

  float sample_rate_in_hz = 1000.0f / spatial_info->data_rate;

	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = inv_sqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
		recipNorm = inv_sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= SPATIAL_AHRS_BETA * s0;
		qDot2 -= SPATIAL_AHRS_BETA * s1;
		qDot3 -= SPATIAL_AHRS_BETA * s2;
		qDot4 -= SPATIAL_AHRS_BETA * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sample_rate_in_hz);
	q1 += qDot2 * (1.0f / sample_rate_in_hz);
	q2 += qDot3 * (1.0f / sample_rate_in_hz);
	q3 += qDot4 * (1.0f / sample_rate_in_hz);

	// Normalise quaternion
	recipNorm = inv_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;

	spatial_info->orientation_q[0] = q0;
	spatial_info->orientation_q[1] = q1;
	spatial_info->orientation_q[2] = q2;
	spatial_info->orientation_q[3] = q3;
}

/*
  The Madwick algorithm :
   * Uses +x to indicate North, we want +y which means we need a -90 degree rotation around z. 
   * Uses the following coordinate system: 
     http://stackoverflow.com/questions/17788043/madgwicks-sensor-fusion-algorithm-on-ios
*/
void spatial_madgeq_to_openglq(float *fMadgQ, float *fRetQ) {
  float fTmpQ[4];
  float fXYRotationQ[4] = { sqrt(0.5), 0, 0, -1.0*sqrt(0.5) }; // wxyz

  fTmpQ[0] = fMadgQ[0];
  fTmpQ[1] = fMadgQ[1] * -1.0f;
  fTmpQ[2] = fMadgQ[2];
  fTmpQ[3] = fMadgQ[3] * -1.0f;

  // And then store the Rotation into fTranslatedQ
  quat_mult((float *) &fTmpQ, (float *) &fXYRotationQ, fRetQ);
}
