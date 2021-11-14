/* 
* ComPort.h
*
* Created: 04.09.2021 18:37:08
* Author: dominik hellhake
*/
#ifndef __COMPORT_H__
#define __COMPORT_H__

#include "..\OS\Executable.h"

typedef struct com_status_t
{
	uint16_t preamble =					0xDEAD;
	uint16_t bytes =					28;
		
	/* DRV8323 */
	uint32_t	Avl_DRV_State =			0x00;
	float		Tar_Duty =				0.0f;
	
	/* HallSensor */
	uint32_t	Avl_AvgHallStateInterval =		0;
	float		Avl_TicksPerSecond =	0.0f;
	uint32_t	Avl_Ticks = 0;
	uint32_t	Avl_DriveDirection =	0;
} com_status_t;

class ComPort : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	public:
		virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		com_status_t Record;

}; //ComPort

extern ComPort Com;

#endif //__COMPORT_H__
