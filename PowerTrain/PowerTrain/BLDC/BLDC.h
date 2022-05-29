/* 
* BLDC.h
*
* Created: 16.05.2022 21:59:20
* Author: dominik hellhake
*/
#ifndef __BLDC_H__
#define __BLDC_H__

#include "..\Task.h"
#include "..\DeviceDriver\TCC\TCClib.h"
#include "..\BLDC\BLDCPattern.h"
#include "..\HallSensor\Hall.h"
#include "..\HallSensor\HallSensor.h"

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
		float Tar_Duty = 2.0f;
		float Max_Duty = 14.0f;
	
		uint8_t once = 0;
		inline void Drive_SetPhase(HALL_STATE state)
		{
			if (Hall.AvlWheelSpeedStability == 0)
			{				
				TCC0_SetDuty(this->Tar_Duty, TCC0_CC_U_IDX);
				TCC0_SetDuty(this->Tar_Duty, TCC0_CC_V_IDX);
				TCC0_SetDuty(this->Tar_Duty, TCC0_CC_W_IDX);
			}
			
			uint32_t pattern = GetPattern(state, DrvDir_Forward, Hall.AvlWheelSpeedStability);						
			TCC0_SetPattern(pattern);			
		}
		
		float sineTarDuty[120];		
	private:
		float sine120[120] {
			0.034899497f, 0.069756474f, 0.104528463f, 0.139173101f, 0.173648178f, 0.207911691f, 0.241921896f,
			0.275637356f, 0.309016994f, 0.342020143f, 0.374606593f, 0.406736643f, 0.438371147f, 0.469471563f, 0.500000000f,
			0.529919264f, 0.559192903f, 0.587785252f, 0.615661475f, 0.642787610f, 0.669130606f, 0.694658370f, 0.719339800f,
			0.743144825f, 0.766044443f, 0.788010754f, 0.809016994f, 0.829037573f, 0.848048096f, 0.866025404f, 0.882947593f,
			0.898794046f, 0.913545458f, 0.927183855f, 0.939692621f, 0.951056516f, 0.961261696f, 0.970295726f, 0.978147601f,
			0.984807753f, 0.990268069f, 0.994521895f, 0.997564050f, 0.999390827f, 1.000000000f, 0.999390827f, 0.997564050f,
			0.994521895f, 0.990268069f, 0.984807753f, 0.978147601f, 0.970295726f, 0.961261696f, 0.951056516f, 0.939692621f,
			0.927183855f, 0.913545458f, 0.898794046f, 0.882947593f, 0.866025404f, 0.882947593f, 0.898794046f, 0.913545458f,
			0.927183855f, 0.939692621f, 0.951056516f, 0.961261696f, 0.970295726f, 0.978147601f, 0.984807753f, 0.990268069f,
			0.994521895f, 0.997564050f, 0.999390827f, 1.000000000f, 0.999390827f, 0.997564050f, 0.994521895f, 0.990268069f,
			0.984807753f, 0.978147601f, 0.970295726f, 0.961261696f, 0.951056516f, 0.939692621f, 0.927183855f, 0.913545458f,
			0.898794046f, 0.882947593f, 0.866025404f, 0.848048096f, 0.829037573f, 0.809016994f, 0.788010754f, 0.766044443f,
			0.743144825f, 0.719339800f, 0.694658370f, 0.669130606f, 0.642787610f, 0.615661475f, 0.587785252f, 0.559192903f,
			0.529919264f, 0.500000000f, 0.469471563f, 0.438371147f, 0.406736643f, 0.374606593f, 0.342020143f, 0.309016994f,
			0.275637356f, 0.241921896f, 0.207911691f, 0.173648178f, 0.139173101f, 0.104528463f, 0.069756474f, 0.034899497f, 0.000000000f
		};

	
}; //BLDC

extern BLDC DRV;

#endif //__BLDC_H__
