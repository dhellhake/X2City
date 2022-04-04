/* 
* ComHandler.cpp
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\Peripheral\DMAC\DMAClib.h"
#include "..\HallSensor\HallSensor.h"

ComHandler ComHdl;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{	
	//this->Record.Avl_DRV_State = (uint32_t)DRV.State;
	//this->Record.Tar_Duty = DRV.Tar_Duty;
	this->Record.Tar_Duty = 123.45f;
	
	this->Record.Avl_AvgHallStateInterval = Hall.Avl_AvgHallStateInterval;
	this->Record.Avl_Ticks = Hall.Avl_TriggerCnt;
	this->Record.Avl_TicksPerSecond = Hall.Avl_TicksPerSecond;
	//this->Record.Avl_DriveDirection = (uint16_t)DRV.Tar_DriveDirecton;
	this->Record.AVL_HallState = (uint32_t)Hall.Avl_HallState;
	
	DMAC_ChannelStartTransfer(DMA_CHID_COM);
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
