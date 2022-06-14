/* 
* BLDC.cpp
*
* Created: 16.05.2022 21:59:19
* Author: dominik hellhake
*/
#include "BLDC.h"
#include "..\HallSensor\Hall.h"
#include "..\HallSensor\HallSensor.h"

BLDC DRV;

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
RUN_RESULT BLDC::Run(uint32_t timeStamp)
{
	if (Hall.AvgHallStateInterval != 0)
		if (this->Max_Duty > this->Tar_Duty)
		{			
			this->Tar_Duty += 0.0005f;
			for (uint8_t x = 0; x < 120; x++)
				this->TarSineDuty[x] = (uint32_t)(this->Tar_Duty * sine120[x] * 1200.0f);
		}
	
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
BLDC::BLDC()
{
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_1] = (uint32_t)BLCD_PATT_DISABLED;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_1] = (uint32_t)BLCD_ACC_PWM_PATT_4;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_2] = (uint32_t)BLCD_ACC_PWM_PATT_5;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_3] = (uint32_t)BLCD_ACC_PWM_PATT_6;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_4] = (uint32_t)BLCD_ACC_PWM_PATT_1;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_5] = (uint32_t)BLCD_ACC_PWM_PATT_2;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_6] = (uint32_t)BLCD_ACC_PWM_PATT_3;
	this->BLDC_TrapezoidalForwardPattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_2] = (uint32_t)BLCD_PATT_DISABLED;
	
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_1] = (uint32_t)BLCD_PATT_DISABLED;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_1] = (uint32_t)BLCD_ACC_PWM_PATT_1;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_2] = (uint32_t)BLCD_ACC_PWM_PATT_2;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_3] = (uint32_t)BLCD_ACC_PWM_PATT_3;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_4] = (uint32_t)BLCD_ACC_PWM_PATT_4;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_5] = (uint32_t)BLCD_ACC_PWM_PATT_5;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_6] = (uint32_t)BLCD_ACC_PWM_PATT_6;
	this->BLDC_TrapezoidalReversePattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_2] = (uint32_t)BLCD_PATT_DISABLED;
	
		
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_1] = (uint32_t)BLCD_PATT_DISABLED;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_1] = (uint32_t)BLCD_ACC_SIN_PATT_4;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_2] = (uint32_t)BLCD_ACC_SIN_PATT_5;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_3] = (uint32_t)BLCD_ACC_SIN_PATT_6;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_4] = (uint32_t)BLCD_ACC_SIN_PATT_1;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_5] = (uint32_t)BLCD_ACC_SIN_PATT_2;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_6] = (uint32_t)BLCD_ACC_SIN_PATT_3;
	this->BLDC_SinusoidalForwardPattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_2] = (uint32_t)BLCD_PATT_DISABLED;
	
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_1] = (uint32_t)BLCD_PATT_DISABLED;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_1] = (uint32_t)BLCD_ACC_SIN_PATT_1;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_2] = (uint32_t)BLCD_ACC_SIN_PATT_2;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_3] = (uint32_t)BLCD_ACC_SIN_PATT_3;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_4] = (uint32_t)BLCD_ACC_SIN_PATT_4;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_5] = (uint32_t)BLCD_ACC_SIN_PATT_5;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::HALL_STATE_6] = (uint32_t)BLCD_ACC_SIN_PATT_6;
	this->BLDC_SinusoidalReversePattern_LuT[(uint8_t)HALL_STATE::UNDEFINED_2] = (uint32_t)BLCD_PATT_DISABLED;
}