/*
 * DRV8323Config.h
 *
 * Created: 01.09.2021 17:41:57
 *  Author: dominik hellhake
 */ 
#ifndef DRV8323CONFIG_H_
#define DRV8323CONFIG_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "samc21.h"

// Configure Driver Control Register	(address = 0x02)
// [9] DIS_CPUV			0b0		(UVLO fault enabled)
// [8] DIS_GDF			0b0		(Gate Drive fault enabled)
// [7] OTW_REP			0b0		(OverTemp fault causes nFAULT/FAULT)
// [6-5] PWM_MODE		0b0		(6x PWM Mode)
// [4] 1PWM_COM			0b0		(synchronous rectification)
// [3] 1PWM_DIR			0b0		(ORed with the INHC (direction))
// [2] COAST			0b0		(Set 1 to put all MOSFETs in the Hi-Z state)
// [1] BRAKE			0b0		(ORed with the INLC (BRAKE))
// [0] CLR_FLT			0b0		(Set to 1 to to clear latched fault)
// => 0x0 / 0
#define DRV8323_DCR_Addr		0x02
#define DRV8323_DCR_Val			0b00000000000

// Configure Gate Drive HS Register		(address = 0x03)
// [10-8] LOCK			0b011	(unlock)
// [7-4] IDRIVEP_HS		0b0011	(80mA)
// [3-0] IDRIVEN_HS		0b0010  (120mA)
// => 0x332 / 818
#define DRV8323_GDHS_Addr		0x03
#define DRV8323_GDHS_Val		0b01100110010

// Configure Gate Drive LS Register		(address = 0x04)
// [10] CBC				0b1
// [9-8] TDRIVE			0b11	(500ns peak gate-current drive time)
// [7-4] IDRIVEP_LS		0b0011  (80mA)
// [3-0] IDRIVEN_LS		0b0010  (120mA)
// => 0x432 / 1074
#define DRV8323_GDLS_Addr		0x04
#define DRV8323_GDLS_Val		0b10000110010

// Configure OCP Control Register		(address = 0x05)
// [10] TRETRY			0b0		(retry time is 4 ms)
// [9-8] DEAD_TIME		0b00	(50ns dead time)
// [7-6] OCP_MODE		0b00	(OC causes latched fault)
// [5-4] OCP_DEG		0b01	(OC deglitch time is 4us)
// [3-0] VDS_LVL		0b0000	(0.06V threashold => 35A)
// => 0x10 / 16
#define DRV8323_OCPCR_Addr		0x05
#define DRV8323_OCPCR_Val		0b00000010000


uint16_t WriteRegister(uint8_t addr, uint16_t value);
uint16_t ReadRegister(uint8_t addr);

uint8_t WriteConfiguration();
uint8_t VerifyConfiguration();

#ifdef __cplusplus
	}
#endif

#endif /* DRV8323CONFIG_H_ */