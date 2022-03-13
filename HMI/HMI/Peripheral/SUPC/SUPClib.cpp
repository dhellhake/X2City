/* 
* SUPClib.cpp
*
* Created: 17.02.2022 21:42:49
* Author: dominik hellhake
*/
#include "SUPClib.h"

void InitSUPC()
{
	SUPC->VREF.reg =	(0x3 << SUPC_VREF_SEL_Pos) |		// 4.096V voltage reference typical value
						(0x0 << SUPC_VREF_ONDEMAND_Pos) |	// The voltage reference is always on, if enabled.
						(0x0 << SUPC_VREF_RUNSTDBY_Pos) |	// The voltage reference is halted during Standby Sleep mode.
						(0x1 << SUPC_VREF_VREFOE_Pos);		// The Voltage Reference output is routed to an ADC input channel.
}