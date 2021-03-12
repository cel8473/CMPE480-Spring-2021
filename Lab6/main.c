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

	// Print welcome over serial
	uart0_put("Running... \n\r");
	
	//Part 1 - UNCOMMENT THIS
	// Generate 20% duty cycle at 10kHz
	// INSERT CODE HERE
	//FTM0_set_duty_cycle(20, 10000, 1);
	
	//for(;;) ;  //then loop forever
	
	
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
	
	//Enable clocks on Port D
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	//Configure the Signal Mux for the Port D GPIO Pins
	PORTD_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //A
	PORTD_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //B
	PORTD_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //C
	PORTD_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D
	//Configure the GPIO Pins for Output
	GPIOD_PDDR|=(1<<0); //A
	GPIOD_PDDR|=(1<<1); //B
	GPIOD_PDDR|=(1<<2); //C
	GPIOD_PDDR|=(1<<3); //D
	
	int forward = 1;
	int phase = 0;
	int true = 1;
	
	while(true){
		//Turn off all coils. Set GPIO pins to 0
		GPIOD_PSOR|=(1<<0); //A
		GPIOD_PSOR|=(1<<1); //B
		GPIOD_PSOR|=(1<<2); //C 
		GPIOD_PSOR|=(1<<3); //D
		
		//Set one pin high at a time
		if(forward){
			if(phase == 0){
				/*turn on coil A*/
				GPIOD_PCOR=(1<<0);
				phase++;} //A, 1a
			else if(phase == 1){
				/*turn on coil B*/
				GPIOD_PCOR=(1<<1);
				phase++;} //B, 2a
			else if(phase == 2){
				/*turn on coil C*/
				GPIOD_PCOR=(1<<2);
				phase++;} //C, 1b
			else{
				/*turn on coil D*/
				GPIOD_PCOR=(1<<3);
				phase++;} //D, 2b
		}
		else{//reverse
				if(phase == 0){
				/*turn on coil D*/
				GPIOD_PCOR=(1<<3);
				phase++;} //D, 2b
			else if(phase == 1){
				/*turn on coil C*/
				GPIOD_PCOR=(1<<2);
				phase++;} //C, 1b
			else if(phase == 2){
				/*turn on coil B*/
				GPIOD_PCOR=(1<<1);
				phase++;} //B, 2a
			else{
				/*turn on coil A*/
				GPIOD_PCOR=(1<<0);
				phase++;} //A, 1a
		}
	}
	return 0;
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
