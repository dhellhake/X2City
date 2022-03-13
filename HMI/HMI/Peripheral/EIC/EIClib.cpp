/* 
* EIClib.cpp
*
* Created: 28.08.2021 17:54:15
* Author: domininik hellhake
*/
#include "EIClib.h"
#include "..\CortexM0\CortexM0.h"

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
	//NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)EIC_IRQn & 0x0000001f));
	NVIC_EnableIRQ(EIC_IRQn);		// Enable SysTick Interrupt
	
	EIC->CONFIG[1].reg =	EIC_CONFIG_SENSE4_BOTH |			// EXTINT12
							EIC_CONFIG_FILTEN4 |
							EIC_CONFIG_SENSE5_BOTH |			// EXTINT13
							EIC_CONFIG_FILTEN5;
							
	//Enable Interrupt on ECTINT 7, 12, 13
	EIC->INTENSET.reg |=	(1 << 7) | (1 << 12) | (1 << 13);
	EIC->INTFLAG.reg =		0xFFFF;		// Clear Interrupt on all EXTINT
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}

void EIC_Handler()
{
	volatile uint32_t tstmp = ElapsedMilis;
	
	if ((EIC->INTFLAG.reg & (1 << 13)) != 0x00)
	{	// Button: Mode
		
		EIC->INTFLAG.reg = (1 << 13);		
	}
	else if ((EIC->INTFLAG.reg & (1 << 12)) != 0x00)
	{	// Button: IO / Light
		EIC->INTFLAG.reg = (1 << 12);
	}	
}