#include "phidgets_native.h"

// This converts an array of doubles into a ruby array of numbers, or into
// nil for the case of an invalid dbl_array
VALUE double_array_to_rb(double *dbl_array, int length) {
  if (!dbl_array) return Qnil;

  VALUE rb_ary = rb_ary_new2(length);

  for(int i=0; i<length; i++) rb_ary_store(rb_ary, i, DBL2NUM(dbl_array[i]));

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

  VALUE m_Phidget = rb_const_get(rb_cObject, rb_intern("Phidgets"));  
  VALUE c_Exception = rb_const_get(m_Phidget, rb_intern(exception_name));

  rb_raise(c_Exception, "%s", description);

  return result;
}
