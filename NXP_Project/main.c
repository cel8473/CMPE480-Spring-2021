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

	// Print welcome over serial
	uart0_put("Running... \n\r");
	
	//Bluetooth is in Lab 4
	//Camera is in Lab 5 PT3
	int left = -1;
	int right = -1;
	int middle = -1;
	for(;;){
		// Find edges of data frmo the camera
		left = edgeFinder(line, 1);
		right = edgeFinder(line, 0);
		// Center of camera data
		middle = find_center(left,right);
		
		// Adjust the wheel towards center 
		int sum  = sum_line(line);
		wheel_adjust(middle, sum);
		
		if(PRINT)
		{
			sprintf(str,"%i : %i -- %i, sum: %i \n\r", left, right, middle, sum);
			
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

