/*
    Title: CMPE460 Lab4 - Bluetooth Low Energy 
    Description: UART driver to send and recieve characters
    Authors: Brandon Key
    Date: 1/29/2020
*/


#include "MK64F12.h"
#include "uart.h"         // Main serial funcitonality
#include "led.h"          // OPTIONAL: Could be useful to display info/warnings
#include <stdio.h>        // General funcitonality
#include <string.h>       // Useful for string operations and memset

#define CHAR_COUNT 80     // Size of a console line

int main()
{
    // Initialize LEDs (optional)
		LED_Init();
    
    // Initialize UART0 and UART3
		uart_init();
    
    // Display startup message
		uart0_put("\r\n Welcome to the jungle: PC\n\r");
		uart3_put("\n Welcome to the jungle: PHONE\n");
    // Declare and reset buffers and print prompts
		//LED Part 1 While Loop
    /*while(1){
			uart3_put("Please enter a number to change the LED: \n");
			uart3_put("0->Off, 1->RED, 2->BLUE, 3->GREEN \n");
			uint8_t data = uart3_getchar();
			switch(data){
				case('0'):
					LED_Off();
					uart3_put("Off\n");
					break;
				case('1'):
					LED_Off();
					LED_On('R');
					uart3_put("Red\n");
					break;
				case('2'):
					LED_Off();
					LED_On('B');
					uart3_put("Blue\n");
					break;
				case('3'):
					LED_Off();
					LED_On('G');
					uart3_put("Green\n");
					break;
			}
		}*/
		char ptr_str_PC[CHAR_COUNT];
		char ptr_str_Phone[CHAR_COUNT];
		int PC_count = 0;
		int Phone_count = 0;
		uart0_put("PC> ");
		uart3_put("Phone> ");
    /* Control loop */
    while(1){
        /* Check if there is data available from the USB */
        if ((UART0_S1&(1<<UART_S1_RDRF_SHIFT))){ /* Check the RDRF flag for the USB UART */
            /* NOTE: Never block in this statement. */
            
            // Retrieve the character
            char ch = uart0_getchar(); // CHANGE THIS
            /* If statements to handle the character */
            if (ch == '\n' || ch == '\r') {
                /** 
                 * Enter pressed. Phone terminal may not \r or \n
                 * Manual entering of a character may be needed
                 * Some terminals enter \r then \n. Handle this.
                 */
                 
                /* Handle text coming from the PC */ 
                
                /*     ON THE PHONE SIDE    */
                // Clear current text
								uart3_put("\r");
                // Print the PC's message with a newline
								uart3_put("\nPC> ");
                uart3_put(ptr_str_PC);
								uart3_put("\n");
                // Restore the current text
                uart3_put("Phone> ");
                /*      ON THE PC SIDE      */
                // Clear buffer
								memset(ptr_str_PC, 0x00, CHAR_COUNT);
								PC_count=0;
                // Newline and prompt
                uart0_put("\r\nPC> ");
								
            } else if (ch == '\b' || ch == 0x7F){
                /* 
                 * Backspace detected. Some terminals don't use \b character
                 * Some enter DEL. Use an ASCII table & debugger to figure out 
                 * the correct character to check for
                */
                /* Handle backspace */
                // Check for buffer underflow
								if(PC_count>0){		
										uart0_put("\b \b");
										ptr_str_PC[PC_count-1]=0x00;
										PC_count--;
								}
								
            } else {
							//while(ch != 0xD && PC_count < CHAR_COUNT){
                /* Non-special character entered */
								uart0_putchar(ch);
                strncat(ptr_str_PC, &ch, 1);
								PC_count++;
                // Check for buffer overflow
            } 
        }
        
        /*  
            Repeat the logic for handling PC characters, for the Phone characters
        
            Separate non-blocking if statements function as different tasks
            This is a simple form of multi-tasking & task scheduling.
            Useful for simple polling opperations. 
            We will learn how to handle this more effciently in later labs. 
        */
				/* Check if there is data available from the USB */
        if ((UART3_S1&(1<<UART_S1_RDRF_SHIFT))){ /* Check the RDRF flag for the USB UART */
            /* NOTE: Never block in this statement. */
            
            // Retrieve the character
            char ch = uart3_getchar(); // CHANGE THIS
            /* If statements to handle the character */
            if (ch == '\n' /*|| ch == '\r'*/) {
                /** 
                 * Enter pressed. Phone terminal may not \r or \n
                 * Manual entering of a character may be needed
                 * Some terminals enter \r then \n. Handle this.
                 */
                 
                /* Handle text coming from the PC */ 
                
                /*     ON THE PHONE SIDE    */
                // Clear current text
								uart0_put("\b \b");
								uart0_put("\b \b");
								uart0_put("\b \b");
								uart0_put("\b \b");
								uart0_put("\r");
                // Print the PC's message with a newline
								uart0_put("Phone> ");
                uart0_put(ptr_str_Phone);
                // Restore the current text
								uart0_put("\r\nPC> ");
								uart0_put(ptr_str_PC);
                /*      ON THE PC SIDE      */
                // Clear buffer
								memset(ptr_str_Phone, 0x00, CHAR_COUNT);
								Phone_count=0;
                // Newline and prompt
                uart3_put("\nPhone> ");
								
            } else if (ch == '\b' || ch == 0x7F){
                /* 
                 * Backspace detected. Some terminals don't use \b character
                 * Some enter DEL. Use an ASCII table & debugger to figure out 
                 * the correct character to check for
                */
                /* Handle backspace */
                // Check for buffer underflow
								if(Phone_count>0){		
										uart3_put("\b \b");
										ptr_str_Phone[Phone_count-1]=0x00;
										Phone_count--;
								}
								
            } else {
							//while(ch != 0xD && PC_count < CHAR_COUNT){
                /* Non-special character entered */
								uart3_putchar(ch);
                strncat(ptr_str_Phone, &ch, 1);
								Phone_count++;
                // Check for buffer overflow
            } 
        }
    }   
}  
