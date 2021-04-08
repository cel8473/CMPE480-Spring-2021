/*
 * File:        uart.h
 * Purpose:     Provide ADC  definitions
 *
 * Notes:		
 *
 */

 #ifndef ADC_H
 #define ADC_H
 
 #include "MK64F12.h"

 void init_ADC0(void);
 void ADC0_IRQHandler(void);
 uint16_t get_ADC0_VAL(void);
 #endif