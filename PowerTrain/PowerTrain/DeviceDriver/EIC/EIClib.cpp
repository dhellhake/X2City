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
#include "..\CortexM0\CortexM0.h"

void InitEIC()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBAMASK.bit.EIC_ = 1;
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[EIC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);
	/* Configure the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);
	// Enable GCLK for peripheral
	GCLK->PCHCTRL[EIC_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	EIC->EVCTRL.reg |=			(1 << 7) | (1 << 12) | (1 << 13);
	
	EIC->CONFIG[0].reg =		EIC_CONFIG_SENSE7_BOTH |			// EXTINT7
								EIC_CONFIG_FILTEN7;
	EIC->CONFIG[1].reg =		EIC_CONFIG_SENSE4_BOTH |			// EXTINT12
								EIC_CONFIG_FILTEN4 |
								EIC_CONFIG_SENSE5_BOTH |			// EXTINT13
								EIC_CONFIG_FILTEN5;
	
	EIC->CTRLA.reg |=			EIC_CTRLA_ENABLE;
}