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
	uint16_t	Avl_HallState;
	uint16_t	Avl_DriveDirection;
	uint32_t	Avl_AvgHallStateInterval;
	float		Avl_TicksPerSecond;
	uint32_t	Avl_Ticks;
	
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
		
		inline void SetSystemTime(uint64_t systemTime)
		{
			this->Record.SystemTime = systemTime;
		}
		
		inline void SetHallState(HALL_STATE state)
		{
			this->Record.Avl_HallState = (uint32_t)state;			
		}
		inline HALL_STATE GetHallState()
		{
			return (HALL_STATE)this->Record.Avl_HallState;
		}
		inline void SetHallStateInterval(uint32_t interval)
		{
			this->Record.Avl_AvgHallStateInterval = interval;
		}
		inline uint32_t GetHallStateInterval()
		{
			return this->Record.Avl_AvgHallStateInterval;
		}
		inline void SetHallTicksPerSecond(float tps)
		{
			this->Record.Avl_TicksPerSecond = tps;
		}
		inline void SetHallTicks(uint32_t ticks)
		{
			this->Record.Avl_Ticks = ticks;
		}
		inline uint32_t GetHallTicks()
		{
			return this->Record.Avl_Ticks;
		}
	
	private:		
}; //RuntimeEnvironment

extern RuntimeEnvironment Rte;

#endif //__RUNTIMEENVIRONMENT_H__
