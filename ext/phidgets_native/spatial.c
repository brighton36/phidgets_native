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
          (data[i]->angularRate[0] * timechange)*360.0*M_PI*2.0,
          (data[i]->angularRate[1] * timechange)/360.0*M_PI*2.0,
          (data[i]->angularRate[2] * timechange)/360.0*M_PI*2.0,
          NULL);
    }

    spatial_info->is_gyroscope_known = true;

    // We'll need this on the next go around:
    spatial_info->last_microsecond = timestamp;

    // TODO: this only works for three-axis systems. Put a conditional in here?
    // TODO: should we translate these values into their respective vectors first?
    // Update the ahrs
    spatial_ahrs_update(spatial_info, 
        // TODO: Dry out the deg to rad conversions:
        (float) (data[i]->angularRate[0]*M_PI/180), 
        (float) (data[i]->angularRate[1]*M_PI/180),
        (float) (data[i]->angularRate[2]*M_PI/180),
      (float) data[i]->acceleration[0],  (float) data[i]->acceleration[1],  (float) data[i]->acceleration[2], 
      (float) data[i]->magneticField[0], (float) data[i]->magneticField[1], (float) data[i]->magneticField[2]);
  }

  return 0;
}

// NOTE: This was largely copied from Madgwick's Quaternion implementation of the 'DCM filter' 
// https://code.google.com/p/imumargalgorithm30042010sohm/

// AHRS.c
// S.O.H. Madgwick
// 25th August 2010

// Description:
//
// Quaternion implementation of the 'DCM filter' [Mayhony et al].  Incorporates the magnetic distortion
// compensation algorithms from my filter [Madgwick] which eliminates the need for a reference
// direction of flux (bx bz) to be predefined and limits the effect of magnetic distortions to yaw
// axis only.
//
// User must define 'SPATIAL_AHRS_HALFT' as the (sample period / 2), and the filter gains 'SPATIAL_AHRS_KP' and 'SPATIAL_AHRS_KI'.
//
// Global variables 'q0', 'q1', 'q2', 'q3' are the quaternion elements representing the estimated
// orientation.  See my report for an overview of the use of quaternions in this application.
//
// User must call 'AHRSupdate()' every sample period and parse calibrated gyroscope ('gx', 'gy', 'gz'),
// accelerometer ('ax', 'ay', 'ay') and magnetometer ('mx', 'my', 'mz') data.  Gyroscope units are
// radians/second, accelerometer and magnetometer units are irrelevant as the vector is normalised.
//
//=====================================================================================================

void spatial_ahrs_init(SpatialInfo *spatial_info) {
  // Scaled integral error
  spatial_info->exInt = 0.0;
  spatial_info->eyInt = 0.0; 
  spatial_info->ezInt = 0.0;	

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

  // TODO: figure out why data rate setting kills this to Nan-land
  // printf("Tick %lf, %lf, %lf, %lf\n", spatial_info->orientation_q[0], spatial_info->orientation_q[1], spatial_info->orientation_q[2], spatial_info->orientation_q[3]);

	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;

  // Just a shorthand meant to make this a bit easier to work with, and transact
  // commits (eventually)
  float q0 = spatial_info->orientation_q[0];
  float q1 = spatial_info->orientation_q[1];
  float q2 = spatial_info->orientation_q[2];
  float q3 = spatial_info->orientation_q[3];

	// auxiliary variables to reduce number of repeated operations
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;   
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;          
	
	// normalise the measurements
	norm = sqrt(ax*ax + ay*ay + az*az);       
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;
	norm = sqrt(mx*mx + my*my + mz*mz);          
	mx = mx / norm;
	my = my / norm;
	mz = mz / norm;         
	
	// compute reference direction of flux
	hx = 2*mx*(0.5 - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
	hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5 - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
	hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5 - q1q1 - q2q2);         
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz;        
	
	// estimated direction of gravity and flux (v and w)
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
	wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
	wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
	
	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) + (my*wz - mz*wy);
	ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
	ez = (ax*vy - ay*vx) + (mx*wy - my*wx);
	
	// integral error scaled integral gain
	spatial_info->exInt = spatial_info->exInt + ex*SPATIAL_AHRS_KI;
	spatial_info->eyInt = spatial_info->eyInt + ey*SPATIAL_AHRS_KI;
	spatial_info->ezInt = spatial_info->ezInt + ez*SPATIAL_AHRS_KI;
	
	// adjusted gyroscope measurements
	gx = gx + SPATIAL_AHRS_KP*ex + spatial_info->exInt;
	gy = gy + SPATIAL_AHRS_KP*ey + spatial_info->eyInt;
	gz = gz + SPATIAL_AHRS_KP*ez + spatial_info->ezInt;
	
	// integrate quaternion rate and normalise
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*SPATIAL_AHRS_HALFT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*SPATIAL_AHRS_HALFT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*SPATIAL_AHRS_HALFT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*SPATIAL_AHRS_HALFT;  
	
	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	spatial_info->orientation_q[0] = q0 / norm;
	spatial_info->orientation_q[1] = q1 / norm;
	spatial_info->orientation_q[2] = q2 / norm;
	spatial_info->orientation_q[3] = q3 / norm;
}
