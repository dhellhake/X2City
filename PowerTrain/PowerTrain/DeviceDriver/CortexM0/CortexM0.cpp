/*
* CortexM0.cpp
*
* Created: 14.11.2021 09:06:25
* Author: dominik hellhake
*/
#include "CortexM0.h"

volatile uint32_t ElapsedMilis = 0;

void InitSysTick()
{
	SysTick->CTRL =		0;				// Disable SysTick
	SysTick->LOAD =		48000;			// Set reload register for overflow interrupts
	SysTick->VAL =		0;				// Reset the SysTick counter value
	SysTick->CTRL =		0b101;			// Enable SysTick[0], Disable SysTick Exceptions[1], Use CPU Clock[2]
	//NVIC_SetPriority(SysTick_IRQn, 0);	// Set interrupt priority to highest urgency
	//NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt
}