/*
 * Main module for Lab 8
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 */
 
#include <stdio.h>
#include "MK64F12.h"

#include "uart.h"
#include "adc.h"
#include "pdb.h"
#include "dac.h"

/*From clock setup 0 in system_MK64f12.c*/
#define DEFAULT_SYSTEM_CLOCK 20485760u /* Default System clock value */

void delay(int del);
void initFTM(void);
void FTM0_IRQHandler(void);

static int timer = 0;
#define THRESHOLD_VOLTAGE (2.4)
int main(void) {
	// Initialize UART and PWM
	
  
  uart0_init();
  DAC0_INIT();
  ADC1_INIT();
  PDB_INIT();
  initFTM();
	char str[100];
	
  PDB0_SC |= PDB_SC_SWTRIG_MASK;
  double voltage;
  double freq;
  
  int rise = 0 ;
  
	for(;;)
  {
    // Convert data from ADC
    // Keep count of two voltages
    
    voltage = (ADC1_RA * 3.3) /65535.0 ;
    //sprintf(str,"My voltage is %f \n\r", voltage);
    //uart0_put(str);
//    sprintf(str,"My timer is %d \n\r", timer);
//    uart0_put(str);
      if(voltage > THRESHOLD_VOLTAGE)
      {
        if(rise == 0)
        {
          timer = 0;
          rise = 1;
          while(voltage > THRESHOLD_VOLTAGE)
          {
            voltage = (ADC1_RA * 3.3) /65535.0 ;
          }
          
        }
        
        else if(rise == 1)
        {
          //sprintf(str,"My timer is %d \n\r", timer);
          //uart0_put(str);
          freq = (1.0 / timer) * 1000.0 * 60.0;
          sprintf(str,"My Heart Rate is %f BPM\n\r", freq);
          uart0_put(str);
          while(voltage > THRESHOLD_VOLTAGE)
          {
            voltage = (ADC1_RA * 3.3) /65535.0 ;
          }
          rise = 0;
        }
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

void initFTM(void){
	//Enable clock for FTM module (use FTM0)
	SIM_SCGC6|=SIM_SCGC6_FTM0_MASK;

	//turn off FTM Mode to  write protection;
	FTM0_MODE|=FTM_MODE_WPDIS_MASK;

	//divide the input clock down by 128,
	FTM0_SC|=FTM_SC_PS(7);

	//reset the counter to zero
	FTM0_CNT=0;

	//Set the overflow rate
	//(Sysclock/128)- clock after prescaler
	//(Sysclock/128)/1000- slow down by a factor of 1000 to go from
	//Mhz to Khz, then 1/KHz = msec
	//Every 1msec, the FTM counter will set the overflow flag (TOF) and
	FTM0->MOD = (DEFAULT_SYSTEM_CLOCK/(1<<7))/1000;

	//Select the System Clock
	FTM0_SC|=FTM_SC_CLKS(1);

	//Enable the interrupt mask. Timer overflow Interrupt enable
	FTM0_SC|=FTM_SC_TOIE_MASK;
  NVIC_EnableIRQ(FTM0_IRQn);
	return;
}

void FTM0_IRQHandler(void){ //For FTM globTimerEn

	FTM0_SC&= ~(FTM_SC_TOF_MASK); //clear interrupt in FTM0 SC
	timer++;
	return;
}
