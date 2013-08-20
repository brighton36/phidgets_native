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

const char MSG_INVALID_LOG_LEVEL[] = "unrecognized log_level, must be one of :critical, :error, :warning, :debug, :info, or :verbose";
const char MSG_FILE_PATH_MUST_BE_STRING[] = "file_path must be string, or nil";
const char MSG_INVALID_MESSAGE_STRING[] = "message must be a string";
const char MSG_UNSUPPORTED_DEVICE_ENUMERATED[] = "enumerated an unsupported phidget device";

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


CPhidgetLog_level sym_to_log_level(VALUE log_sym) {
  const char *log_level_str;

  if (TYPE(log_sym) != T_SYMBOL)
    rb_raise(rb_eTypeError, MSG_INVALID_LOG_LEVEL);

  log_level_str = rb_id2name(SYM2ID(log_sym));

  if (strcmp("critical", log_level_str) == 0)
    return PHIDGET_LOG_CRITICAL;
  else if (strcmp("error", log_level_str) == 0)
    return PHIDGET_LOG_ERROR;
  else if (strcmp("warning", log_level_str) == 0)
    return PHIDGET_LOG_WARNING;
  else if (strcmp("debug", log_level_str) == 0)
    return PHIDGET_LOG_DEBUG;
  else if (strcmp("info", log_level_str) == 0)
    return PHIDGET_LOG_INFO;
  else if (strcmp("verbose", log_level_str) == 0)
    return PHIDGET_LOG_VERBOSE;
  
  rb_raise(rb_eTypeError, MSG_INVALID_LOG_LEVEL);

  return 0;
}

VALUE phidget_enable_logging(int argc, VALUE *argv, VALUE class) {
  VALUE log_level;
  VALUE file_path;

  CPhidgetLog_level phidget_log_level; 
  const char *file_path_str;

  rb_scan_args(argc, argv, "11", &log_level, &file_path);

  phidget_log_level = sym_to_log_level(log_level);

  if (TYPE(file_path) == T_STRING)
    file_path_str = StringValueCStr(file_path);
  else if( TYPE(file_path) == T_NIL)
    file_path_str = NULL;
  else
    rb_raise(rb_eTypeError, MSG_FILE_PATH_MUST_BE_STRING);

  ensure(CPhidget_enableLogging(phidget_log_level,file_path_str));

  return Qnil;
}

VALUE phidget_disable_logging(VALUE class) {
  ensure(CPhidget_disableLogging());

  return Qnil;
}

VALUE phidget_log(VALUE class, VALUE log_level, VALUE message) {
  CPhidgetLog_level phidget_log_level; 

  if (TYPE(message) != T_STRING) rb_raise(rb_eTypeError, MSG_INVALID_MESSAGE_STRING);

  phidget_log_level = sym_to_log_level(log_level);

  // I really don't know what that second parameter does. It doesn't seem too useful.
  ensure(CPhidget_log(phidget_log_level, "N/A", "%s", StringValueCStr(message)));

  return Qnil;
}

VALUE phidget_all(VALUE class) {
  int num_devices = 0;
  int *serial_number;
  const char **device_type;
  CPhidget_DeviceClass *device_class; 

  CPhidgetHandle *handles;
	CPhidgetManagerHandle man_handle = 0;

  // First we enumerate all the devices and note the serial and type:
	ensure(CPhidgetManager_create(&man_handle));
	ensure(CPhidgetManager_open(man_handle));
  
  // I'm not sure that this is the "right" amount of time, but it seems to do
  // well enough:
  usleep(500000);

  ensure(CPhidgetManager_getAttachedDevices(man_handle, &handles, &num_devices));

  serial_number = ALLOC_N(int, num_devices);
  device_type = ALLOC_N(const char*, num_devices);
  device_class = ALLOC_N(CPhidget_DeviceClass, num_devices);

  for( int i=0; i<num_devices; i++) {
    ensure(CPhidget_getSerialNumber(handles[i], &serial_number[i]));
    ensure(CPhidget_getDeviceType(handles[i], &device_type[i]));
    ensure(CPhidget_getDeviceClass(handles[i], &device_class[i]));
  }

  // Then we free the Phidget library resources
  ensure(CPhidgetManager_freeAttachedDevicesArray(handles));
	ensure(CPhidgetManager_close(man_handle));
	ensure(CPhidgetManager_delete(man_handle));

  // So it *seems* that if we don't sleep here, that sometimes the wait_for_attachment
  // below will segfault. I'm guessing that there's a thread still running for a short
  // bit, after we call the delete on the manager, which conflicts with the device 
  // handle below
  usleep(500000);

  // And start constructing the ruby return value:
  VALUE devices = rb_ary_new2(num_devices);
  VALUE phidget_module = rb_const_get(rb_cObject, rb_intern("PhidgetsNative"));

  for(int i=0; i<num_devices; i++) {
    ID class_const;
    VALUE c_Exception;
    VALUE device_klass;
    VALUE device_instance;

    VALUE *args = ALLOC_N(VALUE, 1);
    args[0] = INT2FIX(serial_number[i]);

    switch (device_class[i]) {
      case PHIDCLASS_ACCELEROMETER:
        class_const = rb_intern("Accelerometer");
        break;
      case PHIDCLASS_ADVANCEDSERVO:
        class_const = rb_intern("AdvancedServo");
        break;
      case PHIDCLASS_ANALOG:
        class_const = rb_intern("Analog");
        break;
      case PHIDCLASS_BRIDGE:
        class_const = rb_intern("Bridge");
        break;
      case PHIDCLASS_ENCODER:
        class_const = rb_intern("Encoder");
        break;
      case PHIDCLASS_FREQUENCYCOUNTER:
        class_const = rb_intern("FrequencyCounter");
        break;
      case PHIDCLASS_GPS:
        class_const = rb_intern("GPS");
        break;
      case PHIDCLASS_INTERFACEKIT:
        class_const = rb_intern("InterfaceKit");
        break;
      case PHIDCLASS_IR:
        class_const = rb_intern("IR");
        break;
      case PHIDCLASS_LED:
        class_const = rb_intern("LED");
        break;
      case PHIDCLASS_MOTORCONTROL:
        class_const = rb_intern("MotorControl");
        break;
      case PHIDCLASS_PHSENSOR:
        class_const = rb_intern("PHSensor");
        break;
      case PHIDCLASS_RFID:
        class_const = rb_intern("RFID");
        break;
      case PHIDCLASS_SERVO:
        class_const = rb_intern("Servo");
        break;
      case PHIDCLASS_SPATIAL:
        class_const = rb_intern("Spatial");
        break;
      case PHIDCLASS_STEPPER:
        class_const = rb_intern("Stepper");
        break;
      case PHIDCLASS_TEMPERATURESENSOR:
        class_const = rb_intern("TemperatureSensor");
        break;
      case PHIDCLASS_TEXTLCD:
        class_const = rb_intern("TextLCD");
        break;
      case PHIDCLASS_TEXTLED:
        class_const = rb_intern("TextLED");
        break;
      case PHIDCLASS_WEIGHTSENSOR:
        class_const = rb_intern("WeightSensor");
        break;
      default:
        c_Exception = rb_const_get(phidget_module, rb_intern("UnsupportedError"));
        rb_raise(c_Exception, "%s \"%s\"", MSG_UNSUPPORTED_DEVICE_ENUMERATED, device_type[i]);
    }

    device_klass = rb_const_get(phidget_module, class_const);
    device_instance = rb_class_new_instance(1, args, device_klass);
    rb_funcall(device_instance, rb_intern("wait_for_attachment"), 1, INT2FIX(1000));
    rb_ary_store(devices, i, device_instance);

    xfree(args);
  }

  xfree(serial_number);
  xfree(device_type);
  xfree(device_class);

  return devices;
}
