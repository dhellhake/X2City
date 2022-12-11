/* 
* ComHandler.h
*
* Created: 16.01.2022 13:04:30
* Author: dominik hellhake
*/
#ifndef __COMHANDLER_H__
#define __COMHANDLER_H__

#include "sam.h"
#include "..\Task.h"

typedef struct ADS_DATASET
{
	uint32_t tstmp;
	float VBat = 0.0f;
	
	float IBat0 = 0.0f;
	float IBat1 = 0.1f;
	float IBat2 = 0.2f;
	float IBat3 = 0.7f;
} ADS_DATASET;


class ComHandler : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		float tmpVal = 0.0f;		
		uint8_t tick = 0x00;
		
		uint8_t DataRdy = 0;
		ADS_DATASET TxBuffer;
	
}; //ComHandler

extern ComHandler ComHdl;

#endif //__COMHANDLER_H__
