  /*
 * File:        gpio.c
 * Purpose:     Provide GPIO routines
 *
 * Notes:		
 *
 */
#include "MK64F12.h"
#include "gpio.h"

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
	GPIOC_PDDR|=(0<<6);
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
	GPIOA_PDDR|=(0<<4);
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

/* Set up pins for GPIO
* 	PTB9 		- camera clk
*		PTB23		- camera SI
*		PTB22		- red LED
*/
void init_GPIO(void){
	// Enable LED and GPIO so we can see results
	//PORTB/E Clock is Enabled
	//PTB21/22 are Configured
	//PTBE26 is Configured
	LED_Init(); //Red Green and Blue LEDS
	PORTB_PCR18=PORT_PCR_MUX(3);
	PORTB_PCR9=PORT_PCR_MUX(1);
	PORTB_PCR9|=PORT_PCR_DSE_MASK;
	PORTB_PCR23=PORT_PCR_MUX(1);
	PORTB_PCR23|=PORT_PCR_DSE_MASK;
	GPIOB_PDDR |= (1<<9 | 1<<23);
	return;
}

void HBridge_init(){
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	GPIOB_PDDR |= (1 << 2) | (1 << 3);
	GPIOB_PSOR |= (1 << 2) | (1 << 3);
}
