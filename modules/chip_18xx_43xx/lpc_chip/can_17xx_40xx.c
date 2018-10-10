/*
 * @brief LPC17xx/40xx CAN driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Determine clock for CAN Peripheral */
static SYSCON_CLK_T Chip_CAN_DetermineClk(LPC_CAN_Type *pCAN) {

	SYSCON_CLK_T CanClk;

	if (pCAN == LPC_CAN1) {
		CanClk = SYSCON_CLK_CAN1;
	}
	else {
		CanClk = SYSCON_CLK_CAN2;
	}

	return CanClk;
}

/* Determine CAN peripheral for reset*/
static SYSCON_RESET_T Chip_CAN_DetermineReset(LPC_CAN_Type *pCAN) {

	SYSCON_RESET_T CanRst;

	if (pCAN == LPC_CAN1) {
		CanRst = SYSCON_RESET_CAN1;
	}
	else {
		CanRst = SYSCON_RESET_CAN2;
	}

	return CanRst;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize CAN Interface */
void Chip_CAN_Init(LPC_CAN_Type *pCAN)
{
	/* Enable CAN Clock */
	Chip_SYSCON_PCLKEnable(Chip_CAN_DetermineClk(pCAN));
	Chip_SYSCON_PeriphReset(Chip_CAN_DetermineReset(pCAN));

	/* Initialize CAN Peripheral */
	IP_CAN_Init(pCAN);

	/* Initiialize Acceptance filter */
	IP_CAN_AF_Init(LPC_CANAF, LPC_CANAF_RAM);
}

/* De-Initialize CAN Interface */
void Chip_CAN_DeInit(LPC_CAN_Type *pCAN)
{
	IP_CAN_DeInit(pCAN);
	Chip_SYSCON_PCLKDisable(Chip_CAN_DetermineClk(pCAN));
}

/* Set CAN Bit Rate */
Status Chip_CAN_SetBitRate(LPC_CAN_Type *pCAN, uint32_t BitRate)
{
	IP_CAN_BUS_TIMING_Type BusTiming;
	uint32_t result = 0;
	uint8_t NT, TSEG1 = 0, TSEG2 = 0;
	uint32_t CANPclk = 0;
	uint32_t BRP = 0;

	CANPclk = Chip_SYSCON_GetPCLKRate();

	result = CANPclk / BitRate;

	/* Calculate suitable nominal time value
	 * NT (nominal time) = (TSEG1 + TSEG2 + 3)
	 * NT <= 24
	 * TSEG1 >= 2*TSEG2
	 */
	for (NT = 24; NT > 0; NT = NT - 2) {
		if ((result % NT) == 0) {
			BRP = result / NT - 1;

			NT--;

			TSEG2 = (NT / 3) - 1;

			TSEG1 = NT - (NT / 3) - 1;

			break;
		}
	}
	if (NT == 0) {
		return ERROR;
	}

	BusTiming.TESG1 = TSEG1;
	BusTiming.TESG2 = TSEG2;
	BusTiming.BRP = BRP;
	BusTiming.SJW = 3;
	BusTiming.SAM = 0;
	IP_CAN_SetBusTiming(pCAN, &BusTiming);
	return SUCCESS;
}

/* Get Free TxBuf */
CAN_BUFFER_ID Chip_CAN_GetFreeTxBuf(IP_CAN_001_Type *pCAN)
{
	IP_CAN_BUFFER_ID TxBufID = CAN_BUFFER_1;

	/* Select a free buffer */
	for (TxBufID = (IP_CAN_BUFFER_ID) 0; TxBufID < CAN_BUFFER_LAST; TxBufID++) {
		if (IP_CAN_GetStatus(pCAN) & CAN_SR_TBS(TxBufID)) {
			break;
		}
	}

	return TxBufID;
}
