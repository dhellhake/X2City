/*
 * GCLK.c
 *
 * Created: 15.03.2019 15:22:38
 *  Author: Dominik Hellhake
 */ 
#include "GCLK.h"

void InitGCLK()
{	
	// Set 1 wait state; manual write; deterministich cache read/write
	NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS(2) | NVMCTRL_CTRLB_MANW | NVMCTRL_CTRLB_READMODE_DETERMINISTIC;
	
	// Set Clock-Division to 1 for OCS48M 
	OSCCTRL->OSC48MDIV.reg = OSCCTRL_OSC48MDIV_DIV(1 - 1);
	while (OSCCTRL->OSC48MSYNCBUSY.reg != 0);
		
	GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC_OSC48M | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(1);    // 48MHz
}

