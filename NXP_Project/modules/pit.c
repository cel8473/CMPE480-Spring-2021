  /*
 * File:        pit.c
 * Purpose:     Provide PIT routines
 *
 * Notes:		
 *
 */

#include "MK64F12.h"
#include "pit.h"

// Default System clock value
// period = 1/20485760  = 4.8814395e-8
#define DEFAULT_SYSTEM_CLOCK 20485760u 

// Integration time (seconds)
// Determines how high the camera values are
// Don't exceed 100ms or the caps will saturate
// Must be above 1.25 ms based on camera clk 
//	(camera clk is the mod value set in FTM2)
#define INTEGRATION_TIME .0075f

// These variables are for streaming the camera
//	 data over UART
int debugcamdata = 1;
int capcnt = 0;
char str[100];

/* Initialization of PIT timer to control 
* 		integration period
*/
void init_PIT(void){
	// Setup periodic interrupt timer (PIT)
	// Enable clock for timers
	//INSERT CODE HERE
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Enable timers to continue in debug mode
	//INSERT CODE HERE // In case you need to debug
  PIT_MCR &= ~(PIT_MCR_FRZ_MASK);
  
	// PIT clock frequency is the system clock
	// Load the value that the timer will count down from
	//INSERT CODE HERE
	PIT_LDVAL0 = DEFAULT_SYSTEM_CLOCK*INTEGRATION_TIME;
	// Enable timer interrupts
	//INSERT CODE HERE
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;
	// Enable the timer
	//INSERT CODE HERE
  PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
	// Clear interrupt flag
	//INSERT CODE HERE
  PIT_TCTRL0 |= PIT_TFLG_TIF_MASK;
	// Enable PIT interrupt in the interrupt controller
	//INSERT CODE HERE
	PIT_MCR &= ~(PIT_MCR_MDIS_MASK); 
  NVIC_EnableIRQ(PIT0_IRQn); 
	return;
}


/* PIT0 determines the integration period
*		When it overflows, it triggers the clock logic from
*		FTM2. Note the requirement to set the MOD register
* 	to reset the FTM counter because the FTM counter is 
*		always counting, I am just enabling/disabling FTM2 
*		interrupts to control when the line capture occurs
*/
void PIT0_IRQHandler(void){
	if (debugcamdata) {
		// Increment capture counter so that we can only 
		//	send line data once every ~2 seconds
		capcnt += 1;
	}
	// Clear interrupt
	//INSERT CODE HERE
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
	
	// Setting mod resets the FTM counter
	//INSERT CODE HERE
	FTM2->MOD = (DEFAULT_SYSTEM_CLOCK)/100000;
	
	// Enable FTM2 interrupts (camera)
	//INSERT CODE HERE
	FTM2_SC|=FTM_SC_TOIE_MASK;
	
	return;
}