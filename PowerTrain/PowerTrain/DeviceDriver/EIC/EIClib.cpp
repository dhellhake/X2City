/* 
* EIClib.cpp
*
* Created: 28.08.2021 17:54:15
* Author: domininik hellhake
*/
#include "EIClib.h"
#include "..\TC\TClib.h"
#include "..\..\HallSensor\HallSensor.h"
#include "..\KSZ8851\KSZ8851.h"

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
	NVIC_EnableIRQ(EIC_IRQn);		// Enable SysTick Interrupt
	
	EIC->CONFIG[0].reg =	EIC_CONFIG_SENSE0_LOW |
							EIC_CONFIG_FILTEN0 |
							EIC_CONFIG_SENSE7_BOTH |			// EXTINT7
							EIC_CONFIG_FILTEN7;		
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
	uint32_t elapsedMicros = GetElapsedMicros();
	
	if ((EIC->INTFLAG.reg & (1 << 13)) != 0x00)
		Hall.HallTrigger(HallSignalU, elapsedMicros);
	else if ((EIC->INTFLAG.reg & (1 << 12)) != 0x00)
		Hall.HallTrigger(HallSignalV, elapsedMicros);
	else if ((EIC->INTFLAG.reg & (1 << 7)) != 0x00)
		Hall.HallTrigger(HallSignalW, elapsedMicros);
		
	if ((EIC->INTFLAG.reg & (1 << 0)) != 0x00)
	{
		KSZ8851_Handler();
		EIC->INTFLAG.reg = (1 << 0);
	}
	
 	//DRV.Drive_SetPhase(Hall.Avl_HallState);
	
	EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
}

	
void EICEnableInterrupt(uint16_t extInt)
{
	EIC->INTENSET.reg |=	(1 << extInt);
}
void EICDisableInterrupt(uint16_t extInt)
{
	EIC->INTENCLR.reg =		(1 << extInt);
}