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
	
	EIC->CONFIG[0].reg =	EIC_CONFIG_SENSE7_BOTH |			// EXTINT7
							EIC_CONFIG_FILTEN7;		
	EIC->CONFIG[1].reg =	EIC_CONFIG_SENSE4_BOTH |			// EXTINT12
							EIC_CONFIG_FILTEN4 |
							EIC_CONFIG_SENSE5_BOTH |			// EXTINT13
							EIC_CONFIG_FILTEN5;
							
	//Enable Interrupt on ECTINT 7, 12, 13
	EIC->INTENSET.reg |=	(1 << 0) | (1 << 7) | (1 << 12) | (1 << 13);
	EIC->INTFLAG.reg =		0xFFFF;		// Clear Interrupt on all EXTINT
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}

void EIC_Handler()
{	
	uint32_t elapsedMicros = GetElapsedMicros();
	
	uint32_t hallIn = (EIC->INTFLAG.reg & 0b11000010000000);	
	if (hallIn > 0)
	{
		HALL_STATE newState = HALL_STATE::UNDEFINED_1;
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
		
		if ((hallIn & (1 << 13)) != 0x00)
			newState = Hall.HallTrigger(HallSignalU, elapsedMicros);
		else if ((hallIn & (1 << 12)) != 0x00)
			newState = Hall.HallTrigger(HallSignalV, elapsedMicros);
		else if ((hallIn & (1 << 7)) != 0x00)
			newState = Hall.HallTrigger(HallSignalW, elapsedMicros);
		
		DRV.Drive_SetPhase(newState);
	}
	
		
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