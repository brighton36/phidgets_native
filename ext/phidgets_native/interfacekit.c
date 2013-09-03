#include "phidgets_native.h"

int CCONV interfacekit_on_attach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *ifkit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) phid;

  report(CPhidgetInterfaceKit_getInputCount(interfacekit, &ifkit_info->digital_input_count));
  ifkit_info->is_digital_input_count_known = true;

  report(CPhidgetInterfaceKit_getOutputCount(interfacekit, &ifkit_info->digital_output_count));
  ifkit_info->is_digital_output_count_known = true;

  report(CPhidgetInterfaceKit_getSensorCount(interfacekit, &ifkit_info->analog_input_count));
  ifkit_info->is_analog_input_count_known = true;

  // Allocate space for our state collections:
  ifkit_info->digital_input_states = ALLOC_N(int, ifkit_info->digital_input_count); 
  ifkit_info->analog_input_states = ALLOC_N(int, ifkit_info->analog_input_count); 

  if (ifkit_info->digital_input_count_prior != ifkit_info->digital_input_count) {
    if(ifkit_info->digital_sample_rates) xfree(ifkit_info->digital_sample_rates);

    ifkit_info->digital_sample_rates = ALLOC_N(SampleRate, ifkit_info->digital_input_count); 

    memset(ifkit_info->digital_sample_rates, 0, sizeof(SampleRate) * ifkit_info->digital_input_count);
  }

  if (ifkit_info->digital_output_count_prior != ifkit_info->digital_output_count) {
    if(ifkit_info->digital_output_states) xfree(ifkit_info->digital_output_states);

    ifkit_info->digital_output_states = ALLOC_N(int, ifkit_info->digital_output_count); 

    memset(ifkit_info->digital_output_states, 0, sizeof(int) * ifkit_info->digital_output_count);
  }

  if (ifkit_info->analog_input_count_prior != ifkit_info->analog_input_count) {
    if(ifkit_info->data_rates) xfree(ifkit_info->data_rates);
    if(ifkit_info->data_rates_max) xfree(ifkit_info->data_rates_max);
    if(ifkit_info->data_rates_min) xfree(ifkit_info->data_rates_min);
    if(ifkit_info->sensor_change_triggers) xfree(ifkit_info->sensor_change_triggers);
    if(ifkit_info->analog_sample_rates) xfree(ifkit_info->analog_sample_rates);
    if(ifkit_info->is_ratiometric) xfree(ifkit_info->is_ratiometric);
    if(ifkit_info->updated_since_last_ratiometric_cycle) xfree(ifkit_info->updated_since_last_ratiometric_cycle);

    ifkit_info->data_rates = ALLOC_N(int, ifkit_info->analog_input_count);
    ifkit_info->data_rates_max = ALLOC_N(int, ifkit_info->analog_input_count);
    ifkit_info->data_rates_min = ALLOC_N(int, ifkit_info->analog_input_count);
    ifkit_info->sensor_change_triggers = ALLOC_N(int, ifkit_info->analog_input_count);
    ifkit_info->analog_sample_rates = ALLOC_N(SampleRate, ifkit_info->analog_input_count); 
    ifkit_info->is_ratiometric = ALLOC_N(bool, ifkit_info->analog_input_count); 
    ifkit_info->updated_since_last_ratiometric_cycle = ALLOC_N(bool, ifkit_info->analog_input_count); 

    memset(ifkit_info->analog_sample_rates, 0, sizeof(SampleRate) * ifkit_info->analog_input_count);
    memset(ifkit_info->is_ratiometric, 0, sizeof(bool) * ifkit_info->analog_input_count);
    memset(ifkit_info->updated_since_last_ratiometric_cycle, 0, sizeof(bool) * ifkit_info->analog_input_count);

    for(int i=0;i<ifkit_info->analog_input_count;i++) {
      ifkit_info->data_rates[i] = DEFAULT_INTERFACEKIT_DATA_RATE;
      ifkit_info->sensor_change_triggers[i] = DEFAULT_INTERFACEKIT_CHANGE_TRIGGER;
      
      report(CPhidgetInterfaceKit_getDataRateMax(interfacekit, i, &ifkit_info->data_rates_max[i]));
      report(CPhidgetInterfaceKit_getDataRateMin(interfacekit, i, &ifkit_info->data_rates_min[i]));
    }

    ifkit_info->is_data_rates_known = true;
  }

  if ( interfacekit_ratiometric_state_is_uniform(ifkit_info) ) {
    // Set the board's ratiometric state:
    report(interfacekit_assert_ratiometric_state( info ));
    // Set the data-rate/sensor-thresholds
    report(interfacekit_assert_sensor_rates(info));
  }
  else {
    report(interfacekit_assert_dual_ratiometric_mode(info));
    report(interfacekit_maximize_data_rate(info));
  }

  // Read in all of our initial input values:
  for(int i=0; i<ifkit_info->digital_input_count; i++)
    report(CPhidgetInterfaceKit_getInputState(interfacekit,i, &ifkit_info->digital_input_states[i]));

  for(int i=0; i<ifkit_info->digital_output_count; i++)
    report(CPhidgetInterfaceKit_setOutputState(interfacekit,i, ifkit_info->digital_output_states[i]));

  for(int i=0; i<ifkit_info->analog_input_count; i++)
    if (ifkit_info->ratiometric_changed_usec == 0) 
      // We only read these in if we haven't recently been changed ratiometric status: 
      report(CPhidgetInterfaceKit_getSensorValue(interfacekit,i, &ifkit_info->analog_input_states[i]));
    else
      ifkit_info->analog_input_states[i] = 0;

  return 0;
}

int CCONV interfacekit_on_detach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *ifkit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle)phid;

  // This is used to determine whethere to preserve the allocated memory on reattach:
  ifkit_info->analog_input_count_prior = ifkit_info->analog_input_count;
  ifkit_info->digital_input_count_prior = ifkit_info->digital_input_count;
  ifkit_info->digital_output_count_prior = ifkit_info->digital_output_count;

  // Free the polled resources:
  xfree(ifkit_info->digital_input_states);
  ifkit_info->digital_input_states = NULL;

  xfree(ifkit_info->analog_input_states);
  ifkit_info->analog_input_states = NULL;
 
  // Zero the sample trackers: 
  memset(ifkit_info->digital_sample_rates, 0, sizeof(SampleRate) * ifkit_info->digital_input_count);
  memset(ifkit_info->analog_sample_rates, 0, sizeof(SampleRate) * ifkit_info->analog_input_count);
  
  return 0;
}

void interfacekit_on_free(void *type_info) {
  InterfaceKitInfo *ifkit_info = type_info;

  if(ifkit_info->is_ratiometric) xfree(ifkit_info->is_ratiometric);
  if(ifkit_info->updated_since_last_ratiometric_cycle) xfree(ifkit_info->updated_since_last_ratiometric_cycle);
  if(ifkit_info->data_rates) xfree(ifkit_info->data_rates);
  if(ifkit_info->data_rates_min) xfree(ifkit_info->data_rates_min);
  if(ifkit_info->data_rates_max) xfree(ifkit_info->data_rates_max);
  if(ifkit_info->sensor_change_triggers) xfree(ifkit_info->sensor_change_triggers);
  if (ifkit_info->digital_input_states) xfree(ifkit_info->digital_input_states);
  if (ifkit_info->digital_output_states) xfree(ifkit_info->digital_output_states);
  if (ifkit_info->analog_input_states) xfree(ifkit_info->analog_input_states);
  if(ifkit_info->digital_sample_rates) xfree(ifkit_info->digital_sample_rates);
  if(ifkit_info->analog_sample_rates) xfree(ifkit_info->analog_sample_rates);
  if (ifkit_info) xfree(ifkit_info);
  return;
}

int interfacekit_on_digital_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int inputState) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *ifkit_info = info->type_info;

  sample_tick(&ifkit_info->digital_sample_rates[index], NULL);

  if (ifkit_info->digital_input_states)
    ifkit_info->digital_input_states[index] = inputState;

  return 0;
}

int interfacekit_on_analog_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int sensorValue) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *ifkit_info = info->type_info;

  bool record_value = false;

  if (ifkit_info->analog_input_states) {
    if (ifkit_info->is_dual_ratiometric_mode && interfacekit_is_time_to_flip_ratiometric_state(ifkit_info) ) {
      report(interfacekit_flip_ratiometric_state(info));
    } else if (ifkit_info->ratiometric_changed_usec == 0) 
      record_value = true;
    else {
      // We need to wait 50 milliseconds before accepting values after a ratiometric
      // state change. If we're in this path, it's TBD whether 50 ms has passed
      int usec_difference;
      struct timeval now;
      gettimeofday(&now, NULL);

      usec_difference = ( now.tv_usec + 
        ( (now.tv_usec < ifkit_info->ratiometric_changed_usec)  ? 1000000 : 0) -
        ifkit_info->ratiometric_changed_usec );
         
      // Did we wait long enough between a ratiometric status change:
      if ( usec_difference > INTERFACEKIT_RATIOMETRIC_RESET_USECS ) {
        ifkit_info->ratiometric_changed_usec = 0;
        
        // And sure, go ahead and accept this value:
        record_value = true;
      }
    }

    if (record_value) {
      ifkit_info->analog_input_states[index] = sensorValue;
      sample_tick(&ifkit_info->analog_sample_rates[index], NULL);
      if(ifkit_info->is_dual_ratiometric_mode)
        ifkit_info->updated_since_last_ratiometric_cycle[index] = true;
    }
  }

  return 0;
}

int interfacekit_assert_ratiometric_state(PhidgetInfo *info) {
  InterfaceKitInfo *ifkit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) info->handle;
  int ret = 0;

  bool change_mode = false;

  if (ifkit_info->is_dual_ratiometric_mode) {
    ifkit_info->is_dual_ratiometric_mode = false;
    change_mode = true;
    report(interfacekit_assert_sensor_rates(info));
  } else {
    int is_ratiometric_already;
    ret = CPhidgetInterfaceKit_getRatiometric(interfacekit, &is_ratiometric_already);
    if (ret != EPHIDGET_OK) return ret;

    if (ifkit_info->is_ratiometric[0] != (is_ratiometric_already == PTRUE) )
      change_mode = true;
  }

  if (change_mode) {
    ret = CPhidgetInterfaceKit_setRatiometric(interfacekit, (ifkit_info->is_ratiometric[0]) ? PTRUE : PFALSE );
    if (ret != EPHIDGET_OK) return ret;

    // Zero-out the analog struct:
    memset(ifkit_info->analog_input_states, 0, sizeof(int) * ifkit_info->analog_input_count);

    // We need to wait 50ms after this change before we start to read in values:
    interfacekit_stamp_ratiometric_change(ifkit_info);
  }

  return EPHIDGET_OK;
}

bool interfacekit_ratiometric_state_is_uniform(InterfaceKitInfo *ifkit_info) {
  bool first_state = ifkit_info->is_ratiometric[0];

  for(int i=1; i<ifkit_info->analog_input_count;i++)
    if (ifkit_info->is_ratiometric[i] != first_state)
      return false;

  return true;
}

bool interfacekit_is_time_to_flip_ratiometric_state(InterfaceKitInfo *ifkit_info) {
  for(int i=0; i<ifkit_info->analog_input_count;i++)
    if (ifkit_info->updated_since_last_ratiometric_cycle[i] == false)
      return false;

  return true;
}

// This method ensures that the sensor data rates are set to what's been requested
// by the user. It's typically only used when initializing, and when turning on
// a uniform ratiometric state:
int interfacekit_assert_sensor_rates(PhidgetInfo *info) {
  InterfaceKitInfo *ifkit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) info->handle;

  for(int i=0;i<ifkit_info->analog_input_count;i++) {
    if (ifkit_info->data_rates[i] > 0) {
      report(CPhidgetInterfaceKit_setSensorChangeTrigger(interfacekit, i, 0));
      report(CPhidgetInterfaceKit_setDataRate(interfacekit, i, ifkit_info->data_rates[i]));
    } else {
      report(CPhidgetInterfaceKit_setDataRate(interfacekit, i, 0));
      report(CPhidgetInterfaceKit_setSensorChangeTrigger(interfacekit, i, ifkit_info->sensor_change_triggers[i]));
    }
  }

  return EPHIDGET_OK;
}

int interfacekit_assert_dual_ratiometric_mode(PhidgetInfo *info) {
  InterfaceKitInfo *ifkit_info = info->type_info;

  if (!ifkit_info->is_dual_ratiometric_mode) {
    ifkit_info->is_dual_ratiometric_mode = true;
    memset(ifkit_info->updated_since_last_ratiometric_cycle, 0, sizeof(bool) * ifkit_info->analog_input_count);
    return interfacekit_maximize_data_rate(info);
  }

  return EPHIDGET_OK;
}

int interfacekit_maximize_data_rate(PhidgetInfo *info) {
  InterfaceKitInfo *ifkit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) info->handle;

  for(int i=0;i<ifkit_info->analog_input_count;i++)
    if (ifkit_info->data_rates[i] > 0) {
      report(CPhidgetInterfaceKit_setSensorChangeTrigger(interfacekit, i, 0));
      report(CPhidgetInterfaceKit_setDataRate(interfacekit, i, DEFAULT_INTERFACEKIT_DATA_RATE));
    }

  return EPHIDGET_OK;
}

int interfacekit_flip_ratiometric_state(PhidgetInfo *info) {
  InterfaceKitInfo *ifkit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle) info->handle;

  int ret;
  int current_state;

  ret = CPhidgetInterfaceKit_getRatiometric(interfacekit, &current_state);
  if (ret != EPHIDGET_OK) return ret;

  ret = CPhidgetInterfaceKit_setRatiometric(interfacekit, (current_state) ? PFALSE : PTRUE );
  if (ret != EPHIDGET_OK) return ret;

  // We need to wait 50ms after this change before we start to read in values:
  interfacekit_stamp_ratiometric_change(ifkit_info);

  memset(ifkit_info->updated_since_last_ratiometric_cycle, 0, sizeof(bool) * ifkit_info->analog_input_count);

  return EPHIDGET_OK;
}

// This stamps the ifkit struct with the current time. We cheat a bit to make
// our lookup code a bit easier, by stamping the case of 0 with a 1:
int interfacekit_stamp_ratiometric_change(InterfaceKitInfo *ifkit_info) {
  struct timeval now;
  gettimeofday(&now, NULL);

  ifkit_info->ratiometric_changed_usec = (now.tv_usec == 0) ? 1 : now.tv_usec;

  return EPHIDGET_OK;
}
