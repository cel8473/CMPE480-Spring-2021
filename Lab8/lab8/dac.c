 /*
 * File:        adc.c
 * Purpose:     Provide ADC routines
 *
 * Notes:		
 *
 */
 
 #include "MK64F12.h"
 
 #include "dac.h"
 
 void DAC0_INIT(void) {
    //enable DAC clock
    SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;
    DAC0_C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
    DAC0_C1 = 0;
}
 