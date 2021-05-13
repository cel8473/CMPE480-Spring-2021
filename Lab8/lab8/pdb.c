  
/*
 * File:        PDB.c
 * Purpose:     Provide PDB routines
 *
 * Notes:		
 *
 */
 
 #include "MK64F12.h"
 
 #include "pdb.h"
 
 
void PDB_INIT(void) {
    //Enable PDB Clock
    SIM_SCGC6 |= SIM_SCGC6_PDB_MASK;
    //PDB0_CNT = 0x0000;
    PDB0_MOD = 50000; // 50,000,000 / 50,000 = 1000

    PDB0_SC = PDB_SC_PDBEN_MASK | PDB_SC_CONT_MASK | PDB_SC_TRGSEL(0xf)
                                    | PDB_SC_LDOK_MASK;
    PDB0_CH1C1 = PDB_C1_EN(0x01) | PDB_C1_TOS(0x01);
  
  //NVIC_EnableIRQ(PDB0_IRQn);
}
