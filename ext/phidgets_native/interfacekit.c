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

  int is_ratiometric;
  report(CPhidgetInterfaceKit_getRatiometric(interfacekit, &is_ratiometric));
  interfacekit_info->is_ratiometric = (is_ratiometric == PTRUE) ? true : false;
  interfacekit_info->is_ratiometric_known = true;

  // TODO: flesh out an array of extents:
  //report(CPhidgetInterfaceKit_getDataRateMax(interfacekit, &interfacekit_info->data_rate_max));
  //report(CPhidgetInterfaceKit_getDataRateMin(interfacekit, &interfacekit_info->data_rate_min));
  
  // Dealloc any states collections that are currently alloc'd:
  if (interfacekit_info->digital_input_states) xfree(interfacekit_info->digital_input_states);
  if (interfacekit_info->digital_output_states) xfree(interfacekit_info->digital_output_states);
  if (interfacekit_info->analog_input_states) xfree(interfacekit_info->analog_input_states);

  // Allocate space for our state collections:
  interfacekit_info->digital_input_states = ALLOC_N(int, interfacekit_info->digital_input_count); 
  interfacekit_info->digital_output_states = ALLOC_N(int, interfacekit_info->digital_output_count); 
  interfacekit_info->analog_input_states = ALLOC_N(int, interfacekit_info->analog_input_count); 

  // Read in all of our initial input values:
  for(int i=0; i<interfacekit_info->digital_input_count; i++)
    report(CPhidgetInterfaceKit_getInputState(interfacekit,i, &interfacekit_info->digital_input_states[i]));

  for(int i=0; i<interfacekit_info->digital_output_count; i++)
    report(CPhidgetInterfaceKit_getOutputState(interfacekit,i, &interfacekit_info->digital_output_states[i]));
  
  for(int i=0; i<interfacekit_info->analog_input_count; i++)
    report(CPhidgetInterfaceKit_getSensorValue(interfacekit,i, &interfacekit_info->analog_input_states[i]));

  // Set the data_rates and/or threshold rates
  //if ( DEFAULT_INTERFACEKIT_DATA_RATE > 0) 
  //interfacekit_info->data_rate = DEFAULT_INTERFACEKIT_DATA_RATE;
  //interfacekit_info->sensor_change_trigger = DEFAULT_INTERFACEKIT_CHANGE_TRIGGER;

  //if (interfacekit_info->data_rate > 0)
    //report(CPhidgetSpatial_setDataRate(interfacekit, interfacekit_info->data_rate));
  //else
    //report(CPhidgetInterfaceKit_setSensorChangeTrigger(interfacekit, interfacekit_info->sensor_change_trigger));

  return 0;
}

int CCONV interfacekit_on_detach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;
  CPhidgetInterfaceKitHandle interfacekit = (CPhidgetInterfaceKitHandle)phid;

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

  // TODO: Dealloc the data_rates and change triggers
  
  xfree(interfacekit_info->digital_input_states);
  xfree(interfacekit_info->digital_output_states);
  xfree(interfacekit_info->analog_input_states);
  xfree(interfacekit_info);
  return;
}

int interfacekit_on_digital_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int inputState) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;

  return 0;
}

int interfacekit_on_analog_change(CPhidgetInterfaceKitHandle interfacekit, void *userptr, int index, int sensorValue) {
  PhidgetInfo *info = userptr;
  InterfaceKitInfo *interfacekit_info = info->type_info;

  return 0;
}
