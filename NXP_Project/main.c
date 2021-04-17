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

#define DESIRED (53)
#define PRINT (0)
// line stores the current array of camera data
extern uint16_t line[128];

// These variables are for streaming the camera
//	 data over UART
extern int debugcamdata;
extern int capcnt;
extern char str[100];

void delay(int del);

int main(void){
	// Initialize UART and PWM
  uart0_init();
	uart3_init(); //Uart for Bluetooth module and PuTTy
	init_motor();
	init_camera();
	
	if(PRINT){
		sprintf(str, "Running... Print: %i\n\r", PRINT);
		uart0_put(str);
	}
	//Bluetooth is in Lab 4
	//Camera is in Lab 5 PT3
	int left = -1;
	int right = -1;
	int middle = -1;
	//			turn turnOld error errorOld1 errodOld2 Kp		Ki	Kd	Sum
	struct PID control = {0.0, 0.0, 0.0, 0.0, 0.0, 6, 0, 0.6, 0};
	for(;;){
		// Find edges of data frmo the camera
		left = edgeFinder(line, 1);
		right = edgeFinder(line, 0);
		// Center of camera data
		middle = find_center(left,right);
		//Find how many bits out of 128 are high
		//int sum = sum_line(line);
		control.sum = sum_line(line);
		turn_amount(middle, control);
		if(PRINT){
			// Print welcome over serial
			sprintf(str,"%i\n\r", middle);
			uart0_put(str);
			delay(50);
		}
	}
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
