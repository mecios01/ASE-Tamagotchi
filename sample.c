/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "ProjectFunctions/myfunctions.h"
#include "joystick/joystick.h"
#include "RIT/rit.h"
#include "ProjectFunctions/audio.h"
#include "adc/adc.h"
#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

uint16_t BgColor=White;//Grey;
uint8_t hh=0,mm=0,ss=0;
uint8_t happiness=100,satiety=100;
uint8_t currentAudio = A_NONE; //audio status
uint8_t volume = 100;
int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	TP_Init();
	ADC_init();
	TouchPanel_Calibrate();
	LCD_Clear(BgColor);
	drawVolumeLevels(volume);	
	drawAgeWidget();
	drawHappinessAndSatiety();
	drawActionButtons();
	drawBaseSprite();
	animateSprite();

	init_timer(1,0x17D7840);
	enable_timer(1);
	
	init_timer(2,0x3F940A); //about 1/6 of sec animations
	init_timer(3,0x000100); //almost instantly
	joystick_init();
	
	init_RIT(0x1312D0); //50ms
	enable_RIT();
	
	ADC_init();
	
	LPC_SC->PCON |= 0x1;									
	LPC_SC->PCON &= ~(0x2);						
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
