/*
 * Freescale Cup linescan camera code
 *
 *	This method of capturing data from the line
 *	scan cameras uses a flex timer module, periodic
 *	interrupt timer, an ADC, and some GPIOs.
 *	CLK and SI are driven with GPIO because the FTM2
 *	module used doesn't have any output pins on the
 * 	development board. The PIT timer is used to 
 *  control the integration period. When it overflows
 * 	it enables interrupts from the FTM2 module and then
 *	the FTM2 and ADC are active for 128 clock cycles to
 *	generate the camera signals and read the camera 
 *  output.
 *
 *	PTB9			- camera CLK
 *	PTB23 		- camera SI
 *  ADC0_DP0 	- camera AOut
 *
 * Author:  Alex Avery
 * Created:  11/20/15
 * Modified:  11/23/15
 */

#include "MK64F12.h"
#include "./modules/uart.h"
#include "stdio.h"
#include "led.h"
#include "camera.h"


#define CARPET_THRESHOLD (4500)

#define trackThreshold 20000

/*
	Finds the two edges of the line
	Side = 1, left most edge
	Side = 0, right most edge
*/
int edgeFinder(uint16_t line[], int side){
		switch(side){
			case 1:
				for(int i = 0; i < 128; i++){
					if(line[i] > trackThreshold){
						return i;
					}
				}
			case 0:
				for(int i = 128; i >= 0; i--){
					if(line[i] > trackThreshold){
						return i;
					}
				}
			default:
				return -1;
		}
}

int find_center(int left, int right)












