/* 
* BLDCDrive.h
*
* Created: 14.11.2021 12:04:17
* Author: dominik hellhake
*/
#ifndef __BLDCDRIVE_H__
#define __BLDCDRIVE_H__

#include "..\Task.h"

class BLDCDrive : public Task
{	
	/************************************************************************/
	/* Task Interface implementation										*/
	/************************************************************************/
	public:
		virtual RUN_RESULT Run(uint32_t timeStamp);
}; //BLDCDrive

extern BLDCDrive DRV;

#endif //__BLDCDRIVE_H__
