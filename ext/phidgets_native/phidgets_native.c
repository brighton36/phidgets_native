#include "phidgets_native.h"

/*
 * Document-class: PhidgetsNative::NotFoundError
 *
 * This exception is raised when the library receives a EPHIDGET_NOTFOUNDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::NoMemoryError
 *
 * This exception is raised when the library receives a EPHIDGET_NOMEMORYERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::UnexpectedError
 *
 * This exception is raised when the library receives a EPHIDGET_UNEXPECTEDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::InvalidArgError
 *
 * This exception is raised when the library receives a EPHIDGET_INVALIDARGERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::NotAttachedError
 *
 * This exception is raised when the library receives a EPHIDGET_NOTATTACHEDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::InterruptedError
 *
 * This exception is raised when the library receives a EPHIDGET_INTERRUPTEDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::InvalidError
 *
 * This exception is raised when the library receives a EPHIDGET_INVALIDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::NetworkError
 *
 * This exception is raised when the library receives a EPHIDGET_NETWORKERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::UnknownValError
 *
 * This exception is raised when the library receives a EPHIDGET_UNKNOWNVALERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::BadPasswordError
 *
 * This exception is raised when the library receives a EPHIDGET_BADPASSWORDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::UnsupportedError
 *
 * This exception is raised when the library receives a EPHIDGET_UNSUPPORTEDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::DuplicateError
 *
 * This exception is raised when the library receives a EPHIDGET_DUPLICATEERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::TimeoutError
 *
 * This exception is raised when the library receives a EPHIDGET_TIMEOUTERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::OutOfBoundsError
 *
 * This exception is raised when the library receives a EPHIDGET_OUTOFBOUNDSERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::EventError
 *
 * This exception is raised when the library receives a EPHIDGET_EVENTERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::NetworkNotConnectedError
 *
 * This exception is raised when the library receives a EPHIDGET_NETWORKNOTCONNECTEDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::WrongDeviceError
 *
 * This exception is raised when the library receives a EPHIDGET_WRONGDEVICEERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::ClosedError
 *
 * This exception is raised when the library receives a EPHIDGET_CLOSEDERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::BadVersionError
 *
 * This exception is raised when the library receives a EPHIDGET_BADVERSIONERROR
 * error.
 */

/*
 * Document-class: PhidgetsNative::UnhandledError
 *
 * This exception is raised when the library receives a EPHIDGET_UNHANDLEDERROR
 * error.
 */

void Init_phidgets_native() {
  const char *phidget_library_version;

  ensure(CPhidget_getLibraryVersion(&phidget_library_version));	

  /*
   * Mostly a container module for all of our objects. Singleton methods provide
   * logging and enumeration features.
   */
  VALUE m_Phidget = rb_define_module("PhidgetsNative");

  // We need to require the time library for the gps code
  rb_funcall(m_Phidget, rb_intern("require"), 1, rb_str_new2("time"));

  /*
   * This constant is a string which reflects the version of the phidget library being used.
   */
  rb_define_const(m_Phidget, "LIBRARY_VERSION", rb_str_new2(phidget_library_version));

  /*
   * Document-method: enable_logging!
   * call-seq:
   *   enable_logging!(log_level, file_path = nil) -> nil
   *
   * This method will enable logging within the Phidget library. The log_level
   * parameter indicates the highest degree of desired output verbosity. Logged
   * data will be that which is less than or equal to this level. Currently, the
   * supported verbosity levels, in ascending order of verbosity are as follows:
   * * :critical
   * * :error
   * * :warning
   * * :debug
   * * :info
   * * :verbose
   * Be sure to specify a symbol, and not a string.
   *
   * The optional file_path parameter can be used to divert logging output to a
   * file, instead of the default behavior of logging to stdout. Be advised that
   * only absolute path names appear to be supported by the library at this time.
   */
  rb_define_singleton_method(m_Phidget, "enable_logging!", phidget_enable_logging, -1);

  /*
   * Document-method: log
   * call-seq:
   *   log(log_level, message) -> nil
   *
   * Logs an event of type log_level to the Phidget log, citing the provided message.
   * Supported log_levels are declared in the enable_logging! method.
   */
  rb_define_singleton_method(m_Phidget, "log", phidget_log, 2);

  /*
   * Document-method: disable_logging!
   * call-seq:
   *   disable_logging! -> nil
   *
   * This method will disable logging within the Phidget library, if logging was
   * previously enabled.
   */
  rb_define_singleton_method(m_Phidget, "disable_logging!", phidget_disable_logging, 0);

  /*
   * Document-method: all
   * call-seq:
   *  all -> nil
   *
   * This method will return an array of Phidget objects. These objects all the 
   * represent all the Phidgets which are currently connected to your computer.
   */
  rb_define_singleton_method(m_Phidget, "all", phidget_all, 0);
 

  // Phidget Library Exceptions : 
  VALUE c_PhidgetNotFound = rb_define_class_under(m_Phidget, "NotFoundError", rb_eStandardError);
  VALUE c_PhidgetNoMemory = rb_define_class_under(m_Phidget, "NoMemoryError", rb_eStandardError);
  VALUE c_PhidgetUnexpected = rb_define_class_under(m_Phidget, "UnexpectedError", rb_eStandardError);
  VALUE c_PhidgetInvalidArg = rb_define_class_under(m_Phidget, "InvalidArgError", rb_eStandardError);
  VALUE c_PhidgetNotAttached = rb_define_class_under(m_Phidget, "NotAttachedError", rb_eStandardError);
  VALUE c_PhidgetInterrupted = rb_define_class_under(m_Phidget, "InterruptedError", rb_eStandardError);
  VALUE c_PhidgetInvalid = rb_define_class_under(m_Phidget, "InvalidError", rb_eStandardError);
  VALUE c_PhidgetNetwork = rb_define_class_under(m_Phidget, "NetworkError", rb_eStandardError);
  VALUE c_PhidgetUnknownVal = rb_define_class_under(m_Phidget, "UnknownValError", rb_eStandardError);
  VALUE c_PhidgetBadPassword = rb_define_class_under(m_Phidget, "BadPasswordError", rb_eStandardError);
  VALUE c_PhidgetUnsupported = rb_define_class_under(m_Phidget, "UnsupportedError", rb_eStandardError);
  VALUE c_PhidgetDuplicate = rb_define_class_under(m_Phidget, "DuplicateError", rb_eStandardError);
  VALUE c_PhidgetTimeout = rb_define_class_under(m_Phidget, "TimeoutError", rb_eStandardError);
  VALUE c_PhidgetOutOfBounds = rb_define_class_under(m_Phidget, "OutOfBoundsError", rb_eStandardError);
  VALUE c_PhidgetEvent = rb_define_class_under(m_Phidget, "EventError", rb_eStandardError);
  VALUE c_PhidgetNetworkNotConnected = rb_define_class_under(m_Phidget, "NetworkNotConnectedError", rb_eStandardError);
  VALUE c_PhidgetWrongDevice = rb_define_class_under(m_Phidget, "WrongDeviceError", rb_eStandardError);
  VALUE c_PhidgetClosed = rb_define_class_under(m_Phidget, "ClosedError", rb_eStandardError);
  VALUE c_PhidgetBadVersion = rb_define_class_under(m_Phidget, "BadVersionError", rb_eStandardError);
  VALUE c_PhidgetUnhandled = rb_define_class_under(m_Phidget, "UnhandledError", rb_eStandardError);

  // Phidget Device Classes
  Init_phidgets_native_device(m_Phidget);

  Init_phidgets_native_accelerometer(m_Phidget);
  Init_phidgets_native_advancedservo(m_Phidget);
  Init_phidgets_native_analog(m_Phidget);
  Init_phidgets_native_bridge(m_Phidget);
  Init_phidgets_native_encoder(m_Phidget);
  Init_phidgets_native_frequencycounter(m_Phidget);
  Init_phidgets_native_gps(m_Phidget);
  Init_phidgets_native_interfacekit(m_Phidget);
  Init_phidgets_native_ir(m_Phidget);
  Init_phidgets_native_led(m_Phidget);
  Init_phidgets_native_motorcontrol(m_Phidget);
  Init_phidgets_native_phsensor(m_Phidget);
  Init_phidgets_native_rfid(m_Phidget);
  Init_phidgets_native_servo(m_Phidget);
  Init_phidgets_native_spatial(m_Phidget);
  Init_phidgets_native_stepper(m_Phidget);
  Init_phidgets_native_temperaturesensor(m_Phidget);
  Init_phidgets_native_textlcd(m_Phidget);
  Init_phidgets_native_textled(m_Phidget);
  Init_phidgets_native_weightsensor(m_Phidget);
}

