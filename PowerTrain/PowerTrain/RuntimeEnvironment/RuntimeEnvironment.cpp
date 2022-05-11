/* 
* RuntimeEnvironment.cpp
*
* Created: 11.05.2022 20:43:40
* Author: dominik hellhake
*/
#include "RuntimeEnvironment.h"
#include "..\DeviceDriver\KSZ8851\KSZ8851.h"

RuntimeEnvironment Rte;

RuntimeEnvironment::RuntimeEnvironment()
{
	this->Record = (rte_image_record_t *)TxPacketBuffer;
}