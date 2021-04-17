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

void init_motor(void);

void drive(int duty_cycle);

void drive_wheels(unsigned int speed, int sum);

void turn_wheels(double duty_cycle, int sum);

void wheel_adjust(int middle, int sumActual);

struct PID turn_amount(int middle, struct PID cont);
#endif