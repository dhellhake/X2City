/* 
* ComPort.cpp
*
* Created: 04.09.2021 18:37:08
* Author: dominik hellhake
*/
#include "ComPort.h"
#include "..\Peripheral\DMAC\DMAClib.h"

ComPort Com;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComPort::Run(uint32_t timeStamp)
{
	//this->Record.Avl_DRV_State = (uint32_t)DRV.State;
	//this->Record.Tar_Duty = DRV.Tar_Duty;
	this->Record.Tar_Duty = 123.45f;
	
	//this->Record.Avl_AvgHallStateInterval = Hall.Avl_AvgHallStateInterval;
	//this->Record.Avl_Ticks = Hall.Avl_TriggerCnt;
	//this->Record.Avl_TicksPerSecond = Hall.Avl_TicksPerSecond;
	//this->Record.Avl_DriveDirection = (uint16_t)DRV.Tar_DriveDirecton;
		
	DMAC_ChannelStartTransfer(DMA_CHID_COM);
	
	return RUN_RESULT::SUCCESS;
}
