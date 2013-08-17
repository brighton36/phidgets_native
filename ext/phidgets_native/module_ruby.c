#include "phidgets_native.h"
#include <unistd.h>

const char MSG_INVALID_LOG_LEVEL[] = "unrecognized log_level, must be one of :critical, :error, :warning, :debug, :info, or :verbose";
const char MSG_FILE_PATH_MUST_BE_STRING[] = "file_path must be string, or nil";
const char MSG_INVALID_MESSAGE_STRING[] = "message must be a string";
const char MSG_UNSUPPORTED_DEVICE_ENUMERATED[] = "enumerated an unsupported phidget device";

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
