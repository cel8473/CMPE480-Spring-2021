/*
 * Main module for testing the PWM Code for the K64F
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 */
 
#include <stdio.h>
#include "MK64F12.h"
#include "./modules/uart.h"
#include "pwm.h"
#include "led.h"
#include "camera.h"
#include "motor.h"


// line stores the current array of camera data
extern uint16_t line[128];

// These variables are for streaming the camera
//	 data over UART
extern int debugcamdata;
extern int capcnt;
extern char str[100];

void delay(int del);


int main(void) {
	// Initialize UART and PWM
  	uart0_init();
	uart3_init(); //Uart for Bluetooth module and PuTTy
	HBridge_init(); //H-bridge that amplifies the K64 power
	init_motor();
	init_camera();

	// Print welcome over serial
	uart0_put("Running... \n\r");
	
	//Bluetooth is in Lab 4
	//Camera is in Lab 5 PT3
	int left = -1;
	int right = -1;
	for(;;){
		left = edgeFinder(line, 1);
		right = edgeFinder(line, 0);
		sprintf(str,"%i : %i\n\r", left, right);
		uart0_put(str);
		delay(100);
	}
	//FTM3_set_duty_cycle(5.6, 50); //5.2-8.6
	//FTM0_set_duty_cycle(50, 10000, 1);
	#if 0
	for(;;) {
		if (debugcamdata) {
			// Every 2 seconds
			//if (capcnt >= (2/INTEGRATION_TIME)) {
			if (capcnt >= (500)) {
				GPIOB_PCOR |= (1 << 22);
				// send the array over uart
				sprintf(str,"%i\n\r",-1); // start value
				uart0_put(str);
				for (int i = 0; i < 127; i++) {
					sprintf(str,"%i\n", line[i]);
					uart0_put(str);
				}
				sprintf(str,"%i\n\r",-2); // end value
				uart0_put(str);
				capcnt = 0;
				GPIOB_PSOR |= (1 << 22);
			}
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

