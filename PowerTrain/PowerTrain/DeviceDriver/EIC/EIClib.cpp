/* 
* EIClib.cpp
*
* Created: 28.08.2021 17:54:15
* Author: domininik hellhake
*/
#include "EIClib.h"
#include "..\TC\TClib.h"
#include "..\..\HallSensor\HallSensor.h"
#include "..\..\BLDC\BLDC.h"

void InitEIC()
{
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[EIC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[EIC_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	
	// Reset EIC
	EIC->CTRLA.reg = EIC_CTRLA_SWRST;
	while (EIC->CTRLA.reg & EIC_CTRLA_SWRST);
	
	
	NVIC_SetPriority(EIC_IRQn, 1);
	NVIC_EnableIRQ(EIC_IRQn);
	
	EIC->CONFIG[0].reg =	EIC_CONFIG_SENSE0_LOW |			// EXTINT0
							EIC_CONFIG_FILTEN0;	
							
	EIC->INTENSET.reg |=	(1 << 0);
	EIC->INTFLAG.reg =		0xFFFF;		// Clear Interrupt on all EXTINT
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}

void EIC_Handler()
{			
	if ((EIC->INTFLAG.reg & (1 << 0)) != 0x00)
	{
		EIC->INTFLAG.reg = (1 << 0);
	}	
}

	
void EICEnableInterrupt(uint16_t extInt)
{
	EIC->INTENSET.reg =	(1 << extInt);
}
void EICDisableInterrupt(uint16_t extInt)
{
	EIC->INTENCLR.reg =		(1 << extInt);
}