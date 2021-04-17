  /*
 * File:        ftm.c
 * Purpose:     Provide FTM routines 
 *
 * Notes:		
 *
 */

#include "MK64F12.h"
#include "ftm.h"
#include "adc.h"
#define DEFAULT_SYSTEM_CLOCK    20485760u 
#define FTM0_MOD_VALUE          (DEFAULT_SYSTEM_CLOCK/10000)
#define FTM3_MOD_VALUE          ((DEFAULT_SYSTEM_CLOCK/8)/50)

// Pixel counter for camera logic
// Starts at -2 so that the SI pulse occurs
//		ADC reads start
int pixcnt = -2;

// clkval toggles with each FTM interrupt
int clkval = 0;

// line stores the current array of camera data
uint16_t line[128];

/* Initialization of FTM2 for camera */
void init_FTM2(){
	// Enable clock
	//INSERT CODE HERE
	SIM_SCGC6|=SIM_SCGC6_FTM2_MASK;

	// Disable Write Protection
	//INSERT CODE HERE
	FTM2_MODE|=FTM_MODE_WPDIS_MASK;
	
	// Set output to '1' on init
	//INSERT CODE HERE
  FTM2_OUTINIT|=FTM_OUTINIT_CH0OI_MASK;
	FTM2_MODE|=FTM_MODE_INIT_MASK;
	
	// Initialize the CNT to 0 before writing to MOD
	//INSERT CODE HERE
	FTM2_CNT = FTM_CNT_COUNT(0);
	
	// Set the Counter Initial Value to 0
	//INSERT CODE HERE
	FTM2_CNTIN = FTM_CNTIN_INIT(0);
	
	// Set the period (~10us)
	//INSERT CODE HERE
	FTM2_MOD = (DEFAULT_SYSTEM_CLOCK)/100000;
	
	// 50% duty
	//INSERT CODE HERE
	FTM2_C0V |= FTM_CnV_VAL(DEFAULT_SYSTEM_CLOCK/200000); 
	
	// Set edge-aligned mode
	//INSERT CODE HERE
	FTM2_C0SC|= FTM_CnSC_MSB_MASK; //MSA is don't care
	
	// Enable High-true pulses
	// ELSB = 1, ELSA = 0
	//INSERT CODE HERE
	FTM2_C0SC|= FTM_CnSC_ELSB_MASK;
	FTM2_C0SC&= ~(FTM_CnSC_ELSA_MASK);
	
	// Enable hardware trigger from FTM2
	//INSERT CODE HERE
	FTM2_EXTTRIG|=FTM_EXTTRIG_INITTRIGEN_MASK;
	
	// Don't enable interrupts yet (disable)
	//INSERT CODE HERE
	FTM2_SC&= ~(FTM_SC_TOIE_MASK);
	
	// No prescalar, system clock
	//INSERT CODE HERE
	FTM2_SC&= ~(FTM_SC_PS_MASK);
	FTM2_SC|= FTM_SC_CLKS(1);
	
	// Set up interrupt
	//INSERT CODE HERE
	FTM2_SC|=FTM_SC_TOIE_MASK;
	NVIC_EnableIRQ(FTM2_IRQn);
	return;
}

/* 
* FTM2 handles the camera driving logic
*	This ISR gets called once every integration period
*		by the periodic interrupt timer 0 (PIT0)
*	When it is triggered it gives the SI pulse,
*		toggles clk for 128 cycles, and stores the line
*		data from the ADC into the line variable
*/
void FTM2_IRQHandler(void){ //For FTM timer
	// Clear interrupt
	//INSERT CODE HERE
	int readInt = (FTM2_SC & FTM_SC_TOF_MASK);
  FTM2_SC &= ~(FTM_SC_TOF_MASK);
	// Toggle clk
	//INSERT CODE HERE
	GPIOB_PTOR=(1<<9);
	if(clkval){clkval = 0;}
	else{clkval = 1;}
	
	// Line capture logic
	if((pixcnt >= 2) && (pixcnt < 256)) {
		if (!clkval) {	// check for falling edge
			// ADC read (note that integer division is 
			//  occurring here for indexing the array)
			line[pixcnt/2] = get_ADC0_VAL();
		}
		pixcnt += 1;
	} else if (pixcnt < 2) {
		if (pixcnt == -1) {
			GPIOB_PSOR |= (1 << 23); // SI = 1
		} else if (pixcnt == 1) {
			GPIOB_PCOR |= (1 << 23); // SI = 0
			// ADC read
			line[0] = get_ADC0_VAL();
		} 
		pixcnt += 1;
	} else {
		GPIOB_PCOR |= (1 << 9); // CLK = 0
		clkval = 0; // make sure clock variable = 0
		pixcnt = -2; // reset counter
		// Disable FTM2 interrupts (until PIT0 overflows
		//   again and triggers another line capture)
		//INSERT CODE HERE
		FTM2_SC &= ~(FTM_SC_TOIE_MASK);
	
	}
	return;
}

/*From clock setup 0 in system_MK64f12.c*/


/*
 * Change the motor duty cycle and frequency
 *
 * @param duty_cycle (0 to 100)
 * @param frequency (~1000 Hz to 20000 Hz)
 * @param dir: 1 for pin C4 active, else pin C3 active 
 */
void FTM0_set_duty_cycle_left(int duty_cycle, unsigned int frequency)
{
	// Calculate the new cutoff value
	uint16_t mod = (uint16_t) (((DEFAULT_SYSTEM_CLOCK / frequency) * duty_cycle) / 100);
  int dir = duty_cycle >= 0 ? 0 : 1;
	// Set outputs 
	if(dir) {
			FTM0_C1V = mod;
			FTM0_C0V = 0;
	} else {
			FTM0_C0V = mod;
			FTM0_C1V = 0;
	}

	// Update the clock to the new frequency
	FTM0_MOD = (DEFAULT_SYSTEM_CLOCK / frequency);
}

void FTM0_set_duty_cycle_right(int duty_cycle, unsigned int frequency)
{
	// Calculate the new cutoff value
	uint16_t mod = (uint16_t) (((DEFAULT_SYSTEM_CLOCK / frequency) * duty_cycle) / 100);
  int dir = duty_cycle >= 0 ? 0 : 1;
	// Set outputs 
	if(dir) {
	    FTM0_C3V = mod;
	    FTM0_C2V = 0;
	} else {
	    FTM0_C2V = mod;
	    FTM0_C3V = 0;
	}

	// Update the clock to the new frequency
	FTM0_MOD = (DEFAULT_SYSTEM_CLOCK / frequency);
}

/*
 * Initialize the FlexTimer for PWM
 */
void FTM0_init()
{
	// 12.2.13 Enable the clock to the FTM0 Module
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	
	// Enable clock on PORT C so it can output the PWM signals
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// 11.4.1 Route the output of FTM channel 0 to the pins
	// Use drive strength enable flag to high drive strength
	PORTC_PCR1 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; //Ch0
  PORTC_PCR2 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; //Ch1
  PORTC_PCR3 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; //Ch2
  PORTC_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; //Ch3
	
	// 39.3.10 Disable Write Protection
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;
	
	// 39.3.4 FTM Counter Value
	// Initialize the CNT to 0 before writing to MOD
	FTM0_CNT = 0;
	
	// 39.3.8 Set the Counter Initial Value to 0
	FTM0_CNTIN = 0;
	
	// 39.3.5 Set the Modulo resister
	FTM0_MOD = FTM0_MOD_VALUE;

	// 39.3.6 Set the Status and Control of both channels
	// Used to configure mode, edge and level selection
	// See Table 39-67,  Edge-aligned PWM, High-true pulses (clear out on match)
	FTM0_C3SC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM0_C3SC &= ~FTM_CnSC_ELSA_MASK;
	
	// See Table 39-67,  Edge-aligned PWM, Low-true pulses (clear out on match)
	FTM0_C2SC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM0_C2SC &= ~FTM_CnSC_ELSA_MASK;
	
	// See Table 39-67,  Edge-aligned PWM, Low-true pulses (clear out on match)
	FTM0_C1SC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM0_C1SC &= ~FTM_CnSC_ELSA_MASK;
	
	// See Table 39-67,  Edge-aligned PWM, Low-true pulses (clear out on match)
	FTM0_C0SC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK;
	
	// 39.3.3 FTM Setup
	// Set prescale value to 1 
	// Chose system clock source
	// Timer Overflow Interrupt Enable
	FTM0_SC = FTM_SC_PS(0) | FTM_SC_CLKS(1); 
}

/*
 * Change the motor duty cycle and frequency
 *
 * @param duty_cycle (0 to 100)
 * @param frequency (~1000 Hz to 20000 Hz)
 * @param dir: 1 for pin D4 active, else pin D3 active 
 */
void FTM3_set_duty_cycle(double duty_cycle, unsigned int frequency)
{
	// Calculate the new cutoff value
	uint16_t mod = (uint16_t) (((double)(DEFAULT_SYSTEM_CLOCK / frequency) * duty_cycle) / 800.0);
	// Set outputs 
	FTM3_C4V = mod; 

	// Update the clock to the new frequency
	FTM3_MOD = (DEFAULT_SYSTEM_CLOCK / frequency)/8;
}

/*
 * Initialize the FlexTimer for PWM
 */
void FTM3_init()
{
	// 12.2.13 Enable the clock to the FTM3 Module
	SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;
	
	// Enable clock on PORT D so it can output the PWM signals
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// 11.4.1 Route the output of FTM channel 0 to the pins
	// Use drive strength enable flag to high drive strength
  PORTC_PCR8 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK; //Ch4
	
	// 39.3.10 Disable Write Protection
	FTM3_MODE |= FTM_MODE_WPDIS_MASK;
	
	// 39.3.4 FTM Counter Value
	// Initialize the CNT to 0 before writing to MOD
	FTM3_CNT = 0;
	
	// 39.3.8 Set the Counter Initial Value to 0
	FTM3_CNTIN = 0;
	
	// 39.3.5 Set the Modulo resister
	FTM3_MOD = FTM3_MOD_VALUE;

	// 39.3.6 Set the Status and Control of both channels
	// Used to configure mode, edge and level selection
	// See Table 39-67,  Edge-aligned PWM, High-true pulses (clear out on match)
	FTM3_C4SC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM3_C4SC &= ~FTM_CnSC_ELSA_MASK;
	
	// 39.3.3 FTM Setup
	// Set prescale value to 1 
	// Chose system clock source
	// Timer Overflow Interrupt Enable
	FTM3_SC = FTM_SC_PS(3) | FTM_SC_CLKS(1);
}


