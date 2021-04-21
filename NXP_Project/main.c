/*
 * Main module for testing the PWM Code for the K64F
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "MK64F12.h"
#include "./modules/uart.h"
#include "pwm.h"
#include "led.h"
#include "camera.h"
#include "motor.h"
#include "./modules/gpio.h"

#define DESIRED (53)
#define PRINT (1) //This is commented out currently
#define BLUETOOTH (0)
#define BUTTON (0)
#define CHAR_COUNT (80)
#define FAST_AF (70)
#define	MEDIUM (FAST_AF-10)
#define SLOW (FAST_AF-20)
#define MINUS_TURN (15)
// line stores the current array of camera data
extern uint16_t line[128];

// These variables are for streaming the camera
//	 data over UART
extern int debugcamdata;
extern int capcnt;
extern char str[100];
static int mode;

void delay(int del);
void put3(char *ptr_str);
void button(int speed_mode);

int main(void){
	// Initialize UART and PWM
  uart0_init();
	uart3_init(); //Uart for Bluetooth module and PuTTy
	init_motor();
	init_camera();
	LED_Init();
	Switch2_Init();
	
	char phone_buffer[CHAR_COUNT];
	int phone_counter = 0;
	if(PRINT){
		sprintf(str, "Running... Print: %i\n\r", PRINT);
		uart0_put(str);
	}
	//Bluetooth is in Lab 4
	//Camera is in Lab 5 PT3
	int left = -1;
	int right = -1;
	int middle = -1;
	//	turn turnOld error errorOld1 errodOld2 Kp	 Ki	 Kd	Sum
	PID_T control = {0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0, 0, 0};
	mode = 0;
	button(mode);
	for(;;){
		// Find edges of data frmo the camera
		left = edgeFinder(line, 1);
		right = edgeFinder(line, 0);
		// Center of camera data
		middle = find_center(left,right);
		//Find how many bits out of 128 are high
		control.sum = sum_line(line);
		turn_amount(middle, control);
		if(BUTTON){
			if(Switch2_Pressed()){
				button(mode);
			}
		}
		if(BLUETOOTH)
		{
			if ((UART3_S1 & ( 1 << UART_S1_RDRF_SHIFT )) != 0 )
			{
				char ch =(char) UART3_D;
				
				if (ch == '\n')
				{
					phone_buffer[phone_counter++] = '\0';
					phone_counter = 0;
					
					char *end_ptr;
					control.Kp = strtod(phone_buffer,&end_ptr);
					control.Ki = strtod(end_ptr, &end_ptr);
					control.Kd = strtod(end_ptr, &end_ptr);
					set_turn_speed(strtod(end_ptr, &end_ptr));
					set_straight_speed(strtod(end_ptr, &end_ptr));
					
					sprintf(str,"kp = %f, ki = %f, kd = %f, ts = %f, ss = %f \n",
								control.Kp, control.Ki, control.Kd, get_turn_speed(), get_straight_speed());
					put3(str);
					for(int i = 0; i < CHAR_COUNT; i++)
					{
						phone_buffer[i] = '\0';
					}
				}
				else
				{
					phone_buffer[phone_counter++] = ch;
					phone_buffer[phone_counter] = '\0';
					if(phone_counter == CHAR_COUNT)
					{
						phone_buffer[CHAR_COUNT-1] = '\0';
					}
				}
			}
		}
			
		if(PRINT){
			// Print welcome over serial
			sprintf(str,"%f\n\r", control.turnAmt);
			uart0_put(str);
			//delay(50);
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

void put3(char *ptr_str)
{
	while(*ptr_str)
		uart3_putchar(*ptr_str++);
}

void button(int speed_mode){
	switch(mode){
		case 0: //I am fast af boi
			LED_Off();
			LED_On('R');
			set_straight_speed(FAST_AF);
			set_turn_speed(FAST_AF - MINUS_TURN);
			mode++;
			break;
		case 1: //Medium pace
			LED_Off();
			LED_On('Y');
			set_straight_speed(MEDIUM);
			set_turn_speed(MEDIUM - MINUS_TURN);
			mode++;
			break;
		case 2: //We really need a time on the board
			LED_Off();
			LED_On('G');
			set_straight_speed(SLOW);
			set_turn_speed(SLOW - MINUS_TURN);
			mode=0;
			break;
		default:
			LED_Off();
			LED_On('R');
			break;
	}
	delay(5);
}
