/* 
* HallSensor.h
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#ifndef __HALLSENSOR_H__
#define __HALLSENSOR_H__

#include "..\OS\Executable.h"

class HallSensor : public Executable
{
	/************************************************************************/
	/* Task Interface implementation										*/
	/************************************************************************/
	public:
		virtual RUN_RESULT Run(void);
	
}; //HallSensor

extern HallSensor Hall;

#endif //__HALLSENSOR_H__
