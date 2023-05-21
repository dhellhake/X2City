/* 
* CANlib.cpp
*
* Created: 16.01.2022 10:02:22
* Author: dominik hellhake
*/
#include <stdalign.h>
#include "CANlib.h"

#define CAN0_RX_FIFO_SIZE			16
#define CAN0_RX_FIFO_ELEMENT_SIZE	4

// Nominal bit rate. The time quanta is 48 MHz / (5+1) = 8MHz.
// And each bit is (1 + NTSEG1 + 1 + NTSEG2 + 1) = 16 time quanta
// which means the bit rate is 8MHz / 16 = 500 KHz.
#define NBTP_NBRP_VALUE       5  // Nominal bit Baud Rate Prescaler
#define NBTP_NSJW_VALUE       3  // Nominal bit (Re)Synchronization Jump Width
#define NBTP_NTSEG1_VALUE     10 // Nominal bit Time segment before sample point
#define NBTP_NTSEG2_VALUE     3  // Nominal bit Time segment after sample point

#define COMPILER_ALIGNED(a)        __attribute__((__aligned__(a)))
COMPILER_ALIGNED(4)
static CanMramSidfe can0_rx_standard_filter[1];
COMPILER_ALIGNED(4)
static CanMramTxbe  can0_tx_buffer;
COMPILER_ALIGNED(4)
static CanMramTxefe can0_tx_event_fifo;

volatile uint32_t can0_rx_fifo0[CAN0_RX_FIFO_SIZE * CAN0_RX_FIFO_ELEMENT_SIZE];


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
	
	
	/* Configure CC Control register */	
	CAN0->CCCR.reg =	CAN_CCCR_INIT |												// Initialization
						CAN_CCCR_CCE;												// Configuration Change Enable
						
	CAN0->CCCR.reg =	(0 << CAN_CCCR_NISO_Pos) |									// CAN-FD: ISO Operation
						(1 << CAN_CCCR_TXP_Pos) |									// Transmit pause enabled
						(1 << CAN_CCCR_EFBI_Pos) |									// Edge Filtering during Bus Integration
						(1 << CAN_CCCR_PXHD_Pos) |									// Protocol Exception Handling Disable
						(0 << CAN_CCCR_BRSE_Pos) |									// CAN-FD: Disable Bit Rate Switch Enable
						(0 << CAN_CCCR_FDOE_Pos) |									// CAN-FD: Disable FD Operation
						(0 << CAN_CCCR_TEST_Pos) |									// Disable Test Mode
						(1 << CAN_CCCR_DAR_Pos) |									// Disable Automatic Retransmission
						(0 << CAN_CCCR_MON_Pos) |									// Disable Bus Monitoring Mode
						(0 << CAN_CCCR_CSR_Pos) |									// Disable Clock Stop Request
						(0 << CAN_CCCR_CSA_Pos) |									// Disable Clock Stop Acknowledge
						(0 << CAN_CCCR_ASM_Pos) |									// Normal CAN operation
						(1 << CAN_CCCR_CCE_Pos) |									// Configuration Change Enable
						(1 << CAN_CCCR_INIT_Pos);									// Initialization
	
	/* Configure  Timestamp Counter Configuration register */
	CAN0->TSCC.reg =	CAN_TSCC_TCP(0) |											// Timestamp Counter Prescaler
						CAN_TSCC_TSS_INC_Val;										// Timestamp Select
	
	/* Configure Interrupt Enable register */
	CAN0->IE.reg =		CAN_IE_RF0FE;												// Message stored to Dedicated Rx Buffer Interrupt Enable
	/* Configure Interrupt Line Enable register */
	CAN0->ILE.reg =		CAN_ILE_EINT0 | CAN_ILE_EINT1;
	
	
		
	/* Rx Configuration */
	/* Configure Transmitter Delay Compensation register */
	CAN0->GFC.reg =		CAN_GFC_ANFS_REJECT |										// Accept Non-matching Frames Standard
						CAN_GFC_ANFE_REJECT |										// Accept Non-matching Frames Extended
						CAN_GFC_RRFS |												// Reject Remote Frames Standard
						CAN_GFC_RRFE;												// Reject Remote Frames Extended
		
	/* Configure Extended ID AND Mask register */
	CAN0->XIDAM.reg =	CAN_XIDAM_MASK;												// Extended ID Mask
	
	/* Configure Standard ID Filter Configuration register */	
	CAN0->SIDFC.reg =	CAN_SIDFC_FLSSA((uint32_t)&can0_rx_standard_filter) |		// Filter List Standard Start Address
						CAN_SIDFC_LSS(1);											// List Size Standard
	
	/* Configure Extended ID Filter Configuration register */
	CAN0->XIDFC.reg =	CAN_XIDFC_LSE(0);											// List Size Extended

	/* Configure  Rx FIFO 0 Configuration register */
	CAN0->RXF0C.reg =	CAN_RXF0C_F0WM(0) |											// Rx FIFO 0 Watermark (interrupt)
						CAN_RXF0C_F0S(CAN0_RX_FIFO_SIZE) |							// Rx FIFO 0 size
						CAN_RXF0C_F0SA((uint32_t)&can0_rx_fifo0);					// Rx FIFO 0 Address
	
	/* Configure Rx FIFO 1 Configuration register */	
	CAN0->RXF1C.reg =	CAN_RXF1C_F1OM |											// FIFO 1 Operation Mode : overwrite
						CAN_RXF1C_F1WM(0);											// Rx FIFO 1 Watermark (interrupt)
	
	/* Configure Rx Buffer / FIFO Element Size Configuration register */
	CAN0->RXESC.reg =	CAN_RXESC_RBDS_DATA8 |										// Rx Buffer Data Field Size
						CAN_RXESC_F0DS_DATA8 |										// Rx FIFO 0 Data Field Size
						CAN_RXESC_F1DS_DATA8;										// Rx FIFO 1 Data Field Size
	
	
	
	/* Tx Configuration */
	/* Configure Tx Buffer Configuration register */
	CAN0->TXBC.reg =	CAN_TXBC_TBSA((uint32_t)&can0_tx_buffer) |					// Tx Buffers Start Address
						CAN_TXBC_NDTB(1);											// Number of Dedicated Transmit Buffers
	
	/* Configure Tx Event FIFO Configuration register */
	CAN0->TXEFC.reg =	(0 << CAN_TXEFC_EFWM_Pos) |									// Disable: Event Fifo Watermark interrupt
						CAN_TXEFC_EFSA((uint32_t)&can0_tx_event_fifo) |				// Event FIFO Start Address
						CAN_TXEFC_EFS(1);											// Event FIFO Size											/* Configure Tx Buffer Element Size Configuration register */
	CAN0->TXESC.reg =	CAN_TXESC_TBDS_DATA8;										// Tx Buffer Data Field Size
			/* Configure Transmitter Delay Compensation register */
	CAN0->TDCR.reg =	CAN_TDCR_TDCO(0) |											// Transmitter Delay Compensation Offset
						CAN_TDCR_TDCF(0);											// Transmitter Delay Compensation Filter Window Length					
			/* Configure Tx Buffer Transmission Interrupt Enable register */	CAN0->TXBTIE.reg =	CAN_TXBTIE_MASK;											// Transmission Interrupt Enable
		/* Configure Tx Buffer Cancellation Finished Interrupt Enable register */	CAN0->TXBCIE.reg =	CAN_TXBCIE_MASK;											// Cancellation Finished Interrupt Enable
	/* Configure Nominal Bit Timing and Prescaler register */
	CAN0->NBTP.reg =	CAN_NBTP_NBRP(NBTP_NBRP_VALUE) |							// Nominal Baud Rate Prescaler
						CAN_NBTP_NSJW(NBTP_NSJW_VALUE) |							// Nominal (Re)Syncronization Jump Width
						CAN_NBTP_NTSEG1(NBTP_NTSEG1_VALUE) |						// Nominal Time segment before sample point
						CAN_NBTP_NTSEG2(NBTP_NTSEG2_VALUE);							// Time segment after sample point
	can0_rx_standard_filter[0].SIDFE_0.reg =	CAN_SIDFE_0_SFT_CLASSIC |							// Classic filter: SFID1 = filter, SFID2 = mask
												CAN_SIDFE_0_SFEC_STF0M |							// Store into Rx FiFo 0
												CAN_SIDFE_0_SFID1(CAN_FILTER_ID_MOTORCONTROL) |		// First ID of standard ID filter element
												CAN_SIDFE_0_SFID2(0); 								// Standard Filter ID 2

	NVIC_SetPriority(CAN0_IRQn, 1);	// Set interrupt priority to highest urgency
	NVIC_EnableIRQ(CAN0_IRQn);		// Enable SysTick Interrupt
}


void CAN0_Start()
{
	CAN0->CCCR.reg &= ~CAN_CCCR_INIT;												// Configuration Change Enable
	while (CAN0->CCCR.reg & CAN_CCCR_INIT);
}


void can0_transmit(uint8_t size, uint8_t* data)
{
	for (uint8_t dIdx = 0; dIdx < size; dIdx++)
		((uint8_t *)can0_tx_buffer.TXBE_DATA)[dIdx] = data[dIdx];
	
	can0_tx_buffer.TXBE_0.reg =		CAN_TXBE_0_ID(CAN_FILTER_ID_MOTORCONTROL << 18);			// Message ID
	can0_tx_buffer.TXBE_1.reg =		CAN_TXBE_1_FDF |							// Frame transmitted in CAN FD format
									CAN_TXBE_1_BRS |							// CAN FD frames transmitted with bit rate switching.
									CAN_TXBE_1_DLC(8);							// Data Lenght Code (64 bytes)

	CAN0->TXBAR.reg = CAN_TXBAR_AR0;											// Add Transmit Request

	while ((CAN0->IR.reg & (CAN_IR_TC | CAN_IR_TOO | CAN_IR_PEA | CAN_IR_PED)) == 0x00);

	CAN0->IR.reg = (CAN_IR_TC | CAN_IR_TOO | CAN_IR_PEA | CAN_IR_PED);
}

uint8_t can0_fifo0_data_available()
{
	return ((CAN0->RXF0S.reg & CAN_RXF0S_F0FL_Msk));
}

uint16_t can0_fifo0_read_message(uint8_t* buffer)
{
	uint32_t fifo_slot = (CAN0->RXF0S.reg & CAN_RXF0S_F0GI_Msk) >> CAN_RXF0S_F0GI_Pos;
	
	uint32_t identifier = (can0_rx_fifo0[fifo_slot * 4] >> 18) & 0x7FF;
	
	*((uint32_t*)buffer) = can0_rx_fifo0[(fifo_slot * 4) + 2];
	*((uint32_t*)(buffer + 4)) = can0_rx_fifo0[(fifo_slot * 4) + 3];
	
	CAN0->RXF0A.reg = fifo_slot;
	
	return identifier;
}

void CAN0_Handler()
{
	
}