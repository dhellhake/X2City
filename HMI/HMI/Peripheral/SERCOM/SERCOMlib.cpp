/* 
* SERCOMlib.cpp
*
* Created: 20.09.2020 13:55:34
* Author: dominik hellhake
*/
#include "SERCOMlib.h"

/**
 * \brief Calculate \f$ \left\lceil \frac{a}{b} \right\rceil \f$ using
 * integer arithmetic.
 *
 * \param[in] a An integer
 * \param[in] b Another integer
 *
 * \return (\a a / \a b) rounded up to the nearest integer.
 */
#define div_ceil(a, b)      (((a) + (b) - 1) / (b))

void InitSERCOM1()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM1_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_SLOW].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM1_GCLK_ID_SLOW].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_SLOW].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_SLOW].reg |= GCLK_PCHCTRL_CHEN;
		
	SERCOM1->I2CM.CTRLA.reg =	(0x0 << SERCOM_I2CM_CTRLA_LOWTOUTEN_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_INACTOUT_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_SCLSM_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_SPEED_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_SEXTTOEN_Pos) |
								(0x3 << SERCOM_I2CM_CTRLA_SDAHOLD_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_RUNSTDBY_Pos) |
								(0x5 << SERCOM_I2CM_CTRLA_MODE_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_ENABLE_Pos) |
								(0x0 << SERCOM_I2CM_CTRLA_SWRST_Pos);
								
	SERCOM1->I2CM.CTRLB.reg =	(0x0 << SERCOM_I2CM_CTRLB_ACKACT_Pos) |
								(0x0 << SERCOM_I2CM_CTRLB_CMD_Pos) |
								(0x0 << SERCOM_I2CM_CTRLB_QCEN_Pos) |
								(0x1 << SERCOM_I2CM_CTRLB_SMEN_Pos);
	
	uint32_t fgclk		= 48000000;
	uint32_t fscl		= 400000;
	uint32_t trise		= 215; //ns
	uint32_t tmp_baud = (int32_t)(div_ceil(fgclk - fscl * (10 + (fgclk * 0.000000001) * trise), 2 * fscl));
							
	SERCOM1->I2CM.BAUD.reg =	(0x0 << SERCOM_I2CM_BAUD_HSBAUDLOW_Pos) | 
								(0x0 << SERCOM_I2CM_BAUD_HSBAUD_Pos) |
								(0x0 << SERCOM_I2CM_BAUD_BAUDLOW_Pos) |
								(tmp_baud << SERCOM_I2CM_BAUD_BAUD_Pos);							
					
	/* Wait for I2C module to sync. */			
	while (SERCOM1->I2CM.SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK);
								
	SERCOM1->I2CM.CTRLA.reg |=	(0x1 << SERCOM_I2CM_CTRLA_ENABLE_Pos);
	
	/* Force bus state to idle */
	SERCOM1->I2CM.STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(1);
}

uint8_t SERCOM1_I2CStart(uint8_t address)
{
	uint16_t timeOut;
	
	/* Wait for I2C module to sync. */
	timeOut = 0xFFFF;
	while (SERCOM1->I2CM.SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK)
	{
		timeOut--;
		if (timeOut == 0)
		return 0;
	}
	
	SERCOM1->I2CM.ADDR.reg =	(0x0 << SERCOM_I2CM_ADDR_LEN_Pos) |
								(0x0 << SERCOM_I2CM_ADDR_TENBITEN_Pos) |
								(0x0 << SERCOM_I2CM_ADDR_HS_Pos) |
								(0x0 << SERCOM_I2CM_ADDR_LENEN_Pos) |
								(address  << SERCOM_I2CM_ADDR_ADDR_Pos);
	
	/* Wait for Master On Bus */
	timeOut = 0xFFFF;
	while (!(SERCOM1->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
	{
		timeOut--;
		if (timeOut == 0)
		return 0;
	}
	
	return 1;
}

uint8_t SERCOM1_I2CSendByte(uint8_t data)
{
	uint16_t timeOut;
	
	if (!(SERCOM1->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(2)))
	return 0;	// bus ownership lost / ERR_PACKET_COLLISION
	
	/* Wait for I2C module to sync. */
	timeOut = 0xFFFF;
	while (SERCOM1->I2CM.SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK)
	{
		timeOut--;
		if (timeOut == 0)
		return 0;
	}
	
	SERCOM1->I2CM.DATA.reg = data;
	
	/* Wait for Master On Bus */
	timeOut = 0xFFFF;
	while (!(SERCOM1->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
	{
		timeOut--;
		if (timeOut == 0)
		return 0;
	}
	
	if (SERCOM1->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
	return 0;	// data not acknowledged

	return 1;
}

uint8_t SERCOM1_I2CStop()
{
	SERCOM1->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
	return 1;
}