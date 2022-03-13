/* 
* SDADClib.cpp
*
* Created: 17.02.2022 20:44:53
* Author: dominik hellhake
*/
#include "SDADClib.h"

void InitSDADC()
{
	MCLK->APBCMASK.reg	|= MCLK_APBCMASK_SDADC;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SDADC_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SDADC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SDADC_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SDADC_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	
	// Reset SDADC
	SDADC->CTRLA.reg =	SDADC_CTRLA_SWRST;
	while (SDADC->CTRLA.reg & SDADC_CTRLA_SWRST);
		
	SDADC->REFCTRL.reg =	(0x1 << SDADC_REFCTRL_ONREFBUF_Pos) |	// Reference Buffer Off
							(0x3 << SDADC_REFCTRL_REFRANGE_Pos) | 
							(0x0 << SDADC_REFCTRL_REFSEL_Pos);		// Internal voltage reference, supplied by the bandgap (SUPC.VREF.SEL == 0x0 => 1,024V)
	while (SDADC->SYNCBUSY.reg);
	
	SDADC->CTRLB.reg =		(0x0 << SDADC_CTRLB_SKPCNT_Pos) |		// 0 samples skipped before retrieve the first valid sample.							(0x4 << SDADC_CTRLB_OSR_Pos) |			// Over Sampling Ratio is 1024
							(0xC << SDADC_CTRLB_PRESCALER_Pos);		// The CLK_SDADC_PRESCAL is 24
	// Sampling-Rate:	CLK_GEN_SDADC/(OSR * PRESCALER * 4)
	//					1000000 / (1024 * 24 * 4) = 10,1724 Sps
	while (SDADC->SYNCBUSY.reg);
		
	SDADC->INTENSET.reg =	(0x0 << SDADC_INTENSET_WINMON_Pos) |	// The Window Monitor interrupt is disabled.
							(0x0 << SDADC_INTENSET_OVERRUN_Pos) |	// The Overrun interrupt is disabled.							(0x1 << SDADC_INTENSET_RESRDY_Pos);		// The Result Ready interrupt is enabled.
	NVIC_SetPriority(SDADC_IRQn, 1);								// Set interrupt priority to high
	NVIC_EnableIRQ(SDADC_IRQn);										// Enable SDADC Interrupt
	while (SDADC->SYNCBUSY.reg);
	
	
	SDADC->OFFSETCORR.reg = 4000;	
	
	SDADC->INPUTCTRL.reg =	(0x01 << SDADC_INPUTCTRL_MUXSEL_Pos);	// Select ADC AINN0 and AINP0 pins
	while (SDADC->SYNCBUSY.reg);
	
	SDADC->CTRLC.reg =		(0x1 << SDADC_CTRLC_FREERUN_Pos);		// The SDADC is in free running mode and a new conversion will be initiated when a previous conversion completes.
	while (SDADC->SYNCBUSY.reg);
	
	SDADC->CTRLA.reg =		(0 << SDADC_CTRLA_ONDEMAND_Pos) |		// The SDADC is always on , if enabled.
							(0 << SDADC_CTRLA_RUNSTDBY_Pos) |		// The SDADC is halted during standby sleep mode
							(1 << SDADC_CTRLA_ENABLE_Pos);			// The SDADC is enabled.
}

void SDADC_Handler()
{
	int32_t result = ((int32_t)(SDADC->RESULT.reg << 8)) >> 8;;
	float r = ((float)result) / 539153.4722222222f;
	
	
	PORT->Group[0].OUTTGL.reg = PORT_PA04;
	
	SDADC->INTFLAG.reg =	(0x1 << SDADC_INTFLAG_RESRDY_Pos);		//  Clears the Result Ready interrupt flag
}