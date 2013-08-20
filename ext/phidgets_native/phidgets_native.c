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

