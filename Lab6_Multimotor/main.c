/*
 * Main module for testing the PWM Code for the K64F
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 */
 
#include <stdio.h>
#include "MK64F12.h"
#include "uart.h"
#include "pwm.h"

void delay(int del);

int main(void) {
	// Initialize UART and PWM
	uart_init();
	FTM0_init();
	FTM3_init();

	// Print welcome over serial
	uart0_put("Running... \n\r");
	
	#if 1
	//Part 1 - UNCOMMENT THIS
	// Generate 20% duty cycle at 10kHz
	// INSERT CODE HERE
	FTM0_set_duty_cycle(50, 10000, 1);
	FTM3_set_duty_cycle(50, 50, 1);
	
	for(;;) ;  //then loop forever
	#endif
	#if 0
	// Part 2 - UNCOMMENT THIS
	for(;;)  //loop forever
	{
		//uint16_t dc = 0;
		uint16_t freq = 10000; // Frequency = 10 kHz 
		uint16_t dir = 0;
		//char c = 48;
		char cycle[10];
		int i=0;
		
		// 0 to 100% duty cycle in forward direction
		dir = 1;
		for (i=0; i<100; i++) {
			// INSERT CODE HERE
			FTM0_set_duty_cycle((unsigned int)i, freq, dir);
			sprintf(cycle, "%d\n\r", i);
			uart0_put(cycle);
			delay(10);
		}
		
		// 100% down to 0% duty cycle in the forward direction
		for (i=100; i>=0; i--) {
		  // INSERT CODE HERE
			FTM0_set_duty_cycle((unsigned int)i, freq, dir);
			sprintf(cycle, "%d\n\r", i);
			uart0_put(cycle);
			delay(10);
		}
		
		// 0 to 100% duty cycle in reverse direction
		dir = 0;
		for (i=0; i<100; i++) {
		  // INSERT CODE HERE
			FTM0_set_duty_cycle((unsigned int)i, freq, dir);
			sprintf(cycle, "%d\n\r", i);
			uart0_put(cycle);
			delay(10);
		}
		
		// 100% down to 0% duty cycle in the reverse direction
		for (i=100; i>=0; i--) {
		  // INSERT CODE HERE
			FTM0_set_duty_cycle((unsigned int)i, freq, dir);
			sprintf(cycle, "%d\n\r", i);
			uart0_put(cycle);
			delay(10);
		}

	}
	#endif
}


/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void delay(int del){
	volatile int i;
	for (i=0; i<del*50000; i++){
		// Do nothing
	}
}
