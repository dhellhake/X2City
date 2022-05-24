/*
* TCClib.cpp
*
* Created: 03.09.2021 12:30:17
* Author: dominik hellhake
*/
#include "TCClib.h"
#include "..\..\BLDC\BLDCPattern.h"

void InitTCC0()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TCC0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TCC0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TCC0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TCC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TCC0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	
	TCC0->CTRLA.reg =	(0 << TCC_CTRLA_CPTEN_Pos) |			// Capture Channel disabled
						(0 << TCC_CTRLA_DMAOS_Pos) |			// DMA One-Shot triggered on each TCC cycle
						(0 << TCC_CTRLA_MSYNC_Pos) |			// TCC controls its own counter
						(0 << TCC_CTRLA_ALOCK_Pos) |			// Lock Update bit in CTRLB not affected by TTC event
						(0 << TCC_CTRLA_PRESCSYNC_Pos) |		// Reload or reset Counter on GCLK
						(1 << TCC_CTRLA_RUNSTDBY_Pos) |			// TCC continues to run in standby
						(0 << TCC_CTRLA_PRESCALER_Pos) |		// No division of 48Mhz GCLK
						(0 << TCC_CTRLA_RESOLUTION_Pos);		// Dithering is disabled
	
	TCC0->DBGCTRL.reg = (1 << TCC_DBGCTRL_FDDBD_Pos) |			// Halting the CPU generates non-recoverable fault
						(1 << TCC_DBGCTRL_DBGRUN_Pos);			// TCC continues normal operation when halted
	
	TCC0->WAVE.reg =	(0x0 << TCC_WAVE_SWAP_Pos) |			// No output swap
						(0x0 << TCC_WAVE_POL_Pos) |				// no output polarity change
						(0x0 << TCC_WAVE_CICCEN_Pos) |			// No compare circular buffer
						(0x0 << TCC_WAVE_CIPEREN_Pos) |			// No period circular buffer
						(0x0 << TCC_WAVE_RAMP_Pos) |			// RAMP1 selected
						(0x2 << TCC_WAVE_WAVEGEN_Pos);			// Normal PWM generation
	
	TCC0->WEXCTRL.reg = (0x0 << TCC_WEXCTRL_DTHS_Pos) |			// 0 number of GCLK as dead time on High Side Output
						(0x0 << TCC_WEXCTRL_DTLS_Pos) |			// 0 number of GCLK as dead time on Low Side Output
						(0x0 << TCC_WEXCTRL_DTIEN_Pos) |		// Dead-time Insertion disabled
						(0x2 << TCC_WEXCTRL_OTMX_Pos);			// Use CC0 for each output
	
	TCC0->DRVCTRL.reg = (0x0 << TCC_DRVCTRL_FILTERVAL1_Pos) |	// Non-Recoverable Fault Input 1 Filter
						(0x0 << TCC_DRVCTRL_FILTERVAL0_Pos) |	// Non-Recoverable Fault Input 0 Filter
						(0b11010000 << TCC_DRVCTRL_INVEN_Pos) | // Set U, V, W Low-Side Output inverted
						(0x0 << TCC_DRVCTRL_NRV_Pos) |			// Output value for Non-Recoverable State
						(0xFF << TCC_DRVCTRL_NRE_Pos);			// Override output on Non-Recoverable Fault
	
	TCC0->PATT.reg =	(((1 << U_HIGH_PATT_Pos) |
						  (1 << U_LOW_PATT_Pos) |
						  (1 << V_HIGH_PATT_Pos) |
						  (1 << V_LOW_PATT_Pos) |
						  (1 << W_HIGH_PATT_Pos) |
						  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
						(((0 << U_HIGH_PATT_Pos) |
						  (1 << U_LOW_PATT_Pos) |
						  (0 << V_HIGH_PATT_Pos) |
						  (1 << V_LOW_PATT_Pos) |
						  (0 << W_HIGH_PATT_Pos) |
						  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

	TCC0->COUNT.reg	=	0x00;									// Timer Counter start at 0
	TCC0->PER.reg =		TCC0_PER_VAL;							// Set Phase Length
	TCC0->CC[0].reg	=	0;										// 0% Duty on Phase- U V W
	
	TCC0->CTRLA.bit.ENABLE = 1;
}