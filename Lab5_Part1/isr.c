/*
 * isr.c
 */

#include "isr.h"
#include "MK64F12.h"
#include <stdio.h>
#include "uart.h"

//variables global to the IRQ handlers which dictates if globTimerEn is enabled &  globTimerEn counter
int globCounter = 0; //How long the button is pressed.
int globTimerEn = 0; //boolean for timer

void PDB0_IRQHandler(void){ //For PDB globTimerEn
	PDB0_SC&=(~PDB_SC_PDBIF_MASK); //clear interrupt in PDB0 SC
	GPIOB_PTOR|=(1<<22); //Toggle
	GPIOE_PTOR|=(1<<26); //Output
	GPIOB_PTOR|=(1<<21); //For LED1
	return;
}
	
void FTM0_IRQHandler(void){ //For FTM globTimerEn
	int temp=FTM0_SC;
	FTM0_SC&= ~(FTM_SC_TOF_MASK); //clear interrupt in FTM0 SC
	if(globTimerEn){ //if SW2 has set the global variable to signal a button press 
		globCounter++; //increment global counter
	}
	return;
}
	
void PORTA_IRQHandler(void){ //For switch 3
	PORTA_PCR4|=PORT_PCR_ISF_MASK;//clear interrupt
	if(PDB0_SC&(PDB_SC_PDBEN_MASK)){
		//disable timer
		PDB0_SC&= ~(PDB_SC_PDBEN_MASK);
	}else{
		//enable timer
		PDB0_SC|=PDB_SC_PDBEN_MASK;
		//start timer with a trigger
		PDB0_SC|=PDB_SC_SWTRIG_MASK;
	}
	return;
}
	
void PORTC_IRQHandler(void){ //For switch 2
	PORTC_PCR6|=PORT_PCR_ISF_MASK;//clear interrupt
	char temp[30];
	if((GPIOC_PDIR&(1<<6))==0){ //if SW2 being pressed
		globTimerEn=1;
	}else{
		globTimerEn=0;
		sprintf(temp, "Counter for button: %d ms\n\r", globCounter);
		uart0_put(temp);
		globCounter=0;
	}
	return;
}
