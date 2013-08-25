#include "phidgets_native.h"

int CCONV advancedservo_on_attach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  AdvancedServoInfo *servo_info = info->type_info;
  CPhidgetAdvancedServoHandle advancedservo = (CPhidgetAdvancedServoHandle) phid;

  report(CPhidgetAdvancedServo_getMotorCount(advancedservo, &servo_info->motor_count));
  servo_info->is_motor_count_known = true;

  if (servo_info->motor_count != servo_info->motor_count_prior) {
    if(servo_info->acceleration_min) xfree(servo_info->acceleration_min);
    if(servo_info->acceleration_max) xfree(servo_info->acceleration_max);
    if(servo_info->velocity_min) xfree(servo_info->velocity_min);
    if(servo_info->velocity_max) xfree(servo_info->velocity_max);
    if(servo_info->sample_rates) xfree(servo_info->sample_rates);

    servo_info->acceleration_max = ALLOC_N(double, servo_info->motor_count); 
    servo_info->acceleration_min = ALLOC_N(double, servo_info->motor_count); 
    servo_info->velocity_min = ALLOC_N(double, servo_info->motor_count); 
    servo_info->velocity_max = ALLOC_N(double, servo_info->motor_count); 
    servo_info->sample_rates = ALLOC_N(SampleRate, servo_info->motor_count); 

    memset(servo_info->sample_rates, 0, sizeof(SampleRate) * servo_info->motor_count);

    for (int i=0; i < servo_info->motor_count; i++) {
      report(CPhidgetAdvancedServo_getAccelerationMax(advancedservo,i,&servo_info->acceleration_max[i]));
      report(CPhidgetAdvancedServo_getAccelerationMin(advancedservo,i,&servo_info->acceleration_min[i]));
      report(CPhidgetAdvancedServo_getVelocityMax(advancedservo,i,&servo_info->velocity_max[i]));
      report(CPhidgetAdvancedServo_getVelocityMin(advancedservo,i,&servo_info->velocity_min[i]));
    }
  }

  servo_info->current = ALLOC_N(double, servo_info->motor_count); 
  servo_info->position = ALLOC_N(double, servo_info->motor_count); 
  servo_info->velocity = ALLOC_N(double, servo_info->motor_count); 
  
  memset(servo_info->current, 0, sizeof(double) * servo_info->motor_count);
  memset(servo_info->position, 0, sizeof(double) * servo_info->motor_count);
  memset(servo_info->velocity, 0, sizeof(double) * servo_info->motor_count);

  return 0;
}

int CCONV advancedservo_on_detach(CPhidgetHandle phid, void *userptr) {
  PhidgetInfo *info = userptr;
  AdvancedServoInfo *servo_info = info->type_info;

  servo_info->motor_count_prior = servo_info->motor_count;

  if(servo_info->current) xfree(servo_info->current);
  if(servo_info->position) xfree(servo_info->position);
  if(servo_info->velocity) xfree(servo_info->velocity);

  servo_info->current = 0;
  servo_info->position = 0;
  servo_info->velocity = 0;

  memset(servo_info->sample_rates, 0, sizeof(SampleRate) * servo_info->motor_count);

  return 0;
}

void advancedservo_on_free(void *type_info) {
  AdvancedServoInfo *servo_info = type_info;

  if(servo_info->sample_rates) xfree(servo_info->sample_rates);
  if(servo_info->acceleration_min) xfree(servo_info->acceleration_min);
  if(servo_info->acceleration_max) xfree(servo_info->acceleration_max);
  if(servo_info->velocity_min) xfree(servo_info->velocity_min);
  if(servo_info->velocity_max) xfree(servo_info->velocity_max);
  if(servo_info->current) xfree(servo_info->current);
  if(servo_info->position) xfree(servo_info->position);
  if(servo_info->velocity) xfree(servo_info->velocity);
  if (servo_info) xfree(servo_info);

  return;
}

int CCONV advancedservo_on_velocity_change(CPhidgetAdvancedServoHandle phid, void *userptr, int index, double velocity) {
  PhidgetInfo *info = userptr;
  AdvancedServoInfo *servo_info = info->type_info;

  sample_tick(&servo_info->sample_rates[index], NULL);

  if (servo_info->velocity) servo_info->velocity[index] = velocity;

  return 0;
}

int CCONV advancedservo_on_position_change(CPhidgetAdvancedServoHandle phid, void *userptr, int index, double position) {
  PhidgetInfo *info = userptr;
  AdvancedServoInfo *servo_info = info->type_info;

  sample_tick(&servo_info->sample_rates[index], NULL);

  if (servo_info->position) servo_info->position[index] = position;

  return 0;
}

int CCONV advancedservo_on_current_change(CPhidgetAdvancedServoHandle phid, void *userptr, int index, double current) {
  PhidgetInfo *info = userptr;
  AdvancedServoInfo *servo_info = info->type_info;

  sample_tick(&servo_info->sample_rates[index], NULL);

  if (servo_info->current) servo_info->current[index] = current;

  return 0;
}

