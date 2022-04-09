/* 
* CortexM0.cpp
*
* Created: 14.11.2021 09:06:25
* Author: dominik hellhake
*/
#include "CortexM0.h"
#include "OS\os.h"

void InitSysTick()
{
	SysTick->CTRL =		0;				// Disable SysTick
	NVIC_SetPriority(SysTick_IRQn, 0);	// Set interrupt priority to highest urgency
	SysTick->LOAD =		48000;			// Set reload register for overflow interrupts
	SysTick->VAL =		0;				// Reset the SysTick counter value
	SysTick->CTRL =		0x00000007;		// Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
	NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt
}

void SysTick_Handler()
{
	os_task_scheduler();
}