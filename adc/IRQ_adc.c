/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"
#include "../ProjectFunctions/myfunctions.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/
 
void ADC_IRQHandler(void) {
	static uint16_t AD_current, AD_last=0xFFFF; 
	uint16_t volumeADC;
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result  (12bit)          */
  if(AD_current != AD_last){
		volumeADC = (uint8_t) (AD_current * 100/4096);
		drawVolumeLevels(volumeADC);
  }
}
