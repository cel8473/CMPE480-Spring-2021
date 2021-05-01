/*
 * File:        MOTOR.h
 * Purpose:     Provide Motor  definitions
 *
 * Notes:		
 *
 */

#ifndef MOTOR_H
#define MOTOR_H
 
#include "MK64F12.h"


#define PWM_MULT_FACTOR (0.034)
#define PWM_CONST       (5.2)
#define MOTOR_FREQ      (10000) // 10kHz Frequency
#define SERVO_FREQ      (50)
#define CARPET_SUM (15)
#define CARPET (1)
#define DESIRED_POSITION (53)
#define DIFF_FACTOR (0.1)
#define BOOST (1.35)
#define DAMP (1.1)

typedef struct PID_S {
	double turnAmt;
	double turnOld;
	double err;
	double errOld1;
	double errOld2;
	double Kp;
	double Ki;
	double Kd;
	int sum;
} PID_T;

void init_motor(void);

void drive(int dc_left, int dc_right);

void drive_wheels(int left_speed, int right_speed, int sum);

void turn_wheels(double duty_cycle, int sum);

void wheel_adjust(int middle, int sumActual);

PID_T turn_amount(int middle, PID_T cont);

void set_straight_speed(double speed);
double get_straight_speed();

void set_turn_speed(double speed);
double get_turn_speed();

void motor_delay(int del);
#endif