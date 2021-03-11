/* 
Title: Led.c
Purpose: 
Name: Chris Larson  
Date: 1/29/21
*/
#include "MK64F12.h"                    // Device header
#include "led.h"

void LED_Off(void)
{
	// set the bits to ONE to turn off LEDs
	// use PSOR to set a bit
	GPIOB_PSOR|=(1<<22); //red
	GPIOE_PSOR|=(1<<26); //green
	GPIOB_PSOR|=(1<<21); //blue
}

void LED_Init(void)
{
	// Enable clocks on Ports B and E for LED timing
	// We use PortB for RED and BLUE LED
	// We use PortE for GREEN LED
	// 12.2.12 System Clock Gating Control Register 5
	// Port B is Bit 10
	// Port E is Bit 13
				   // 0x0400 (Bit 10)                 0x2000 (Bit 13)
	SIM_SCGC5|=SIM_SCGC5_PORTB_MASK;//Enables Clock on PORTB
	SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;//Enables Clock on PORTE
	// Configure the Signal Multiplexer for GPIO
 	// Pin Control Register n  
	PORTB_PCR22=PORT_PCR_MUX(1); //red
	PORTE_PCR26=PORT_PCR_MUX(1); //green
	PORTB_PCR21=PORT_PCR_MUX(1); //blue
	// Switch the GPIO pins to output mode
	// GPIOB_DDR is the direction control for Port B
	// GPIOE_DDR is the direction control for Port E
	// set it to ONE at BIT21, 22 on Port B for an output
	// set it to ONE at Bit26 on Port E for an output	 
  GPIOB_PDDR|=(1<<22); //red
	GPIOE_PDDR|=(1<<26); //green
	GPIOB_PDDR|=(1<<21); //blue
	// Turn off the LEDs
    LED_Off();
}

void LED_On (unsigned char color)
{
	// set the appropriate color
	// you need to drive the appropriate pin OFF to turn on the LED
	switch(color){
		case 'R':
			GPIOB_PCOR=(1<<22);//red
			break;
		
		case 'G':
			GPIOE_PCOR=(1<<26);//green
			break;
		
		case 'B':
			GPIOB_PCOR=(1<<21);//blue
			break;
		
		case 'C':
			GPIOE_PCOR=(1<<26);//cyan
			GPIOB_PCOR=(1<<21);
			break;
		
		case 'M':
			GPIOB_PCOR=(1<<22);//magenta
			GPIOB_PCOR=(1<<21);
			break;
		
		case 'Y':
			GPIOB_PCOR=(1<<22);//yellow
			GPIOE_PCOR=(1<<26);
			break;
		
		case 'W':
			GPIOB_PCOR=(1<<22);//white
			GPIOE_PCOR=(1<<26);
			GPIOB_PCOR=(1<<21);
			break;
		
		default:
			GPIOB_PCOR=(1<<22);//white
			GPIOE_PCOR=(1<<26);
			GPIOB_PCOR=(1<<21);
	}
}
