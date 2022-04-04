/* 
* ComHandler.h
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#ifndef __COMHANDLER_H__
#define __COMHANDLER_H__

#include "..\OS\Executable.h"

typedef struct com_debug_record_t
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
	uint32_t	AVL_HallState =         0;
} com_debug_record_t;

class ComHandler : public Executable
{
	/************************************************************************/
	/* Task Interface implementation										*/
	/************************************************************************/
	public:
	virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		com_debug_record_t Record;

}; //ComHandler

extern ComHandler ComHdl;

#endif //__COMHANDLER_H__
