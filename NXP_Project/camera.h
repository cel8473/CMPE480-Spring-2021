/* 
Title: camera.h
Purpose: 
Name: Chris Larson  
Date: 3/21/21
*/
#include "MK64F12.h"                    // Device header
#include "adc.h"

#if 0
void init_FTM2(void);
void init_GPIO(void);
void init_PIT(void);
void init_ADC0(void);
void FTM2_IRQHandler(void);
void PIT0_IRQHandler(void);
void ADC0_IRQHandler(void);
#endif

void init_camera(void);
int edgeFinder(uint16_t line[], int side);