/*
 * File:        GPIO.h
 * Purpose:     Provide GPIO  definitions
 *
 * Notes:		
 *
 */

#ifndef GPIO_H
#define GPIO_H
 
#include "MK64F12.h"
void LED_Init(void);
void LED_On (unsigned char color);
void init_GPIO(void);
void HBridge_init();
 #endif