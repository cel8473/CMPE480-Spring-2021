/* 
Title: Led.h
Purpose: 
Name: Chris Larson  
Date: 1/29/21
*/
#include "MK64F12.h"                    // Device header

#ifndef _LED_H
#define _LED_H
void LED_Off(void);

void LED_Init(void);

void LED_On (unsigned char color);
#endif
