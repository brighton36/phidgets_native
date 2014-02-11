#include "phidgets_native.h"

/* This converts an array of doubles into a ruby array of numbers, or into
 * nil for the case of an invalid dbl_array
 */
VALUE double_array_to_rb(double *dbl_array, int length) {
  if (!dbl_array) return Qnil;
  VALUE rb_ary = rb_ary_new2(length);
  for(int i=0; i<length; i++) rb_ary_store(rb_ary, i, DBL2NUM(dbl_array[i]));
  return rb_ary;
}

/* This converts an array of ints into a ruby array of fixnums, or into
 * nil for the case of an invalid int_array
 */
VALUE int_array_to_rb(int *int_array, int length) {
  if (!int_array) return Qnil;
  VALUE rb_ary = rb_ary_new2(length);
  for(int i=0; i<length; i++) rb_ary_store(rb_ary, i, INT2NUM(int_array[i]));
  return rb_ary;
}

/* This converts an array of ints into a ruby array of fixnums. If an element
 * of the array is 0, this converts to Qnil. nil is returned for the case of an 
 * invalid int_array.
 */
VALUE int_array_zeronils_to_rb(int *int_array, int length) {
  if (!int_array) return Qnil;
  VALUE rb_ary = rb_ary_new2(length);
  for(int i=0; i<length; i++) 
    rb_ary_store(rb_ary, i, (int_array[i] == 0) ? Qnil : INT2NUM(int_array[i]));
  return rb_ary;
}

/* This converts an array of phidget bools into a ruby array of booleans. If an 
 * element of the array is 0, this converts to Qnil. nil is returned for the case 
 * of an invalid bool_array.
 */
VALUE phidgetbool_array_to_rb(int *bool_array, int length) {
  if (!bool_array) return Qnil;
  VALUE rb_ary = rb_ary_new2(length);
  for(int i=0; i<length; i++) 
    rb_ary_store(rb_ary, i, (bool_array[i] == PTRUE) ? Qtrue : Qfalse);
  return rb_ary;
}

/* This converts a 3x3 array of doubles into a ruby matrix, or into
 * nil for the case of an invalid dbl_array
 */
VALUE double3x3_to_matrix_rb(double m3x3[][3]) {
  if (!m3x3) return Qnil;

  // To compose a Matrix, we need a 3x3 array of floats:
  VALUE aryRet = rb_ary_new2(3);
  for (unsigned int i=0; i < 3; i++) {
    VALUE arRow = rb_ary_new2(3);

    for (unsigned int j=0; j < 3; j++)
      rb_ary_store(arRow, j, rb_float_new(m3x3[i][j]));

    rb_ary_store(aryRet, i, arRow);
  }

  VALUE c_Matrix = rb_const_get( rb_cObject, rb_intern("Matrix") );
  VALUE rbMatrixRet = rb_funcall(c_Matrix, rb_intern("rows"), 2, aryRet, Qfalse);     
  
  return rbMatrixRet;  
}

// The name is ambiguous, but the main purpose here is dry things out a bit
// and let us do a better job of reporting errors to ruby
int ensure(int result) {
  if ( result == EPHIDGET_OK ) return 0;

  const char *exception_name;

  switch(result) {
    case EPHIDGET_NOTFOUND:
      exception_name = "NotFoundError";
      break;
    case EPHIDGET_NOMEMORY:
      exception_name = "NoMemoryError";
      break;
    case EPHIDGET_UNEXPECTED:
      exception_name = "UnexpectedError";
      break;
    case EPHIDGET_INVALIDARG:
      exception_name = "InvalidArgError";
      break;
    case EPHIDGET_NOTATTACHED:
      exception_name = "NotAttachedError";
      break;
    case EPHIDGET_INTERRUPTED:
      exception_name = "InterruptedError";
      break;
    case EPHIDGET_INVALID:
      exception_name = "InvalidError";
      break;
    case EPHIDGET_NETWORK:
      exception_name = "NetworkError";
      break;
    case EPHIDGET_UNKNOWNVAL:
      exception_name = "UnknownValError";
      break;
    case EPHIDGET_BADPASSWORD:
      exception_name = "BadPasswordError";
      break;
    case EPHIDGET_UNSUPPORTED:
      exception_name = "UnsupportedError";
      break;
    case EPHIDGET_DUPLICATE:
      exception_name = "DuplicateError";
      break;
    case EPHIDGET_TIMEOUT:
      exception_name = "TimeoutError";
      break;
    case EPHIDGET_OUTOFBOUNDS:
      exception_name = "OutOfBoundsError";
      break;
    case EPHIDGET_EVENT:
      exception_name = "EventError";
      break;
    case EPHIDGET_NETWORK_NOTCONNECTED:
      exception_name = "NetworkNotConnectedError";
      break;
    case EPHIDGET_WRONGDEVICE:
      exception_name = "WrongDeviceError";
      break;
    case EPHIDGET_CLOSED:
      exception_name = "ClosedError";
      break;
    case EPHIDGET_BADVERSION:
      exception_name = "BadVersionError";
      break;
    default:
      exception_name = "UnhandledError";
      break;
  }

  const char *description;
  CPhidget_getErrorDescription(result, &description);

  VALUE m_Phidget = rb_const_get(rb_cObject, rb_intern("PhidgetsNative"));  
  VALUE c_Exception = rb_const_get(m_Phidget, rb_intern(exception_name));

  rb_raise(c_Exception, "%s", description);

  return result;
}

// The name is ambiguous, but the main purpose here is dry things out a bit
// and let us do a better job of reporting errors albeit not halting.
// This is necessary in the threaded callbacks where access to the interpreter 
// is unavailable:
int report(int result) {
  if (result == EPHIDGET_OK) return result;

  const char *description;
  CPhidget_getErrorDescription(result, &description);

  CPhidget_log(PHIDGET_LOG_ERROR, "N/A", "PhidgetNative report error: %s", description);

  return result;
}

SampleRate *sample_create() {
  SampleRate *ret = ALLOC(SampleRate); 
  if (ret)
    sample_zero(ret);

  return ret;
}

int sample_zero(SampleRate *sample_rate) {
  memset(sample_rate, 0, sizeof(SampleRate));

  return 0;
}

int sample_free(SampleRate *sample_rate) {
  xfree(sample_rate);
  sample_rate = 0;
  return 0;
}

/*
 * The first time this is called, we're a little inadequate. Mostly this is because
 * we're not including microseconds. If we included microseconds, then we could
 * process a block if the seconds component were different than the last, and our
 * microseconds were greater than the starting microsecond amount.
 */
int sample_tick(SampleRate *sample_rate, CPhidget_Timestamp *ts) {
  unsigned long now_seconds;

  sample_rate->samples_in_second++;

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
  if (sample_rate->last_second == 0)
   sample_rate->last_second = now_seconds;
  else if (sample_rate->last_second != now_seconds ) {
   sample_rate->in_hz = (double)sample_rate->samples_in_second / 
        (double) (now_seconds -sample_rate->last_second);
   sample_rate->samples_in_second = 0;
   sample_rate->last_second = now_seconds;
  }

  return 0;
}

// There's a number of ways to implement this, at the moment, I'm more interested
// in accuracy... so we use the close-to-optimal method with low cost from :
//  http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root 
// as opposed to: 1.0f / sqrtf(x)
float inv_sqrt(float x) {
  unsigned int i = 0x5F1F1412 - (*(unsigned int*)&x >> 1);
  float tmp = *(float*)&i;
  return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
}


// Quaternion Multiplication operator. Expects its 4-element arrays in wxyz order 
void quat_mult(float a[4], float b[4], float ret[4]) {
  ret[0] = (b[0] * a[0]) - (b[1] * a[1]) - (b[2] * a[2]) - (b[3] * a[3]);
  ret[1] = (b[0] * a[1]) + (b[1] * a[0]) + (b[2] * a[3]) - (b[3] * a[2]);
  ret[2] = (b[0] * a[2]) + (b[2] * a[0]) + (b[3] * a[1]) - (b[1] * a[3]);
  ret[3] = (b[0] * a[3]) + (b[3] * a[0]) + (b[1] * a[2]) - (b[2] * a[1]);

  return;
}

// Quaternion Normalization operator. Expects its 4-element arrays in wxyz order 
void quat_norm(float a[4]) {
  float n = a[1]*a[1] + a[2]*a[2] + a[3]*a[3] + a[0]*a[0];

  if (n == 1.0f) 
    return ;

  n = inv_sqrt(n);

  a[1]*=n;
  a[2]*=n;
  a[3]*=n;
  a[0]*=n;

  return;
}

// Quaternion to direction cosine matrix. This implementation is a bit lazy, 
// but it works well enough.
void quat_to_dcm(float q[4], double dcm[][3]) {
  float fEuler[3];

  quat_to_euler(q, (float *)&fEuler);

  double tmpDcm[3][3] = {{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}};

  euler_to_3x3dcm((double *)&tmpDcm, 
      (double) fEuler[0], (double) fEuler[1], (double) fEuler[2], "xyz");

  memcpy(dcm, &tmpDcm, sizeof(tmpDcm));

  return;
}

// Quaternion to euler:
void quat_to_euler(float q[4], float e[3]) {
  float sqw = q[0]*q[0];
  float sqx = q[1]*q[1];
  float sqy = q[2]*q[2];
  float sqz = q[3]*q[3];
  e[2] = -1.0f * atan2f(2.f * (q[1]*q[2] + q[3]*q[0]), sqx - sqy - sqz + sqw);
  e[1] = asinf(-2.f * (q[1]*q[3] - q[2]*q[0]));
  e[0] = -1.0f * atan2f(2.f * (q[2]*q[3] + q[1]*q[0]), -sqx - sqy + sqz + sqw);

  return;
}

