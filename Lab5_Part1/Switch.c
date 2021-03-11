/* 
Title: Switch.c
Purpose: 
Name: Chris Larson  
Date: 1/29/21
*/
#include "MK64F12.h"                    // Device header
#include "Switch.h"
void Switch2_Init(void)
{
	// 12.2.12 System Clock Gating Control Register 5
	// Port C is Bit 11 in SCGC5
	// Enable clock for Port C PTC6 button
	SIM_SCGC5|=SIM_SCGC5_PORTC_MASK;
	// Configure the Mux for the button
	PORTC_PCR6=PORT_PCR_MUX(1);
	// Set the push button as an input
	// GPIOC_PDDR is the direction control for Port C
	// set it to ZERO at BIT6 for an input
	GPIOC_PDDR&= ~(1ul<<6);
	//Pull enable and pull up resistor
	PORTC_PCR6|=PORT_PCR_PE_MASK;
	PORTC_PCR6|=PORT_PCR_PS_MASK;
}

unsigned char Switch2_Pressed(void)
{
	// check if switch2 pressed ()
	// if PORT C - BIT6 is ZERO, then the switch is pressed
	// PDIR - Port Data Input Register
	// return a ZERO if NOT Pressed
	// return a 0xFF if Pressed
	if((GPIOC_PDIR&(1<<6))==0){
			return 0xFF;
	}
	else{
		return 0;
	}
}

void Switch3_Init(void)
{
	// 12.2.12 System Clock Gating Control Register 5
	// Port C is Bit 11 in SCGC5
	// Enable clock for Port C PTC6 button
	SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;
	
	// Configure the Mux for the button
	PORTA_PCR4=PORT_PCR_MUX(1);
	// Set the push button as an input
	// GPIOC_PDDR is the direction control for Port C
	// set it to ZERO at BIT6 for an input
	GPIOA_PDDR&= ~(1ul<<4);
	//Pull enable and pull up resistor
	PORTA_PCR4|=PORT_PCR_PE_MASK;
	PORTA_PCR4|=PORT_PCR_PS_MASK;
}

unsigned char Switch3_Pressed(void)
{
	// check if switch2 pressed ()
	// if PORT C - BIT6 is ZERO, then the switch is pressed
	// PDIR - Port Data Input Register
	// return a ZERO if NOT Pressed
	// return a 0xFF if Pressed
	if((GPIOA_PDIR&(1<<4))==0){
			return 0xFF;
	}
	else{
		return 0;
	}
}
