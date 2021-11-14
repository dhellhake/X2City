/* 
* CortexM0.h
*
* Created: 14.11.2021 09:06:25
*  Author: Dominik Hellhake
*/
#ifndef __CORTEXM0_H__
#define __CORTEXM0_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"

	extern volatile uint32_t ElapsedMilis;

	void InitSysTick();

#ifdef __cplusplus
}
#endif

#endif //__CORTEXM0_H__
