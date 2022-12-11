/* 
* ADS1118.cpp
*
* Created: 18.07.2022 22:14:39
* Author: dominik hellhake
*/
#include "ADS1118.h"
#include "..\..\ComHandler\ComHandler.h"
#include "..\..\Peripheral\SERCOM\SERCOMlib.h"

ADS1118 ADS;


/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ADS1118::Run(uint32_t timeStamp)
{
	int16_t raw = 0x0000;
	
	switch (this->CurrentPort)
	{
		case ADS1118_PORT::PORT_0:
			raw = this->ReadPortAndSetConfig(&this->Config[0], 0);
			ComHdl.TxBuffer.VBat = raw  * 0.000125f * 2.021697f;		
			this->CurrentPort = ADS1118_PORT::PORT_1;
		break;
		case ADS1118_PORT::PORT_1:
			raw = this->ReadPortAndSetConfig(&this->Config[2], 1);
			ComHdl.TxBuffer.IBat0 = raw * 0.0000078125f;
			ComHdl.TxBuffer.IBat0 /= 0.099866733f;
			this->CurrentPort = ADS1118_PORT::PORT_2;
		break;
		case ADS1118_PORT::PORT_2:
			raw = this->ReadPortAndSetConfig(&this->Config[4], 1);
			ComHdl.TxBuffer.IBat1 = raw * 0.0000078125f;
			ComHdl.TxBuffer.IBat1 /= 0.099739126f;
			this->CurrentPort = ADS1118_PORT::PORT_3;
		break;
		case ADS1118_PORT::PORT_3:
			raw = this->ReadPortAndSetConfig(&this->Config[6], 1);
			ComHdl.TxBuffer.IBat2 = raw * 0.0000078125f;
			ComHdl.TxBuffer.IBat2 /= 0.100773727f;
			this->CurrentPort = ADS1118_PORT::PORT_4;
		break;
		case ADS1118_PORT::PORT_4:
			raw = this->ReadPortAndSetConfig(&this->Config[8], 1);
			ComHdl.TxBuffer.IBat3 = raw * 0.0000078125f;
			ComHdl.TxBuffer.IBat3 /= 0.099773098f;
			
			ComHdl.TxBuffer.tstmp = timeStamp;
			ComHdl.DataRdy = 0x01;
			
			this->CurrentPort = ADS1118_PORT::PORT_0;
		break;
		default:
		break;
	}
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
int16_t ADS1118::ReadPortAndSetConfig(uint8_t* config, uint8_t adsIdx)
{
	uint8_t respH, respL;
	
	if (adsIdx == 0)
		PORT->Group[0].OUTCLR.reg = PORT_PA19;
	else if (adsIdx == 1)
		PORT->Group[0].OUTCLR.reg = PORT_PA20;
	
	respH = SERCOM1_TransmitByte(config[1]);
	respL = SERCOM1_TransmitByte(config[0]);
	SERCOM1_TransmitByte(config[1]);
	SERCOM1_TransmitByte(config[0]);
	
	if (adsIdx == 0)
		PORT->Group[0].OUTSET.reg = PORT_PA19;
	else if (adsIdx == 1)
		PORT->Group[0].OUTSET.reg = PORT_PA20;
	
	return (respH << 8) | respL;
}