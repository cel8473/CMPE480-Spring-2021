/*
 * File:        FTM.h
 * Purpose:     Provide FTM  definitions
 *
 * Notes:		
 *
 */

 #ifndef FTM_H
 #define FTM_H
 
 #include "MK64F12.h"

// Camera
void init_FTM2(void);
void FTM2_IRQHandler(void);

// DC Motor
void FTM0_set_duty_cycle_left(int duty_cycle, unsigned int frequency);
void FTM0_set_duty_cycle_right(int duty_cycle, unsigned int frequency);
void FTM0_init();

// PWM for servo
void FTM3_set_duty_cycle(double duty_cycle, unsigned int frequency);
void FTM3_init();
 #endif