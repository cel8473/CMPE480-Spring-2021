  /*
 * File:        ADC.c
 * Purpose:     Provide ADC routines 
 *
 * Notes:		
 *
 */
#include "MK64F12.h"
#include "adc.h"



// ADC0VAL holds the current ADC value
uint16_t ADC0VAL;

/* Set up ADC for capturing camera data */
void init_ADC0(void) {
  unsigned int calib;
  // Turn on ADC0
  //INSERT CODE HERE
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
  // Single ended 16 bit conversion, no clock divider
  //INSERT CODE HERE
  ADC0_CFG1 |= ADC_CFG1_MODE(3); // Set mode to 3 -- 16-bit conversion
	ADC0_CFG1 &= ~(ADC_CFG1_ADIV_MASK);

  // Do ADC Calibration for Singled Ended ADC. Do not touch.
  ADC0_SC3 = ADC_SC3_CAL_MASK;
  while ( (ADC0_SC3 & ADC_SC3_CAL_MASK) != 0 );
  calib = ADC0_CLP0; calib += ADC0_CLP1; calib += ADC0_CLP2;
  calib += ADC0_CLP3; calib += ADC0_CLP4; calib += ADC0_CLPS;
  calib = calib >> 1; calib |= 0x8000;
  ADC0_PG = calib;
  
  // Select hardware trigger.
  //INSERT CODE HERE
  ADC0_SC2 |= ADC_SC2_ADTRG_MASK; // Set ADTRG bit to 1
  // Set to single ended mode	
  //INSERT CODE HERE
  ADC0_SC1A &= ~(ADC_SC1_DIFF_MASK); // Set DIFF bit to 0
  // Set up FTM2 trigger on ADC0
  SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(10); // FTM2 select
	SIM_SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK; // Alternative trigger en.
	SIM_SOPT7 &= ~(SIM_SOPT7_ADC0PRETRGSEL_MASK); // Pretrigger A
  
	// Enable NVIC interrupt
  //INSERT CODE HERE
	ADC0_SC1A |= ADC_SC1_AIEN_MASK;
	ADC0_SC1A &= ~(ADC_SC1_ADCH_MASK);
  NVIC_EnableIRQ(ADC0_IRQn);
}

/* ADC0 Conversion Complete ISR  */
void ADC0_IRQHandler(void) {
	// Reading ADC0_RA clears the conversion complete flag
	ADC0VAL = (uint16_t)ADC0_RA;
}

uint16_t get_ADC0_VAL(void)
{
  return ADC0VAL;
}

