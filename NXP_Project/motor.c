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
#define MOTOR_FREQ      (10000) // 10kHz Frequency
#define FORWARD         (0)
#define BACKWARD        (~FORWARD)
#define SERVO_FREQ      (50)
#define LEFT (48)
#define	RIGHT (55)
#define LEFT_ADJUSTER  (30)
#define RIGHT_ADJUSTER (30)
#define CARPET (15)
#define STRAIGHT_SPEED (60)
#define TURN_SPEED (45)

// Initializes the camera
void init_motor(void)
{
  FTM0_init(); // dc motors
  FTM3_init(); // servo
  HBridge_init();
}

// Drives the car forward with a given duty cycle value
void drive_forward(unsigned int duty_cycle)
{
  FTM0_set_duty_cycle(duty_cycle,MOTOR_FREQ, FORWARD);
}

// Drives the car backward with a given duty cycle value
void drive_backward(unsigned int duty_cycle)
{
  FTM0_set_duty_cycle(duty_cycle,MOTOR_FREQ, BACKWARD);
}

void drive_wheels(unsigned int speed, int sum){
	if(sum < CARPET){
		drive_forward(0);
	}
	else{
		drive_forward(speed);
	}
}

/** 
 * Double duty_cycle - Requires a value from 0 - 100
 */
void turn_wheels(double duty_cycle)
{
  FTM3_set_duty_cycle(duty_cycle * PWM_MULT_FACTOR + PWM_CONST, SERVO_FREQ);
}

// Adjusts the wheels of the car given the mid point from the camera array
void wheel_adjust(int middle, int sum){
	if(middle >= LEFT && middle <= RIGHT){
		turn_wheels(50.0);
		drive_wheels(STRAIGHT_SPEED, sum);
	}
	else if(middle > RIGHT)
	{
		middle = (middle + RIGHT_ADJUSTER) <= 100 ? middle + RIGHT_ADJUSTER: 100;
		turn_wheels((double)middle);
		drive_wheels(TURN_SPEED, sum);
	}
	else if(middle < LEFT)
	{
		middle = (middle - LEFT_ADJUSTER) >= 0 ? middle - LEFT_ADJUSTER: 0;
		turn_wheels((double)middle);
		drive_wheels(TURN_SPEED, sum);
	}
}


#endif