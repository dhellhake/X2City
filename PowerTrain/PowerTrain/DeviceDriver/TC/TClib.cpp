/* 
* TClib.cpp
*
* Created: 05.09.2021 12:04:53
* Author: dominik hellhake
*/
#include "TClib.h"
#include "..\DMAC\DMAClib.h"
#include "..\..\HallSensor\HallSensor.h"
#include "..\..\BLDC\BLDC.h"
#include "..\TCC\TCClib.h"

volatile uint32_t TC0_Overflow;

void InitTC0_1()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC0_ = 1;
	MCLK->APBCMASK.bit.TC1_ = 1;
	
	 
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);
	GCLK->PCHCTRL[TC1_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC1_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);
	GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	GCLK->PCHCTRL[TC1_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;


	// Disable TC0 & TC1
	TC0->COUNT32.CTRLA.reg &=	~(1 << TC_CTRLA_ENABLE_Pos);
	
	TC0->COUNT32.CTRLA.reg =	(0 << TC_CTRLA_CAPTEN1_Pos) |		// Default caputre mode for channel 1
								(0 << TC_CTRLA_CAPTEN0_Pos) |		// Default capture mode for channel 0
								(0 << TC_CTRLA_COPEN1_Pos) |		// Event as capture trigger for channel 1
								(0 << TC_CTRLA_COPEN0_Pos) |		// Event as capture trigger for channel 0
								(0 << TC_CTRLA_CAPTEN1_Pos) |		// Disable capture for channel 1
								(0 << TC_CTRLA_CAPTEN0_Pos) |		// Disable capture for channel 0
								(0 << TC_CTRLA_ALOCK_Pos) |			// Disable auto lock
								(0 << TC_CTRLA_PRESCALER_Pos) |		// Set 1x Prescaler
								(0 << TC_CTRLA_ONDEMAND_Pos) |		// Disable on demand
								(0 << TC_CTRLA_RUNSTDBY_Pos) |		// Disable Run Standby
								(0 << TC_CTRLA_PRESCSYNC_Pos) |		// Sync on GCLK
								(2 << TC_CTRLA_MODE_Pos);			// Set 32 bit mode
	
	TC0->COUNT32.WAVE.reg =		(1 << TC_WAVE_WAVEGEN_Pos);			// Set MFRQ mode
	
	TC0->COUNT32.CC[0].reg = 0xFFFFFFFF;
	
	TC0->COUNT32.INTENSET.reg = (1 << TC_INTENSET_OVF_Pos);			// Enable overflow interrupt
	
	NVIC_SetPriority(TC0_IRQn, 1);									// Set interrupt priority to least urgency
	NVIC_EnableIRQ(TC0_IRQn);										// Enable Interrupt
		
	TC0->COUNT32.CTRLA.reg |= (1 << TC_CTRLA_ENABLE_Pos);	
}

void TC0_Handler(void)
{
	if ((TC0->COUNT32.INTFLAG.reg & TC_INTFLAG_OVF) != 0x00)
	{
		TC0->COUNT32.INTFLAG.reg = TC_INTFLAG_OVF;
		TC0_Overflow++;
	}
}



void InitTC3()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC3_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC3_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC3_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC3_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;


	// Disable TC0
	TC3->COUNT16.CTRLA.reg &=	~(1 << TC_CTRLA_ENABLE_Pos);
	
	TC3->COUNT16.CTRLA.reg =	(0 << TC_CTRLA_CAPTEN1_Pos) |		// Default caputre mode for channel 1
								(0 << TC_CTRLA_CAPTEN0_Pos) |		// Default capture mode for channel 0
								(0 << TC_CTRLA_COPEN1_Pos) |		// Event as capture trigger for channel 1
								(0 << TC_CTRLA_COPEN0_Pos) |		// Event as capture trigger for channel 0
								(0 << TC_CTRLA_CAPTEN1_Pos) |		// Disable capture for channel 1
								(0 << TC_CTRLA_CAPTEN0_Pos) |		// Disable capture for channel 0
								(0 << TC_CTRLA_ALOCK_Pos) |			// Disable auto lock
								(0 << TC_CTRLA_PRESCALER_Pos) |		// Set 1x Prescaler
								(0 << TC_CTRLA_ONDEMAND_Pos) |		// Disable on demand
								(0 << TC_CTRLA_RUNSTDBY_Pos) |		// Disable Run Standby
								(0 << TC_CTRLA_PRESCSYNC_Pos) |		// Sync on GCLK
								(0 << TC_CTRLA_MODE_Pos);			// Set 16 bit mode
	
	TC3->COUNT16.WAVE.reg =		(1 << TC_WAVE_WAVEGEN_Pos);			// Set MFRQ mode
	
	TC3->COUNT16.COUNT.reg = 0;
	//TC1->COUNT16.CC[0].reg = 5582;
	TC3->COUNT16.CC[0].reg = 0xffff;
	
	TC3->COUNT16.INTENSET.reg = (1 << TC_INTENSET_OVF_Pos);			// Enable overflow interrupt
	
	NVIC_SetPriority(TC3_IRQn, 1);									// Set interrupt priority to least urgency
	NVIC_EnableIRQ(TC3_IRQn);										// Enable Interrupt
	
	//TC1->COUNT16.CTRLA.reg |= (1 << TC_CTRLA_ENABLE_Pos);
}

