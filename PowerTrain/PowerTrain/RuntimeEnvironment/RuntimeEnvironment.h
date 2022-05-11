/* 
* RuntimeEnvironment.h
*
* Created: 11.05.2022 20:43:40
* Author: dominik hellhake
*/
#ifndef __RUNTIMEENVIRONMENT_H__
#define __RUNTIMEENVIRONMENT_H__

#include "sam.h"

typedef struct rte_image_record_t
{
	uint8_t EthHeader[16];
	
	uint64_t SystemTime;
	
	/* DRV8323 */
	uint32_t	Avl_DRV_State;
	float		Tar_Duty;
	
	/* HallSensor */
	uint32_t	Avl_AvgHallStateInterval;
	float		Avl_TicksPerSecond;
	uint32_t	Avl_Ticks;
	uint32_t	Avl_DriveDirection;
	uint32_t	Avl_HallState;
	
} rte_image_record_t;

class RuntimeEnvironment
{
	public:
		RuntimeEnvironment();
		
		inline void SetSystemTime(uint64_t systemTime)
		{
			this->Record->SystemTime = systemTime;
		}
	
	private:		
		rte_image_record_t *Record;
		
		
}; //RuntimeEnvironment

extern RuntimeEnvironment Rte;

#endif //__RUNTIMEENVIRONMENT_H__
