/* 
* SDADClib.cpp
*
* Created: 11.12.2022 15:38:34
* Author: dominik hellhake
*/
#include "SDADClib.h"

float tmpVoltage = 0.0f;

void InitSDADC()
{
	//Enable Clock for SDADC
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SDADC_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SDADC_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SDADC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SDADC_GCLK_ID].reg = GCLK_PCHCTRL_GEN(1);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SDADC_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	SDADC->CTRLA.reg		=	SDADC_CTRLA_SWRST;
	while (SDADC->SYNCBUSY.reg & SDADC_SYNCBUSY_SWRST);
	
	SDADC->CTRLA.reg		&=	~(SDADC_CTRLA_ENABLE);
	while (SDADC->SYNCBUSY.reg & SDADC_SYNCBUSY_ENABLE);
	
	
	SDADC->CTRLA.reg		=	(0 << SDADC_CTRLA_ONDEMAND_Pos) |
								(0 << SDADC_CTRLA_RUNSTDBY_Pos);
	
	SDADC->REFCTRL.reg		=	(0 << SDADC_REFCTRL_ONREFBUF_Pos) |
								(0x3 << SDADC_REFCTRL_REFSEL_Pos);
	
	SDADC->CTRLB.reg		=	(2 << SDADC_CTRLB_SKPCNT_Pos) |
								(0x4 << SDADC_CTRLB_OSR_Pos) |
								(0x1 << SDADC_CTRLB_PRESCALER_Pos);
	
	
	SDADC->INTENSET.reg		=	(0 << SDADC_INTENSET_WINMON_Pos) |
								(0 << SDADC_INTENSET_OVERRUN_Pos) |
								(1 << SDADC_INTENSET_RESRDY_Pos);							
	
	SDADC->INPUTCTRL.reg	=	(     SDADC_INPUTCTRL_MUXSEL_AIN0);	
	
	SDADC->CTRLC.reg		=	(1 << SDADC_CTRLC_FREERUN_Pos);
	while (SDADC->SYNCBUSY.reg & SDADC_SYNCBUSY_CTRLC);
		
	NVIC_SetPriority(SDADC_IRQn, 1);
	NVIC_EnableIRQ(SDADC_IRQn);
		
	SDADC->CTRLA.reg		=	(SDADC_CTRLA_ENABLE);
}


void SDADC_Handler(void)
{
	int32_t result  = ((int32_t)(SDADC->RESULT.reg));
	float resultf = result * 0.000000393089f;
	
	tmpVoltage = resultf;
	
	SDADC->INTFLAG.reg = SDADC_INTFLAG_RESRDY;
}