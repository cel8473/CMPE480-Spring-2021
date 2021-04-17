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
#define SERVO_FREQ      (50)
#define CARPET (15)
#define STRAIGHT_SPEED (50)
#define TURN_SPEED (40)
#define DESIRED_POSITION (54)

struct PID{
	double turnAmt;
	double turnOld;
	double err;
	double errOld1;
	double errOld2;
	double Kp;
	double Ki;
	double Kd;
	int sum;
};

// Initializes the camera
void init_motor(void)
{
  FTM0_init(); // dc motors
  FTM3_init(); // servo
  HBridge_init();
}

// Drives the car forward with a given duty cycle value
void drive(int dc_left, int dc_right)
{
	FTM0_set_duty_cycle_right(dc_right,MOTOR_FREQ);
	FTM0_set_duty_cycle_left(dc_left,MOTOR_FREQ);
}

void drive_wheels(int left_speed, int right_speed, int sum){
	if(sum < CARPET){
		drive(0, 0);
	}
	else{
		drive(left_speed, right_speed);
	}
}

/** 
 * Double duty_cycle - Requires a value from 0 - 100
 */
void turn_wheels(double duty_cycle, int sum)
{
  FTM3_set_duty_cycle(duty_cycle * PWM_MULT_FACTOR + PWM_CONST, SERVO_FREQ);
	int left_speed, right_speed;
	if(duty_cycle<60 && duty_cycle>40){
		left_speed = STRAIGHT_SPEED;
		right_speed = STRAIGHT_SPEED;
	}else if(duty_cycle>=60){
		left_speed = TURN_SPEED-10;
		right_speed = TURN_SPEED;
	}else{
		left_speed = TURN_SPEED;
		right_speed = TURN_SPEED-10;
	}
	drive_wheels(left_speed, right_speed, sum);
}

struct PID turn_amount(int middle, struct PID cont){
	cont.err=DESIRED_POSITION-middle;
	cont.turnAmt = cont.turnOld - cont.Kp*(cont.err-cont.errOld1) - cont.Ki*(cont.err+cont.errOld1)/2 - cont.Kd*(cont.err-2*cont.errOld1+cont.errOld2);
	if(cont.turnAmt > 100){
		cont.turnAmt = 100;
	}
	else if(cont.turnAmt < -100){
		cont.turnAmt = -100;
	}
	cont.turnOld = cont.turnAmt;
	cont.errOld2 = cont.errOld1;
	cont.errOld1 = cont.err;
	cont.turnAmt = (cont.turnAmt+100)/2;
	turn_wheels(cont.turnAmt, cont.sum);
	return cont;
}

#endif