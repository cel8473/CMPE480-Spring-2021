/* 
Title: Main
Purpose: 
Name: Chris Larson  
Date: 1/29/21
*/

// NOTE: The LEDs and the Switches on the K64 are all active LOW

#include "MK64F12.h"                    // Device header
#include "Led.h"
#include "Switch.h"

void delay(void);

void delay(void)
{
	volatile long j = 0;
	for ( j=0; j < 2000000; j++)
		;
}

int main(void)
{
	// FYI:
	// PCOR Turns Pin to ZERO  (clear)
	// PSOR Turns Pin to ONE   (sets)

	//Initialize any supporting variables
	 int counter = 1;
	
	// Initialize buttons and LEDs
	LED_Init();
	Switch2_Init();
	
	for(;;)
	{
		//Turn on an LED configuration when switch2 is pressed
		if(Switch2_Pressed())
		{
			// first time pressed cycle R,G,B - delay() in between colors
			// use LED_On()
			if(counter == 1){
				LED_On('R');
				delay();
				LED_Off();
				LED_On('G');
				delay();
				LED_Off();
				LED_On('B');
				delay();
				LED_Off();
				counter++;
			}
			// second time pressed cycle C,M,Y- delay() in between colors
			// use LED_On()
			else if(counter == 2){
				LED_On('C');
				delay();
				LED_Off();
				LED_On('M');
				delay();
				LED_Off();
				LED_On('Y');
				delay();
				LED_Off();
				counter++;
			}
			// third time pressed W		- delay() in between colors
			// use LED_On()
			else if(counter == 3){
				LED_On('W');
				delay();
				LED_Off();
				counter = 1;
			}
		}
		// Turn off LED when no switch is pressed
		else
		{
			LED_Off();
		}
	}
}
