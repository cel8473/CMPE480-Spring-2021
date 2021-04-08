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
#include "./modules/gpio.h"
#include "./modules/ftm.h"
#include "./modules/adc.h"
#include "./modules/pit.h"

#define TRACK_THRESHOLD 20000

void init_camera(void)
{
	init_GPIO(); //For CLK and SI output on GPIO
	init_FTM2(); //To generate CLK, SI, and trigger ADC
	init_ADC0(); //Don't ask why they changed their naming convention, wasn't me
	init_PIT(); //To trigger camera read based on integration time
}

/*
	Finds the two edges of the line
	Side = 1, left most edge
	Side = 0, right most edge
*/
int edgeFinder(uint16_t line[], int side){
		switch(side){
			case 1:
				for(int i = 0; i < 128; i++){
					if(line[i] > TRACK_THRESHOLD){
						return i;
					}
				}
			case 0:
				for(int i = 128; i >= 0; i--){
					if(line[i] > TRACK_THRESHOLD){
						return i;
					}
				}
			default:
				return -1;
		}
}

// Takes the left most edge and the right most edge 
// of the camera array and finds mid point between the two
int find_center(int left, int right){
	int middle = (left+right)/2;
	middle = middle*100/120;
	return middle;
}

int sum_line(uint16_t line[])
{
	int sum = 0;
	for(int i = 0 ; i < 128;i++)
	{
			if(line[i]>=TRACK_THRESHOLD){
					sum += 1;
			}
	}
	return sum;
}

