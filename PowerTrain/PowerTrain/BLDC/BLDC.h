/* 
* BLDC.h
*
* Created: 16.05.2022 21:59:20
* Author: dominik hellhake
*/
#ifndef __BLDC_H__
#define __BLDC_H__

#include "..\Task.h"

class BLDC : public Task
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
	
}; //BLDC

#endif //__BLDC_H__
