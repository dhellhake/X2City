/* 
* RuntimeEnvironment.h
*
* Created: 11.05.2022 20:43:40
* Author: dominik hellhake
*/
#ifndef __RUNTIMEENVIRONMENT_H__
#define __RUNTIMEENVIRONMENT_H__

#include "sam.h"
#include "..\HallSensor\Hall.h"

typedef struct rte_image_record_t
{	
	uint64_t SystemTime;
		
	/* HallSensor */
	uint16_t	Avl_DriveDirection;
	float		Avl_TicksPerSecond;
	uint32_t	Avl_Ticks = 0;
	
	/* DRV8323 */
	uint32_t	Avl_DRV_State;
	float		Tar_Duty;
	
	uint32_t padding = 0xAAAAAAAA;
} rte_image_record_t;

class RuntimeEnvironment
{
	public:
		rte_image_record_t Record;
	
		RuntimeEnvironment();
			
	private:		
}; //RuntimeEnvironment

extern RuntimeEnvironment Rte;

#endif //__RUNTIMEENVIRONMENT_H__
