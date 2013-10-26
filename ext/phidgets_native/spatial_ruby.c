#include "phidgets_native.h"
#include <ctype.h>

const char MSG_COMPASS_CORRECTION_NOT_ARRAY[] = "compass correction must be a 13 element array";
const char MSG_COMPASS_CORRECTION_MUST_BE_FLOAT[] = "compass correction elements must be float or fixnum";
const char MSG_DATA_RATE_MUST_BE_NUM[] = "data rate must be fixnum";
const char MSG_AROUND_MUST_BE_FLOAT[] = "rotation angles must be fixnums";
const char MSG_IN_ORDER_MUST_BE_STRING[] = "rotation order must be a string";
const char MSG_INVALID_ROTATION_AXIS[] = "invalid rotation axis in rotation order, must be x, y, or z";

const char DEFAULT_IN_ORDER_STR[] = "xyz";
/*
 * Document-class: PhidgetsNative::Spatial < PhidgetsNative::Device
 *
 * This class provides functionality specific to the "Spatial" device class. 
 * Primarily, this includes reporting of acceleration, compass, and orientation 
 * vectors.
 */

void Init_phidgets_native_spatial(VALUE m_Phidget) {
  // We need this for the orientation code mostly:
  rb_funcall(rb_mKernel, rb_intern("require"), 1, rb_str_new2("matrix"));

  // And now attach methods to the object:
  VALUE c_Device = rb_const_get(m_Phidget, rb_intern("Device"));

  VALUE c_Spatial = rb_define_class_under(m_Phidget,"Spatial",c_Device);

  /*
   * Document-method: new
   * call-seq:
   *   new(serial_number)
   *
   * All phidget objects are created from the device serial number. Serial numbers
   * are required to be Fixnums (aka "unsigned integers").
   */
  rb_define_method(c_Spatial, "initialize", spatial_initialize, 1);

  /*
   * Document-method: close
   * call-seq:
   *   close -> nil
   *
   * This method will unregister the phidget event handlers, and free up all
   * API resources associated with the phidget. This is an optional, but useful
   * way to remove the object's overhead before the GC kicks in and actually 
   * frees the resource.
   */
  rb_define_method(c_Spatial, "close", spatial_close, 0);

  /*
   * Document-method: sample_rate
   * call-seq:
   *   sample_rate -> FixNum
   *
   * For most Phidgets, an event handler processes the device state changes at
   * some regular interval. For these devices, this method will return the rate
   * of state changes measured in Hz.
   */
  rb_define_method(c_Spatial, "sample_rate", spatial_sample_rate, 0);

  /*
   * Document-method: accelerometer_axes
   * call-seq:
   *   accelerometer_axes -> Fixnum
   *
   * This method returns the number of axis reported by the accelerometer. This 
   * number comes from the 
   * CPhidgetSpatial_getAccelerationAxisCount[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "accelerometer_axes", spatial_accelerometer_axes, 0);

  /*
   * Document-method: compass_axes
   * call-seq:
   *   compass_axes -> Fixnum
   *
   * This method returns the number of axis reported by the compass. This 
   * number comes from the 
   * CPhidgetSpatial_getCompassAxisCount[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "compass_axes", spatial_compass_axes, 0);

  /*
   * Document-method: gyro_axes
   * call-seq:
   *   gyro_axes -> Fixnum
   *
   * This method returns the number of axis reported by the gyroscope. This 
   * number comes from the 
   * CPhidgetSpatial_getGyroAxisCount[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "gyro_axes", spatial_gyro_axes, 0);

  /*
   * Document-method: gyro_min
   * call-seq:
   *   gyro_min -> Array
   *
   * This method returns an array of Float(s) which represent the minimal value
   * that an axis will report during a sample interval. These values come from the
   * CPhidgetSpatial_getAngularRateMin[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "gyro_min", spatial_gyro_min, 0);

  /*
   * Document-method: gyro_max
   * call-seq:
   *   gyro_max -> Array
   *
   * This method returns an array of Float(s) which represent the maximal value
   * that an axis will report during a sample interval. These values come from the
   * CPhidgetSpatial_getAngularRateMax[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "gyro_max", spatial_gyro_max, 0);

  /*
   * Document-method: accelerometer_min
   * call-seq:
   *   accelerometer_min -> Array
   *
   * This method returns an array of Float(s) which represent the minimal value
   * that an axis will report during a sample interval. These values come from the
   * CPhidgetSpatial_getAccelerationMin[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "accelerometer_min", spatial_accelerometer_min, 0);

  /*
   * Document-method: accelerometer_max
   * call-seq:
   *   accelerometer_max -> Array
   *
   * This method returns an array of Float(s) which represent the maximal value
   * that an axis will report during a sample interval. These values come from the
   * CPhidgetSpatial_getAccelerationMax[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "accelerometer_max", spatial_accelerometer_max, 0);

  /*
   * Document-method: compass_min
   * call-seq:
   *   compass_min -> Array
   *
   * This method returns an array of Float(s) which represent the minimal value
   * that an axis will report during a sample interval. These values come from the
   * CPhidgetSpatial_getMagneticFieldMin[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "compass_min", spatial_compass_min, 0);

  /*
   * Document-method: compass_max
   * call-seq:
   *   compass_max -> Array
   *
   * This method returns an array of Float(s) which represent the maximal value
   * that an axis will report during a sample interval. These values come from the
   * CPhidgetSpatial_getMagneticFieldMax[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html] 
   * function.
   */
  rb_define_method(c_Spatial, "compass_max", spatial_compass_max, 0);

  /*
   * Document-method: gyro
   * call-seq:
   *   gyro -> Array
   *
   * This method returns an array of Float(s) which represent the normalized value 
   * of each axis on the gyro in degrees (0-359.9). These values are calculated 
   * by accumulating the delta measurements which are reported inside the
   * CPhidgetSpatial_set_OnSpatialData_Handler[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html]
   * event handler. 
   *
   * NOTE: There's probably better algorithms to calculate this value than the 
   * one being used by this library. The algorithm used was merely the one found 
   * in the phidget-provided examples. Feel free to submit your algorithm for 
   * inclusion in this library if you know of a better way to do this.
   */
  rb_define_method(c_Spatial, "gyro", spatial_gyro, 0);

  /*
   * Document-method: compass
   * call-seq:
   *   compass -> Array
   *
   * This method returns an array of Float(s) which represent the relative magnetic 
   * attraction of each axis. These values are merely those which were most 
   * recently reported via the 
   * CPhidgetSpatial_set_OnSpatialData_Handler[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html]
   * event handler.
   *
   * NOTE: Sometimes the phidget library won't have values to report on this
   * measurement due to "EPHIDGET_UNKNOWNVAL" errors. When we come up against 
   * this case, we return the last values that were reported successfully.
   *
   * NOTE: You probably want this value in degrees. This is difficult, primarily
   * due to the pre-requisite of knowing what the cross-product is of the ground
   * plane (aka, the ground's surface normal). I might add a 'bad estimate'
   * of this vector in a future version of the library. E-mail the author if you 
   * need some sample code based on the accelerometer's inverse vector.
   */
  rb_define_method(c_Spatial, "compass", spatial_compass, 0);

  /*
   * Document-method: accelerometer
   * call-seq:
   *   accelerometer -> Array
   *
   * This method returns an array of Float(s) which represent the accelerometer 
   * magnitude of each axis, in meters per second. These values are merely those
   * which were most recently reported via the 
   * CPhidgetSpatial_set_OnSpatialData_Handler[http://www.phidgets.com/documentation/web/cdoc/group__phidspatial.html]
   * event handler.
   */
  rb_define_method(c_Spatial, "accelerometer", spatial_accelerometer, 0);

  /*
   * Document-method: zero_gyro!
   * call-seq:
   *   zero_gyro! -> nil
   *
   * Zero's the gyro values, and reference point. Once this method is executed
   * it takes approximately two seconds for the gyro to zero, and start returning
   * its offsets. This method zero's out the current gyro state vector and calls the 
   * CPhidgetSpatial_zeroGyro[http://www.phidgets.com/documentation/web/cdoc/group__phidcommon.html] 
   * function.
   */
  rb_define_method(c_Spatial, "zero_gyro!", spatial_zero_gyro, 0);

  /*
   * Document-method: reset_compass_correction!
   * call-seq:
   *   reset_compass_correction! -> nil
   *
   * Zero's the compass correction parameters. This method calls the 
   * CPhidgetSpatial_resetCompassCorrectionParameters[http://www.phidgets.com/documentation/web/cdoc/group__phidcommon.html] 
   * function.
   */
  rb_define_method(c_Spatial, "reset_compass_correction!", spatial_reset_compass_correction, 0);

  /*
   * Document-method: compass_correction=
   * call-seq:
   *   compass_correction=( Array correction_parameters ) -> Array
   *
   * This method expects a thirteen digit array of Floats, which are passed to the 
   * CPhidgetSpatial_setCompassCorrectionParameters[http://www.phidgets.com/documentation/web/cdoc/group__phidcommon.html] 
   * function. It returns the provided array.
   */
  rb_define_method(c_Spatial, "compass_correction=", spatial_compass_correction_set, 1);

  /*
   * Document-method: compass_correction
   * call-seq:
   *   compass_correction -> Array
   *
   * This method returns the current compass_correction parameters, which were
   * previously supplied to the compass_correction= method. If no such corrections
   * were supplied, the return is nil.
   */
  rb_define_method(c_Spatial, "compass_correction", spatial_compass_correction_get, 0);

  /*
   * Document-method: data_rate_min
   * call-seq:
   *   data_rate_min -> Fixnum
   *
   * This method returns a Fixnum which represents the minimaly supported data rate
   * that is supported by the library. This value comes direct from the
   * CPhidgetSpatial_getDataRateMin[http://www.phidgets.com/documentation/web/cdoc/group__phidcommon.html] 
   * function.
   *
   * NOTE: Keep in mind that the higher the value, the "lower" the rate. 
   */
  rb_define_method(c_Spatial, "data_rate_min", spatial_data_rate_min, 0);

  /*
   * Document-method: data_rate_max
   * call-seq:
   *   data_rate_max -> Fixnum
   *
   * This method returns a Fixnum which represents the maximum supported data rate
   * that is supported by the library. This value comes direct from the
   * CPhidgetSpatial_getDataRateMax[http://www.phidgets.com/documentation/web/cdoc/group__phidcommon.html] 
   * function.
   *
   * NOTE: Keep in mind that the lower the value, the "higher" the rate. 
   */
  rb_define_method(c_Spatial, "data_rate_max", spatial_data_rate_max, 0);

  /*
   * Document-method: data_rate=
   * call-seq:
   *   data_rate=( FixNum rate_in_ms ) -> FixNum
   *
   * This method expects a FixNum, which is passed to the 
   * CPhidgetSpatial_setDataRate[http://www.phidgets.com/documentation/web/cdoc/group__phidcommon.html] 
   * function. This value is the number of milliseconds between device sampling
   * reports, and it defaults to 16.
   * 
   * NOTE: For the case of 16, the data_rate as would be measured in Hz is 62.5: 1000/16 = 62.5 Hz
   */
  rb_define_method(c_Spatial, "data_rate=", spatial_data_rate_set, 1);

  /*
   * Document-method: data_rate
   * call-seq:
   *   data_rate -> FixNum
   *
   * This method returns the current data_rate as was previously supplied to the 
   * data_rate= method. If no such rate was supplied, then this method returns
   * the default rate, which is 16.
   */
  rb_define_method(c_Spatial, "data_rate", spatial_data_rate_get, 0);
  
  /*
   * call-seq:
   *   direction_cosine_matrix( FixNum around_x, FixNum around_y, FixNum around_z, String in_order = 'xyz' ) -> Matrix
   * This private method is used internally to return a single direction cosine 
   * Matrix that represents the combined translations provided around the x,y, 
   * and z axis. The optional in_order will translate around the axis in the 
   * order provided.
   */
  rb_define_private_method(c_Spatial, "direction_cosine_matrix", spatial_direction_cosine_matrix, -1);
}

VALUE spatial_initialize(VALUE self, VALUE serial) {
  PhidgetInfo *info = device_info(self);

  SpatialInfo *spatial_info = ALLOC(SpatialInfo); 
  memset(spatial_info, 0, sizeof(SpatialInfo));

  spatial_info->data_rate = DEFAULT_SPATIAL_DATA_RATE;
  spatial_info->sample_rate = sample_create();

  // Setup a spatial handle
  CPhidgetSpatialHandle spatial = 0;
  ensure(CPhidgetSpatial_create(&spatial));
	ensure(CPhidgetSpatial_set_OnSpatialData_Handler(spatial, spatial_on_data, info));

  info->handle = (CPhidgetHandle)spatial;
  info->on_type_attach = spatial_on_attach;
  info->on_type_detach = spatial_on_detach;
  info->on_type_free = spatial_on_free;
  info->type_info = spatial_info;

  return rb_call_super(1, &serial);
}

VALUE spatial_close(VALUE self) {
  PhidgetInfo *info = device_info(self);

  ensure(CPhidgetSpatial_set_OnSpatialData_Handler((CPhidgetSpatialHandle)info->handle, NULL, NULL));

  return rb_call_super(0,NULL);
}

VALUE spatial_sample_rate(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return INT2FIX(spatial_info->sample_rate->in_hz);
}  

VALUE spatial_accelerometer_axes(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->accelerometer_axes) ? 
    INT2FIX(spatial_info->accelerometer_axes) : Qnil;
}  

VALUE spatial_compass_axes(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->compass_axes) ? INT2FIX(spatial_info->compass_axes) : Qnil;
}  

VALUE spatial_gyro_axes(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->gyro_axes) ? INT2FIX(spatial_info->gyro_axes) : Qnil;
}  

VALUE spatial_accelerometer(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->is_acceleration_known) ? 
    double_array_to_rb(spatial_info->acceleration, spatial_info->accelerometer_axes) :
    Qnil;
}  

VALUE spatial_accelerometer_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->acceleration_min, spatial_info->accelerometer_axes);
}  

VALUE spatial_accelerometer_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->acceleration_max, spatial_info->accelerometer_axes);
}  

VALUE spatial_gyro(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  if (!spatial_info->is_gyroscope_known) return Qnil;

  double *gyroscope_in_degrees;
  VALUE ret; 

  gyroscope_in_degrees = ALLOC_N(double, spatial_info->gyro_axes); 
  for(int i=0; i < spatial_info->gyro_axes; i++)
    gyroscope_in_degrees[i] = fmod(spatial_info->gyroscope[i], DEGREES_IN_CIRCLE); 

  ret = double_array_to_rb(gyroscope_in_degrees, spatial_info->gyro_axes);
  xfree(gyroscope_in_degrees);

  return ret;
}  

VALUE spatial_gyro_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->gyroscope_min, spatial_info->gyro_axes);
}  

VALUE spatial_gyro_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->gyroscope_max, spatial_info->gyro_axes);
}  

VALUE spatial_compass(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->is_compass_known) ? 
    double_array_to_rb(spatial_info->compass, spatial_info->compass_axes) :
    Qnil;
}  

VALUE spatial_compass_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->compass_min, spatial_info->compass_axes);
}  

VALUE spatial_compass_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return double_array_to_rb(spatial_info->compass_max, spatial_info->compass_axes);
}  

VALUE spatial_zero_gyro(VALUE self) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  ensure(CPhidgetSpatial_zeroGyro((CPhidgetSpatialHandle) info->handle));
  memset(spatial_info->gyroscope, 0, sizeof(double) * spatial_info->gyro_axes);

  return Qnil;
}

VALUE spatial_reset_compass_correction(VALUE self) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  ensure(CPhidgetSpatial_resetCompassCorrectionParameters((CPhidgetSpatialHandle) info->handle));
  memset(spatial_info->compass_correction, 0, sizeof(double) * COMPASS_CORRECTION_LENGTH );
  spatial_info->is_compass_correction_known = false; 

  return Qnil;
}

VALUE spatial_compass_correction_set(VALUE self, VALUE compass_correction) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  if ( (TYPE(compass_correction) != T_ARRAY) || (RARRAY_LEN(compass_correction) != COMPASS_CORRECTION_LENGTH) ) {
    rb_raise(rb_eTypeError, MSG_COMPASS_CORRECTION_NOT_ARRAY);
    return Qnil;
  } else {
    for (int i=0; i<RARRAY_LEN(compass_correction); i++) {
      VALUE cc_element = rb_ary_entry(compass_correction, i);

      if ( TYPE(cc_element) == T_FLOAT)
        spatial_info->compass_correction[i] = NUM2DBL(cc_element);
      else {
        rb_raise(rb_eTypeError, MSG_COMPASS_CORRECTION_MUST_BE_FLOAT);
        return Qnil;
      }
    }

    spatial_info->is_compass_correction_known = true; 
    if (info->is_attached)
      spatial_set_compass_correction_by_array( 
        (CPhidgetSpatialHandle)info->handle, spatial_info->compass_correction);
  }

  return compass_correction;
}

VALUE spatial_compass_correction_get(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->is_compass_correction_known) ? 
    double_array_to_rb(spatial_info->compass_correction, COMPASS_CORRECTION_LENGTH) : Qnil;
}

VALUE spatial_data_rate_max(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->data_rate_max) ? INT2FIX(spatial_info->data_rate_max) : Qnil;
}  

VALUE spatial_data_rate_min(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return (spatial_info->data_rate_min) ? INT2FIX(spatial_info->data_rate_min) : Qnil;
}  

VALUE spatial_data_rate_set(VALUE self, VALUE data_rate) {
  PhidgetInfo *info = device_info(self);
  SpatialInfo *spatial_info = info->type_info;

  if ( TYPE(data_rate) != T_FIXNUM ) {
    rb_raise(rb_eTypeError, MSG_DATA_RATE_MUST_BE_NUM);
    return Qnil;
  } else {
    spatial_info->data_rate = FIX2INT(data_rate);

    if (info->is_attached)
      ensure(CPhidgetSpatial_setDataRate((CPhidgetSpatialHandle)info->handle, 
          spatial_info->data_rate));
  }

  return data_rate;
}

VALUE spatial_data_rate_get(VALUE self) {
  SpatialInfo *spatial_info = device_type_info(self);

  return INT2FIX(spatial_info->data_rate);
}

/*
def direction_cosine_matrix(around_x, around_y, around_z, in_order = 'XYZ')
  rotations = {
    # X-rotation:
    :x => Matrix.rows([
      [1.0, 0, 0],
      [0, Math.cos(around_x), Math.sin(around_x) * (-1.0)],
      [0, Math.sin(around_x), Math.cos(around_x)]
    ]), 

    # Y-rotation:
    :y => Matrix.rows([
      [Math.cos(around_y), 0, Math.sin(around_y)],
      [0, 1.0, 0],
      [Math.sin(around_y) * (-1.0), 0, Math.cos(around_y)]
    ]), 

    # Z-rotation:
    :z => Matrix.rows([ 
      [Math.cos(around_z), Math.sin(around_z) * (-1.0), 0],
      [Math.sin(around_z), Math.cos(around_z), 0],
      [0, 0, 1.0]
    ]) 
  }

  in_order.chars.collect{ |axis| rotations[axis.downcase.to_sym] }.reduce(:*)
end 
*/

VALUE spatial_direction_cosine_matrix(int argc, VALUE *argv, VALUE self) {
  VALUE around_x;
  VALUE around_y; 
  VALUE around_z; 
  VALUE in_order;

  const char *in_order_str = NULL;
  double dbl_around_x;
  double dbl_around_y;
  double dbl_around_z;

  rb_scan_args(argc, argv, "31", &around_x, &around_y, &around_z, &in_order);

  // TODO: Test the optional in_order
  if (TYPE(in_order) == T_STRING)
    in_order_str = StringValueCStr(in_order);
  else if( TYPE(in_order) != T_NIL)
    rb_raise(rb_eTypeError, MSG_IN_ORDER_MUST_BE_STRING);

  // Validate the inputs:
  if (TYPE(around_x) == T_FLOAT)
    dbl_around_x = NUM2DBL(around_x);
  else
    rb_raise(rb_eTypeError, MSG_AROUND_MUST_BE_FLOAT);

  if (TYPE(around_y) == T_FLOAT)
    dbl_around_y = NUM2DBL(around_y);
  else
    rb_raise(rb_eTypeError, MSG_AROUND_MUST_BE_FLOAT);

  if (TYPE(around_z) == T_FLOAT)
    dbl_around_z = NUM2DBL(around_z);
  else
    rb_raise(rb_eTypeError, MSG_AROUND_MUST_BE_FLOAT);

  // Perform the actual rotation:
  double mA[3][3];
  double mB[3][3];
  double mRet[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}}; 

  unsigned long in_order_length = strlen((in_order_str) ? in_order_str : DEFAULT_IN_ORDER_STR); 

  for (unsigned long i = 0; i < in_order_length; i++) {
    switch (tolower((in_order_str) ? in_order_str[i] : DEFAULT_IN_ORDER_STR[i])) {
      case 'x':
        mB[0][0] = 1.0; mB[0][1] = 0.0;               mB[0][2] = 0.0;
        mB[1][0] = 0.0; mB[1][1] = cos(dbl_around_x); mB[1][2] = sin(dbl_around_x) * (-1.0);
        mB[2][0] = 0.0; mB[2][1] = sin(dbl_around_x); mB[2][2] = cos(dbl_around_x);
        break;
      case 'y':
        mB[0][0] = cos(dbl_around_y);          mB[0][1] = 0.0; mB[0][2] = sin(dbl_around_y);
        mB[1][0] = 0.0;                        mB[1][1] = 1.0; mB[1][2] = 0.0;
        mB[2][0] = sin(dbl_around_y) * (-1.0); mB[2][1] = 0.0; mB[2][2] = cos(dbl_around_y);
        break;
      case 'z':
        mB[0][0] = cos(dbl_around_z); mB[0][1] = sin(dbl_around_z) * (-1.0); mB[0][2] = 0.0;
        mB[1][0] = sin(dbl_around_z); mB[1][1] = cos(dbl_around_z);          mB[1][2] = 0.0;
        mB[2][0] = 0.0;               mB[2][1] = 0.0;                        mB[2][2] = 1.0;
        break;
      default:
        rb_raise(rb_eTypeError, MSG_INVALID_ROTATION_AXIS);
        break;
    }

    memcpy(&mA, &mRet, sizeof(mRet));

    // Accumulate via multiplication
    mRet[0][0] = mA[0][0]*mB[0][0] + mA[1][0]*mB[0][1] + mA[2][0]*mB[0][2];
    mRet[0][1] = mA[0][0]*mB[1][0] + mA[1][0]*mB[1][1] + mA[2][0]*mB[1][2];
    mRet[0][2] = mA[0][0]*mB[2][0] + mA[1][0]*mB[2][1] + mA[2][0]*mB[2][2];

    mRet[1][0] = mA[0][1]*mB[0][0] + mA[1][1]*mB[0][1] + mA[2][1]*mB[0][2];
    mRet[1][1] = mA[0][1]*mB[1][0] + mA[1][1]*mB[1][1] + mA[2][1]*mB[1][2];
    mRet[1][2] = mA[0][1]*mB[2][0] + mA[1][1]*mB[2][1] + mA[2][1]*mB[2][2];

    mRet[2][0] = mA[0][2]*mB[0][0] + mA[1][2]*mB[0][1] + mA[2][2]*mB[0][2];
    mRet[2][1] = mA[0][2]*mB[1][0] + mA[1][2]*mB[1][1] + mA[2][2]*mB[1][2];
    mRet[2][2] = mA[0][2]*mB[2][0] + mA[1][2]*mB[2][1] + mA[2][2]*mB[2][2];
  }

  // TODO: for loop
  VALUE aryRow1 = rb_ary_new2(3);
  VALUE aryRow2 = rb_ary_new2(3);
  VALUE aryRow3 = rb_ary_new2(3);

  rb_ary_store(aryRow1, 0, rb_float_new(mRet[0][0]));
  rb_ary_store(aryRow1, 1, rb_float_new(mRet[0][1]));
  rb_ary_store(aryRow1, 2, rb_float_new(mRet[0][2]));

  rb_ary_store(aryRow2, 0, rb_float_new(mRet[1][0]));
  rb_ary_store(aryRow2, 1, rb_float_new(mRet[1][1]));
  rb_ary_store(aryRow2, 2, rb_float_new(mRet[1][2]));

  rb_ary_store(aryRow3, 0, rb_float_new(mRet[2][0]));
  rb_ary_store(aryRow3, 1, rb_float_new(mRet[2][1]));
  rb_ary_store(aryRow3, 2, rb_float_new(mRet[2][2]));

  VALUE aryRet = rb_ary_new2(3);
  rb_ary_store(aryRet, 0, aryRow1);
  rb_ary_store(aryRet, 1, aryRow2);
  rb_ary_store(aryRet, 2, aryRow3);

  VALUE c_Matrix = rb_const_get( rb_cObject, rb_intern("Matrix") );
  VALUE rbMatrixRet = rb_funcall(c_Matrix, rb_intern("rows"), 1, aryRet);     
  
  return rbMatrixRet;  
}
