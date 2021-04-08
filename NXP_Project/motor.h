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


void init_motor(void);

void drive_forward(unsigned int duty_cycle);

void drive_backward(unsigned int duty_cycle);

void drive_wheels(unsigned int speed, int sum);

void turn_wheels(double duty_cycle);

void wheel_adjust(int middle, int sum);
#endif