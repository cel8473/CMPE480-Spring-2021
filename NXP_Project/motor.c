/*
 * File:        MOTOR.c
 * Purpose:     Provide Motor  functionality
 *
 * Notes:		
 *
 */

#ifndef MOTOR_H
#define MOTOR_H
 
#include "MK64F12.h"
#include "./modules/ftm.h"
#include "./modules/gpio.h"

#define PWM_MULT_FACTOR (0.034)
#define PWM_CONST       (5.2)
#define MOTOR_FREQ            (10000) // 10kHz Frequency
#define FORWARD         (0)
#define BACKWARD        (~FORWARD)
#define SERVO_FREQ      (50)

void init_motor(void)
{
  FTM0_init();
  FTM3_init();
  HBridge_init();
}
void drive_forward(unsigned int duty_cycle)
{
  FTM0_set_duty_cycle(duty_cycle,MOTOR_FREQ, FORWARD);
}

void drive_backward(unsigned int duty_cycle)
{
  FTM0_set_duty_cycle(duty_cycle,MOTOR_FREQ, BACKWARD);
}

/** 
 * Double duty_cycle - Requires a value from 0 - 100
 */
void turn_wheels(double duty_cycle)
{
  FTM3_set_duty_cycle(duty_cycle * PWM_MULT_FACTOR + PWM_CONST, SERVO_FREQ);
}


#endif