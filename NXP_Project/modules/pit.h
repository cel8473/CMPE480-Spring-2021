/*
 * File:        pit.h
 * Purpose:     Provide PIT  definitions
 *
 * Notes:		
 *
 */

 #ifndef PIT_H
 #define PIT_H
 
 #include "MK64F12.h"
 
void init_PIT(void);
void PIT0_IRQHandler(void);
 #endif