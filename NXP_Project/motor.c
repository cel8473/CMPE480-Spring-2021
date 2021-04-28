/*
 * File:        MOTOR.c
 * Purpose:     Provide Motor  functionality
 *
 * Notes:		
 */
 
#include "MK64F12.h"
#include "./modules/ftm.h"
#include "./modules/gpio.h"
#include "./motor.h"

#define DELAY (0)
double turn_speed;
double straight_speed;

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
	if(CARPET){
		if(sum < CARPET_SUM){
			drive(0, 0);
		}
		else{
			drive(left_speed, right_speed);
		}
	}else{
		drive(left_speed, right_speed);
	}
}

/** 
 * Double duty_cycle - Requires a value from 0 - 100
 */
void turn_wheels(double duty_cycle, int sum)
{
	int turn_scaling = 0.04*(duty_cycle-50)*(duty_cycle-50);
  FTM3_set_duty_cycle(duty_cycle * PWM_MULT_FACTOR + PWM_CONST, SERVO_FREQ);
	int left_speed, right_speed;
	int right_bound = 63;
	int left_bound = 37;
	turn_scaling = turn_scaling > 80 ? 80 : turn_scaling;
	if(duty_cycle<right_bound && duty_cycle>left_bound)
	{ //straight
		left_speed = straight_speed;
		right_speed = straight_speed;
	}
	else if(duty_cycle>=right_bound)
	{ //right
		left_speed = turn_speed*(1-turn_scaling/100) * DIFF_FACTOR; 
		right_speed = turn_speed*(1-turn_scaling/100);
	}
	else if(duty_cycle<= left_bound)
	{ //left
		left_speed = turn_speed*(1-turn_scaling/100);
		right_speed = turn_speed*(1-turn_scaling/100)*DIFF_FACTOR;
	}
	drive_wheels(left_speed, right_speed, sum);
}

PID_T turn_amount(int middle, PID_T cont){
	cont.err=DESIRED_POSITION-middle;
	cont.turnAmt= cont.turnOld - cont.Kp*(cont.err-cont.errOld1) - cont.Ki*(cont.err+cont.errOld1)/2 - cont.Kd*(cont.err-2*cont.errOld1+cont.errOld2);
	//Boost
	if(cont.turnAmt >= 75){
		cont.turnAmt *= BOOST;
	}
	else if(cont.turnAmt <= -75){
		cont.turnAmt *= BOOST;
	}
	//Capped
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

void set_straight_speed(double speed)
{
	straight_speed = speed;
}

double get_straight_speed()
{
	return straight_speed;
}
void set_turn_speed(double speed)
{
	turn_speed = speed;
}

double get_turn_speed()
{
	return turn_speed;
}

/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void motor_delay(int del){
	volatile int i;
	for (i=0; i<del*50000; i++){
		// Do nothing
	}
}