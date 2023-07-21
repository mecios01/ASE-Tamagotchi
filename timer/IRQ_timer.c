/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../ProjectFunctions/myfunctions.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
const uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};
extern uint8_t volume;
void TIMER0_IRQHandler (void)
{
	//play note
	static int sineticks=0;
	
	/* DAC management */	
	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	
	LPC_DAC->DACR = (uint32_t)((currentValue << 6)*volume/100);
	sineticks++;
	if(sineticks==45) sineticks=0;

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	updateAge();
	updateLevels();
	animateSprite();
	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	disable_timer(2); //I'm using this timer just as a delay
	reset_timer(2);
	handleAnimation();
	LPC_TIM2->IR = 1;
}

void TIMER3_IRQHandler (void)
{
	//note duration
	disable_timer(0); //stop playing note
	//play next note(?)4
	playCurrentAudio(); //if there are more notes to play restart timer else stop and set audio state to NONE
	LPC_TIM3->IR = 1;
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
