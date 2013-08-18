#include "phidgets_native.h"

int CCONV interfacekit_on_attach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) phid;

  report(CPhidgetInterfaceKit_getInputCount(interfacekit, &interfacekit_info->digital_input_count));
  interfacekit_info->is_digital_input_count_known = true;

  report(CPhidgetInterfaceKit_getOutputCount(interfacekit, &interfacekit_info->digital_output_count));
  interfacekit_info->is_digital_output_count_known = true;

  report(CPhidgetInterfaceKit_getSensorCount(interfacekit, &interfacekit_info->analog_input_count));
  interfacekit_info->is_analog_input_count_known = true;

  // Allocate space for our state collections:
  interfacekit_info->digital_input_states = ALLOC_N(int, interfacekit_info->digital_input_count); 
  interfacekit_info->digital_output_states = ALLOC_N(int, interfacekit_info->digital_output_count); 
  interfacekit_info->analog_input_states = ALLOC_N(int, interfacekit_info->analog_input_count); 

  // Allocate our data_rate-related collections
  if (interfacekit_info->analog_input_count_prior != interfacekit_info->analog_input_count) {
    if(interfacekit_info->data_rates) xfree(interfacekit_info->data_rates);
    if(interfacekit_info->data_rates_max) xfree(interfacekit_info->data_rates_max);
    if(interfacekit_info->data_rates_min) xfree(interfacekit_info->data_rates_min);
    if(interfacekit_info->sensor_change_triggers) xfree(interfacekit_info->sensor_change_triggers);

    interfacekit_info->data_rates = ALLOC_N(int, interfacekit_info->analog_input_count);
    interfacekit_info->data_rates_max = ALLOC_N(int, interfacekit_info->analog_input_count);
    interfacekit_info->data_rates_min = ALLOC_N(int, interfacekit_info->analog_input_count);
    interfacekit_info->sensor_change_triggers = ALLOC_N(int, interfacekit_info->analog_input_count);

    for(int i=0;i<interfacekit_info->analog_input_count;i++) {
      interfacekit_info->data_rates[i] = DEFAULT_INTERFACEKIT_DATA_RATE;
      interfacekit_info->sensor_change_triggers[i] = DEFAULT_INTERFACEKIT_CHANGE_TRIGGER;
      
      report(CPhidgetInterfaceKit_getDataRateMax(interfacekit, i, &interfacekit_info->data_rates_max[i]));
      report(CPhidgetInterfaceKit_getDataRateMin(interfacekit, i, &interfacekit_info->data_rates_min[i]));
    }

    interfacekit_info->is_data_rates_known = true;
  }

  // TODO: Re-set digital output values here

  // If the interfacekit isn't what we expect, we need to perform a change:
  report(interfacekit_assert_ratiometric_state( info ));

  // Set the data-rate/sensor-thresholds
  for(int i=0;i<interfacekit_info->analog_input_count;i++)
    if (interfacekit_info->data_rates[i] > 0)
      report(CPhidgetInterfaceKit_setDataRate(interfacekit, i, interfacekit_info->data_rates[i]));
    else if (interfacekit_info->sensor_change_triggers[i] > 0)
      report(CPhidgetInterfaceKit_setSensorChangeTrigger(interfacekit, i, interfacekit_info->sensor_change_triggers[i]));

  // Read in all of our initial input values:
  for(int i=0; i<interfacekit_info->digital_input_count; i++)
    report(CPhidgetInterfaceKit_getInputState(interfacekit,i, &interfacekit_info->digital_input_states[i]));

  for(int i=0; i<interfacekit_info->digital_output_count; i++)
    report(CPhidgetInterfaceKit_getOutputState(interfacekit,i, &interfacekit_info->digital_output_states[i]));

  for(int i=0; i<interfacekit_info->analog_input_count; i++)
    if (interfacekit_info->rationmetric_changed_usec == 0) 
      // We only read these in if we haven't recently been changed ratiometric status: 
      report(CPhidgetInterfaceKit_getSensorValue(interfacekit,i, &interfacekit_info->analog_input_states[i]));
    else
      interfacekit_info->analog_input_states[i] = 0;

  return 0;
}

int CCONV interfacekit_on_detach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle)phid;

  // This is used to determine whethere to preserve the allocated memory on reattach:
  interfacekit_info->analog_input_count_prior = interfacekit_info->analog_input_count;

  // Free the polled resources:
  xfree(interfacekit_info->digital_input_states);
  interfacekit_info->digital_input_states = NULL;

  xfree(interfacekit_info->digital_output_states);
  interfacekit_info->digital_output_states = NULL;

  xfree(interfacekit_info->analog_input_states);
  interfacekit_info->analog_input_states = NULL;
  
  return 0;
}

void interfacekit_on_free(void *type_info) {
  InterfaceKitInfo *interfacekit_info = type_info;

  if(interfacekit_info->data_rates)
    xfree(interfacekit_info->data_rates);
  if(interfacekit_info->data_rates_min)
    xfree(interfacekit_info->data_rates_min);
  if(interfacekit_info->data_rates_max)
    xfree(interfacekit_info->data_rates_max);
  if(interfacekit_info->sensor_change_triggers)
    xfree(interfacekit_info->sensor_change_triggers);
  if (interfacekit_info->digital_input_states)
    xfree(interfacekit_info->digital_input_states);
  if (interfacekit_info->digital_output_states)
    xfree(interfacekit_info->digital_output_states);
  if (interfacekit_info->analog_input_states)
    xfree(interfacekit_info->analog_input_states);
  if (interfacekit_info)
    xfree(interfacekit_info);
  return;
}

int interfacekit_on_digital_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int inputState) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;

  device_sample(info, NULL);

  if (interfacekit_info->digital_input_states)
    interfacekit_info->digital_input_states[index] = inputState;

  return 0;
}

int interfacekit_on_analog_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int sensorValue) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;

  device_sample(info, NULL);

  if (interfacekit_info->analog_input_states) {
    if (interfacekit_info->rationmetric_changed_usec == 0) 
      interfacekit_info->analog_input_states[index] = sensorValue;
    else {
      // We need to wait 50 milliseconds before accepting values after a ratiometric
      // state change. If we're in this path, it's TBD whether 50 ms has passed
      int usec_difference;
      struct timeval now;
      gettimeofday(&now, NULL);

      usec_difference = ( now.tv_usec + 
        ( (now.tv_usec < interfacekit_info->rationmetric_changed_usec)  ? 1000000 : 0) -
        interfacekit_info->rationmetric_changed_usec );
         
      // Did we wait long enough between a ratiometric status change:
      if ( usec_difference > INTERFACEKIT_RATIOMETRIC_RESET_USECS ) {
        interfacekit_info->rationmetric_changed_usec = 0;
        
        // And sure, go ahead and accepti this value:
        interfacekit_info->analog_input_states[index] = sensorValue;
      }
    }
  }

  return 0;
}

int interfacekit_assert_ratiometric_state(PhidgetInfo *info) {
  InterfaceKitInfo *interfacekit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) info->handle;
  int ret = 0;

  int is_ratiometric_already;
  ret = CPhidgetInterfaceKit_getRatiometric(interfacekit, &is_ratiometric_already);
  if (ret != EPHIDGET_OK) return ret;

  if (interfacekit_info->is_ratiometric != (interfacekit_info->is_ratiometric == PTRUE) ) {
    ret = CPhidgetInterfaceKit_setRatiometric(interfacekit, (interfacekit_info->is_ratiometric) ? PTRUE : PFALSE );
    if (ret != EPHIDGET_OK) return ret;

    // Zero-out the analog struct:
    memset(interfacekit_info->analog_input_states, 0, sizeof(int) * interfacekit_info->analog_input_count);

    // We need to wait 50ms after this change before we start to read in values:
    struct timeval now;
    gettimeofday(&now, NULL);

    interfacekit_info->rationmetric_changed_usec = (now.tv_usec == 0) ? 1 : now.tv_usec;
  }

  return EPHIDGET_OK;
}
