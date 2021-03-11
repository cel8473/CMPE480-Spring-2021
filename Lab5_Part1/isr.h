/*
 * isr.h
 */

#include "MK64F12.h"
#include <stdio.h>
#include "uart.h"


void PDB0_IRQHandler(void);//For PDB timer

void FTM0_IRQHandler(void); //For FTM timer
	
void PORTA_IRQHandler(void); //For switch 3
	
void PORTC_IRQHandler(void); //For switch 2

