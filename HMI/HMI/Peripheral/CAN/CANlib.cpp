/* 
* CANlib.cpp
*
* Created: 16.01.2022 10:02:22
* Author: dominik hellhake
*/
#include <string.h>
#include "CANlib.h"
#include "..\..\ComHandler\ComHandler.h"

#define COMPILER_ALIGNED(a)        __attribute__((__aligned__(a)))

COMPILER_ALIGNED(4)
static CanMramSidfe can0_rx_standard_filter[2];
COMPILER_ALIGNED(4)
static CanMramRxbe  can0_rx_buffer[2];
COMPILER_ALIGNED(4)
static CanMramTxbe  can0_tx_buffer;
COMPILER_ALIGNED(4)
static CanMramTxefe can0_tx_event_fifo;

static const uint8_t dlc_to_size[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

// Nominal bit rate. The time quanta is 48 MHz / (5+1) = 8MHz.
// And each bit is (1 + NTSEG1 + 1 + NTSEG2 + 1) = 16 time quanta
// which means the bit rate is 8MHz / 16 = 500 KHz.
#define NBTP_NBRP_VALUE       5  // Nominal bit Baud Rate Prescaler
#define NBTP_NSJW_VALUE       3  // Nominal bit (Re)Synchronization Jump Width
#define NBTP_NTSEG1_VALUE     10 // Nominal bit Time segment before sample point
#define NBTP_NTSEG2_VALUE     3  // Nominal bit Time segment after sample point

// Data bit rate. The time quanta is 48 MHz / (0+1) =  48MHz.
// And each bit is (1 + DTSEG1 + 1 + DTSEG2 + 1) = 16 time quanta
// which means the bit rate is 48 MHz / 16 = 3 MHz.
#define DBTP_DBRP_VALUE       0  // Data bit Baud Rate Prescaler
#define DBTP_DSJW_VALUE       3  // Data bit (Re)Synchronization Jump Width
#define DBTP_DTSEG1_VALUE     10 // Data bit Time segment before sample point
#define DBTP_DTSEG2_VALUE     3  // Data bit Time segment after sample point

#define CAN_FILTER_ID_MOTORCONTROL			0x45a
#define CAN_FILTER_ID_BATTERYCONTROL        0x25a

void InitCAN0()
{
	/* Enable Clock for CAN0 */
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_CAN0;
	
	// Disable the peripheral channel 
	GCLK->PCHCTRL[CAN0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[CAN0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	// Configure the peripheral channel
	GCLK->PCHCTRL[CAN0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[CAN0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	/* Configure CAN0 Peripheral */	
	CAN0->CCCR.reg =	CAN_CCCR_INIT |												// Initialization
						CAN_CCCR_CCE;												// Configuration Change Enable
	CAN0->CCCR.reg =	CAN_CCCR_INIT | 											// Initialization
						CAN_CCCR_CCE | 												// Configuration Change Enable
						CAN_CCCR_TXP |												// Transmit Pause
						CAN_CCCR_EFBI |												// Edge Filtering during Bus Integration
						CAN_CCCR_PXHD |												// Protocol Exception Handling Disable
						CAN_CCCR_DAR |												// Disable Automatic Retransmission
						CAN_CCCR_FDOE |												// FD Operation Enable
						CAN_CCCR_BRSE;												// Bit Rate Switch Enable
	CAN0->SIDFC.reg =	CAN_SIDFC_FLSSA((uint32_t)&can0_rx_standard_filter) |		// Filter List Standard Start Address
						CAN_SIDFC_LSS(2);											// List Size Standard
	CAN0->XIDFC.reg =	CAN_XIDFC_LSE(0);											// List Size Extended

	CAN0->RXF0C.reg =	CAN_RXF0C_F0OM |											// FIFO 0 Operation Mode
						CAN_RXF0C_F0WM(0);											// Rx FIFO 0 Watermark (interrupt)
	CAN0->RXF1C.reg =	CAN_RXF1C_F1OM |											// FIFO 1 Operation Mode
						CAN_RXF1C_F1WM(0);											// Rx FIFO 1 Watermark (interrupt)

	CAN0->RXBC.reg =	CAN_RXBC_RBSA((uint32_t)&can0_rx_buffer);					// Rx Buffer Start Address
	CAN0->TXBC.reg =	CAN_TXBC_TBSA((uint32_t)&can0_tx_buffer) |					// Tx Buffers Start Address
						CAN_TXBC_NDTB(1);											// Number of Dedicated Transmit Buffers
	CAN0->TXEFC.reg =	CAN_TXEFC_EFSA((uint32_t)&can0_tx_event_fifo) |				// Event FIFO Start Address
						CAN_TXEFC_EFS(1);											// Event FIFO Size							CAN0->RXESC.reg =	CAN_RXESC_RBDS_DATA64 |										// Rx Buffer Data Field Size
						CAN_RXESC_F0DS_DATA64 |										// Rx FIFO 0 Data Field Size
						CAN_RXESC_F1DS_DATA64;										// Rx FIFO 1 Data Field Size
	CAN0->TXESC.reg =	CAN_TXESC_TBDS_DATA64;										// Tx Buffer Data Field Size
	
	CAN0->TSCC.reg =	CAN_TSCC_TCP(0) |											// Timestamp Counter Prescaler
						CAN_TSCC_TSS_INC_Val;										// Timestamp Select
	CAN0->TDCR.reg =	CAN_TDCR_TDCO(0) |											// Transmitter Delay Compensation Offset
						CAN_TDCR_TDCF(0);											// Transmitter Delay Compensation Filter Window Length
						
	CAN0->GFC.reg =		CAN_GFC_ANFS_REJECT |										// Accept Non-matching Frames Standard
						CAN_GFC_ANFE_REJECT |										// Accept Non-matching Frames Extended
						CAN_GFC_RRFS |												// Reject Remote Frames Standard
						CAN_GFC_RRFE;												// Reject Remote Frames Extended
	
	CAN0->XIDAM.reg =	CAN_XIDAM_MASK;												// Extended ID Mask		CAN0->TXBTIE.reg =	CAN_TXBTIE_MASK;											// Transmission Interrupt Enable
	CAN0->TXBCIE.reg =	CAN_TXBCIE_MASK;											// Cancellation Finished Interrupt Enable
	CAN0->NBTP.reg =	CAN_NBTP_NBRP(NBTP_NBRP_VALUE) |							// Nominal Baud Rate Prescaler
						CAN_NBTP_NSJW(NBTP_NSJW_VALUE) |							// Nominal (Re)Syncronization Jump Width
						CAN_NBTP_NTSEG1(NBTP_NTSEG1_VALUE) |						// Nominal Time segment before sample point
						CAN_NBTP_NTSEG2(NBTP_NTSEG2_VALUE);							// Time segment after sample point	
	CAN0->DBTP.reg =	CAN_DBTP_DBRP(DBTP_DBRP_VALUE) |							// Data Baud Rate Prescaler
						CAN_DBTP_DSJW(DBTP_DSJW_VALUE) |							// Data (Re)Syncronization Jump Width
						CAN_DBTP_DTSEG1(DBTP_DTSEG1_VALUE) |						// Fast time segment before sample point
						CAN_DBTP_DTSEG2(DBTP_DTSEG2_VALUE);							// Data time segment after sample point
	CAN0->IE.reg =		CAN_IE_DRXE;												// Message stored to Dedicated Rx Buffer Interrupt Enable
	CAN0->ILE.reg =		CAN_ILE_EINT0;
	
	CAN0->CCCR.reg &= ~CAN_CCCR_INIT;												// Configuration Change Enable
	while (CAN0->CCCR.reg & CAN_CCCR_INIT);

	can0_rx_standard_filter[0].SIDFE_0.reg =	CAN_SIDFE_0_SFID2(0) |								// Standard Filter ID 2
												CAN_SIDFE_0_SFID1(CAN_FILTER_ID_MOTORCONTROL) |		// First ID of standard ID filter element
												CAN_SIDFE_0_SFT_CLASSIC |							// Classic filter: SFID1 = filter, SFID2 = mask
												//CAN_SIDFE_0_SFEC_STRXBUF;							// Store into Rx Buffer or as debug message, configuration of SFT[1:0] ignored
												(0b111 << CAN_SIDFE_0_SFEC_Pos);

	can0_rx_standard_filter[1].SIDFE_0.reg =	CAN_SIDFE_0_SFID2(1) |								// Standard Filter ID 2
												CAN_SIDFE_0_SFID1(CAN_FILTER_ID_BATTERYCONTROL) |	// First ID of standard ID filter element
												CAN_SIDFE_0_SFT_CLASSIC |							// Classic filter: SFID1 = filter, SFID2 = mask
												//CAN_SIDFE_0_SFEC_STRXBUF;							// Store into Rx Buffer or as debug message, configuration of SFT[1:0] ignored
												(0b111 << CAN_SIDFE_0_SFEC_Pos);

	NVIC_SetPriority(CAN0_IRQn, 1);	// Set interrupt priority to highest urgency
	NVIC_EnableIRQ(CAN0_IRQn);		// Enable SysTick Interrupt
}

void can0_transmit(uint8_t size, uint8_t* data)
{
	for (uint8_t dIdx = 0; dIdx < size; dIdx++)
		((uint8_t *)can0_tx_buffer.TXBE_DATA)[dIdx] = data[dIdx];
	
	can0_tx_buffer.TXBE_0.reg =		CAN_TXBE_0_ID(CAN_FILTER_ID_MOTORCONTROL << 18);			// Message ID
	can0_tx_buffer.TXBE_1.reg =		CAN_TXBE_1_FDF |							// Frame transmitted in CAN FD format
									CAN_TXBE_1_BRS |							// CAN FD frames transmitted with bit rate switching.
									CAN_TXBE_1_DLC(15);							// Data Lenght Code (64 bytes)

	CAN0->TXBAR.reg = CAN_TXBAR_AR0;											// Add Transmit Request

	while ((CAN0->IR.reg & (CAN_IR_TC | CAN_IR_TOO | CAN_IR_PEA | CAN_IR_PED)) == 0x00);

	CAN0->IR.reg = (CAN_IR_TC | CAN_IR_TOO | CAN_IR_PEA | CAN_IR_PED);
}


void CAN0_Handler()
{
	// Clear Message stored to Dedicated Rx Buffer Interrupt
	CAN0->IR.reg = CAN_IR_DRX;
	
	if (CAN0->NDAT1.bit.ND0 == 0b1)
	{		
		memcpy(ComHdl.can_motorcontrol_message_buffer, (uint8_t *)can0_rx_buffer[0].RXBE_DATA, 64);
		
		// Clear new Data Available in Buffer indicator
		CAN0->NDAT1.reg = CAN_NDAT1_ND0;
	}
	
	if (CAN0->NDAT1.bit.ND1 == 0b1)
	{
		memcpy(ComHdl.can_batterycontrol_message_buffer, (uint8_t *)can0_rx_buffer[1].RXBE_DATA, 64);
		
		// Clear new Data Available in Buffer indicator
		CAN0->NDAT1.reg = CAN_NDAT1_ND1;
	}
	
}