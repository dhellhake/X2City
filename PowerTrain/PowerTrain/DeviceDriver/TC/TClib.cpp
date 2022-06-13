/* 
* TClib.cpp
*
* Created: 05.09.2021 12:04:53
* Author: dominik hellhake
*/
#include "TClib.h"
#include "..\..\HallSensor\HallSensor.h"

void InitTC0()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Disable TC0
	TC0->COUNT16.CTRLA.reg &=	~(1 << TC_CTRLA_ENABLE_Pos);
	
	TC0->COUNT16.CTRLA.reg =	(0 << TC_CTRLA_COPEN1_Pos) |		// Event as capture trigger for channel 1
								(0 << TC_CTRLA_COPEN0_Pos) |		// Event as capture trigger for channel 0
								(1 << TC_CTRLA_CAPTEN1_Pos) |		// Disable capture for channel 1
								(1 << TC_CTRLA_CAPTEN0_Pos) |		// Disable capture for channel 0
								(0 << TC_CTRLA_ALOCK_Pos) |			// Disable auto lock
								(0 << TC_CTRLA_PRESCALER_Pos) |		// Set 1x Prescaler
								(0 << TC_CTRLA_ONDEMAND_Pos) |		// Disable on demand
								(0 << TC_CTRLA_RUNSTDBY_Pos) |		// Disable Run Standby
								(0 << TC_CTRLA_PRESCSYNC_Pos) |		// Sync on GCLK
								(0 << TC_CTRLA_MODE_Pos);			// Set 16 bit mode
	
	TC0->COUNT16.EVCTRL.reg =	(1 << TC_EVCTRL_TCEI_Pos) |
								(0x1 << TC_EVCTRL_EVACT_Pos);
	
	NVIC_SetPriority(TC0_IRQn, 1);									// Set interrupt priority to least urgency
	NVIC_EnableIRQ(TC0_IRQn);										// Enable Interrupt
	TC0->COUNT16.INTENSET.reg = (1 << TC_INTENSET_MC0_Pos) |		// Match or Capture Channel Interrupt Enable
								(1 << TC_INTENSET_MC1_Pos);			// Match or Capture Channel x Interrupt Enable
	
	TC0->COUNT16.CTRLA.reg |= (1 << TC_CTRLA_ENABLE_Pos);
}

void InitTC1()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC1_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC1_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC1_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC1_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Disable TC1
	TC1->COUNT16.CTRLA.reg &=	~(1 << TC_CTRLA_ENABLE_Pos);
	
	TC1->COUNT16.CTRLA.reg =	(0 << TC_CTRLA_COPEN1_Pos) |		// Event as capture trigger for channel 1
								(0 << TC_CTRLA_COPEN0_Pos) |		// Event as capture trigger for channel 0
								(1 << TC_CTRLA_CAPTEN1_Pos) |		// Disable capture for channel 1
								(1 << TC_CTRLA_CAPTEN0_Pos) |		// Disable capture for channel 0
								(0 << TC_CTRLA_ALOCK_Pos) |			// Disable auto lock
								(0 << TC_CTRLA_PRESCALER_Pos) |		// Set 1x Prescaler
								(0 << TC_CTRLA_ONDEMAND_Pos) |		// Disable on demand
								(0 << TC_CTRLA_RUNSTDBY_Pos) |		// Disable Run Standby
								(0 << TC_CTRLA_PRESCSYNC_Pos) |		// Sync on GCLK
								(0 << TC_CTRLA_MODE_Pos);			// Set 16 bit mode
	
	TC1->COUNT16.EVCTRL.reg =	(1 << TC_EVCTRL_TCEI_Pos) |
								(0x1 << TC_EVCTRL_EVACT_Pos);
	
	NVIC_SetPriority(TC1_IRQn, 1);									// Set interrupt priority to least urgency
	NVIC_EnableIRQ(TC1_IRQn);										// Enable Interrupt
	TC1->COUNT16.INTENSET.reg = (1 << TC_INTENSET_MC0_Pos) |		// Match or Capture Channel Interrupt Enable
								(1 << TC_INTENSET_MC1_Pos);			// Match or Capture Channel x Interrupt Enable
	
	TC1->COUNT16.CTRLA.reg |= (1 << TC_CTRLA_ENABLE_Pos);
}

void InitTC2()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC2_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC2_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC2_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC2_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC2_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Disable TC2
	TC2->COUNT16.CTRLA.reg &=	~(1 << TC_CTRLA_ENABLE_Pos);
	
	TC2->COUNT16.CTRLA.reg =	(0 << TC_CTRLA_COPEN1_Pos) |		// Event as capture trigger for channel 1
								(0 << TC_CTRLA_COPEN0_Pos) |		// Event as capture trigger for channel 0
								(1 << TC_CTRLA_CAPTEN1_Pos) |		// Disable capture for channel 1
								(1 << TC_CTRLA_CAPTEN0_Pos) |		// Disable capture for channel 0
								(0 << TC_CTRLA_ALOCK_Pos) |			// Disable auto lock
								(0 << TC_CTRLA_PRESCALER_Pos) |		// Set 1x Prescaler
								(0 << TC_CTRLA_ONDEMAND_Pos) |		// Disable on demand
								(0 << TC_CTRLA_RUNSTDBY_Pos) |		// Disable Run Standby
								(0 << TC_CTRLA_PRESCSYNC_Pos) |		// Sync on GCLK
								(0 << TC_CTRLA_MODE_Pos);			// Set 16 bit mode
	
	TC2->COUNT16.EVCTRL.reg =	(1 << TC_EVCTRL_TCEI_Pos) |
								(0x1 << TC_EVCTRL_EVACT_Pos);
	
	NVIC_SetPriority(TC2_IRQn, 1);									// Set interrupt priority to least urgency
	NVIC_EnableIRQ(TC2_IRQn);										// Enable Interrupt
	TC2->COUNT16.INTENSET.reg = (1 << TC_INTENSET_MC0_Pos) |		// Match or Capture Channel Interrupt Enable
								(1 << TC_INTENSET_MC1_Pos);			// Match or Capture Channel x Interrupt Enable
	
	TC2->COUNT16.CTRLA.reg |= (1 << TC_CTRLA_ENABLE_Pos);
}


void TC0_Handler(void)
{
	HALL_STATE newState = (HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111);
	
	float ccVal = 0.0f;
	if ((TC0->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0) != 0)
		ccVal = ((float)TC0->COUNT16.CC[0].reg) / 3.0f;
	else if ((TC0->COUNT16.INTFLAG.reg & TC_INTFLAG_MC1) != 0)
		ccVal = ((float)TC0->COUNT16.CC[1].reg) / 3.0f;
		
	Hall.HallTrigger(newState, (uint32_t)ccVal);
}

void TC1_Handler(void)
{
	HALL_STATE newState = (HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111);
	
	float ccVal = 0.0f;
	if ((TC1->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0) != 0)
		ccVal = ((float)TC1->COUNT16.CC[0].reg) / 3.0f;
	else if ((TC1->COUNT16.INTFLAG.reg & TC_INTFLAG_MC1) != 0)
		ccVal = ((float)TC1->COUNT16.CC[1].reg) / 3.0f;
		
	Hall.HallTrigger(newState, (uint32_t)ccVal);
}

void TC2_Handler(void)
{
	HALL_STATE newState = (HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111);
	
	float ccVal = 0.0f;
	if ((TC2->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0) != 0)
		ccVal = ((float)TC2->COUNT16.CC[0].reg) / 3.0f;
	else if ((TC2->COUNT16.INTFLAG.reg & TC_INTFLAG_MC1) != 0)
		ccVal = ((float)TC2->COUNT16.CC[1].reg) / 3.0f;
		
	Hall.HallTrigger(newState, (uint32_t)ccVal);
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
	TC3->COUNT16.CC[0].reg = 0xffff;
	
}

