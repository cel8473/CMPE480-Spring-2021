 /*
 * File:        adc.c
 * Purpose:     Provide ADC routines
 *
 * Notes:		
 *
 */
 
 #include "MK64F12.h"
 
 #include "adc.h"
 
void ADC1_INIT(void) {
    unsigned int calib;
 
    // Turn on ADC1
		SIM_SCGC3|=SIM_SCGC3_ADC1_MASK;
 
    // Configure CFG Registers
    // Configure ADC to divide 50 MHz down to 6.25 MHz AD Clock, 16-bit single ended
    ADC1_CFG1=ADC_CFG1_ADIV(3);
		ADC1_CFG1=ADC_CFG1_MODE(3);
 
    // Do ADC Calibration for Singled Ended ADC. Do not touch.
    ADC1_SC3 = ADC_SC3_CAL_MASK;
    while ( (ADC1_SC3 & ADC_SC3_CAL_MASK) != 0 );
    calib = ADC1_CLP0;
    calib += ADC1_CLP1;
    calib += ADC1_CLP2;
    calib += ADC1_CLP3;
    calib += ADC1_CLP4;
    calib += ADC1_CLPS;
    calib = calib >> 1;
    calib |= 0x8000;
    ADC1_PG = calib;
 
    // Configure SC registers.
    // Select hardware trigger.
    ADC1_SC2 = ADC_SC2_ADTRG_MASK;
 
    // Configure SC1A register.
    // Select ADC Channel and enable interrupts. Use ADC1 channel DADP3  in single ended mode.
		ADC1_SC1A = 0;
		ADC1_SC1A |= (0 <<ADC_SC1_DIFF_SHIFT);
    ADC1_SC1A |= ADC_SC1_ADCH(3);
		ADC1_SC1A |= ADC_SC1_AIEN_MASK;
		
 
    // Enable NVIC interrupt
    NVIC_EnableIRQ(ADC1_IRQn);
}

// ADC1 Conversion Complete ISR
void ADC1_IRQHandler(void) {
    // Read the result (upper 12-bits). This also clears the Conversion complete flag.
    unsigned short i = (unsigned short)ADC1_RA >> 4;

    //Set DAC output value (12bit)
    DAC0_DAT0H=(i >> 8);
		DAC0_DAT0L= i;
}