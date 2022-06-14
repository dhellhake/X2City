/*
 * BLDCPattern.h
 *
 * Created: 21.05.2022 11:13:09
 *  Author: domin
 */
#ifndef BLDCPATTERN_H_
#define BLDCPATTERN_H_

#include "samc21.h"
#include "..\HallSensor\Hall.h"

#define U_HIGH_PATT_Pos		3
#define V_HIGH_PATT_Pos		2
#define W_HIGH_PATT_Pos		0
#define U_LOW_PATT_Pos		7
#define V_LOW_PATT_Pos		6
#define W_LOW_PATT_Pos		4


#define BLCD_ACC_PWM_PATT_1	(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_PWM_PATT_2	(((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_PWM_PATT_3	(((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_PWM_PATT_4	(((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_PWM_PATT_5	(((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_PWM_PATT_6	(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);


#define BLCD_ACC_SIN_PATT_1	(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_SIN_PATT_2	(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
							  
#define BLCD_ACC_SIN_PATT_3	(((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_SIN_PATT_4	(((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);						

#define BLCD_ACC_SIN_PATT_5	(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_ACC_SIN_PATT_6	(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
 			  				  

							  

#define BLCD_BRK_PATT_1		(((1 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (1 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (0 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (0 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#define BLCD_PATT_DISABLED  (((1 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos)	| \
							  (1 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (1 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							(((0 << U_HIGH_PATT_Pos) | \
							  (1 << U_LOW_PATT_Pos) | \
							  (0 << V_HIGH_PATT_Pos) | \
							  (1 << V_LOW_PATT_Pos) | \
							  (0 << W_HIGH_PATT_Pos) | \
							  (1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);

#endif /* BLDCPATTERN_H_ */