/*
 * @brief LPC17xx/40xx SD card driver
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
#define CMD_TIMEOUT                  (0x10000)
#define DATA_TIMEOUT                 (0x1000000)
#define DATA_TIMER_VALUE_R           (SDC_TRAN_CLOCK_RATE / 4)		// 250ms
#define DATA_TIMER_VALUE_W           (SDC_TRAN_CLOCK_RATE)		// 1000ms
#define MS_ACQUIRE_DELAY             (100)			/*!< inter-command acquire oper condition delay in msec*/

/* SD commands supported */
STATIC const IP_SDC_001_CMD_Type SDMMC_Commands[] = {
	{CMD0_GO_IDLE_STATE, 0, SDC_NO_RESPONSE, SDC_COMMAND_INTERRUPT},/*!< GO_IDLE_STATE(MMC) or RESET(SD) */
	{CMD1_SEND_OP_COND, SDC_OCR_27_36, CMDRESP_R3_TYPE, 0},		/*!< SEND_OP_COND(MMC) or ACMD41(SD) */
	{CMD2_ALL_SEND_CID, 0, CMDRESP_R2_TYPE, 0},					/*!< ALL_SEND_CID */
	{CMD3_SET_RELATIVE_ADDR, 0 /*RCA*/, CMDRESP_R1_TYPE, 0},	/*!< SET_RELATE_ADDR */
	{CMD3_SEND_RELATIVE_ADDR, 0, CMDRESP_R6_TYPE, 0},			/*!< SEND_RELATE_ADDR */
	{CMD7_SELECT_CARD, 0 /*RCA*/, CMDRESP_R1b_TYPE, 0},			/*!< SELECT/DESELECT_CARD */
	{CMD8_SEND_IF_COND, (CMD8_VOLTAGESUPPLIED_27_36 | CMD8_CHECKPATTERN(CMD8_DEF_PATTERN)), CMDRESP_R7_TYPE, 0},	/*!<SEND_IF_COND */
	{CMD9_SEND_CSD, 0 /*RCA*/, CMDRESP_R2_TYPE, 0},						/*!< SEND_CSD */
	{CMD12_STOP_TRANSMISSION, 0, CMDRESP_R1_TYPE, 0},			/*!<STOP_TRANSMISSION */
	{CMD13_SEND_STATUS, 0 /*RCA*/, CMDRESP_R1_TYPE, 0},		/*!< SEND_STATUS */

	/* Block-Oriented Read Commands (class 2) */
	{CMD16_SET_BLOCKLEN, 0 /*Block Len*/, CMDRESP_R1_TYPE, 0},		/*!<SET_BLOCK_LEN */
	{CMD17_READ_SINGLE_BLOCK, 0	/*Data Addr*/, CMDRESP_R1_TYPE, 0},	/*!<READ_SINGLE_BLOCK */
	{CMD18_READ_MULTIPLE_BLOCK, 0 /*Data Addr*/, CMDRESP_R1_TYPE, 0},	/*!<READ_MULTIPLE_BLOCK */
	/* Class 3 */

	/* Block-Oriented Write Commands (class 4) */
	{CMD24_WRITE_BLOCK, 0 /*Data Addr*/, CMDRESP_R1_TYPE, 0},	/*!<WRITE_BLOCK */
	{CMD25_WRITE_MULTIPLE_BLOCK, 0 /*Data Addr*/, CMDRESP_R1_TYPE, 0},		/*!<WRITE_MULTIPLE_BLOCK */

	/* Erase Commands (class 5) */
	{CMD32_ERASE_WR_BLK_START, 0 /*Data Addr*/, CMDRESP_R1_TYPE, 0},		/*!<ERASE_WR_BLK_START */
	{CMD33_ERASE_WR_BLK_END, 0 /*Data Addr*/, CMDRESP_R1_TYPE, 0},			/*!<ERASE_WR_BLK_END */
	{CMD38_ERASE, 0, CMDRESP_R1b_TYPE, 0},						/*!<ERASE */

	/* Application-Specific Commands (class 8) */
	{CMD55_APP_CMD, 0 /*RCA*/, CMDRESP_R1_TYPE, 0},				/*!<APP_CMD*/
	{ACMD6_SET_BUS_WIDTH, 0	/*Bus width*/, CMDRESP_R1_TYPE, 0},		/*!<SET_BUS_WIDTH*/
	{ACMD13_SEND_SD_STATUS, 0, CMDRESP_R1_TYPE, 0},					/*!<SEND_SD_STATUS*/
	{ACMD41_SD_SEND_OP_COND, 0 /*HCS, S18R, OCR*/, CMDRESP_R3_TYPE, 0},				/*!<SD_SEND_OP_COND*/
};
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
/* Send a command to card */
STATIC int32_t sendCmd(IP_SDC_001_CMD_Type *pCommand, uint32_t timeout)
{
	int32_t ret = SDC_RET_TIMEOUT;
	uint32_t Status;

	/* Set Command Info */
	IP_SDC_SetCommand(LPC_SDC, pCommand);

	while (timeout) {

		Status = IP_SDC_GetStatus(LPC_SDC);

		/* check if command was sent */
		if ((pCommand->RespType == SDC_NO_RESPONSE) && (Status & SDC_STATUS_CMDSENT)) {
			ret =  SDC_RET_OK;
			break;
		}
		/* check if response was received */
		if (Status & SDC_STATUS_CMDRESPEND) {
			ret = SDC_RET_OK;
			break;
		}

		/* check command sending status */
		if (Status & SDC_STATUS_CMDERR) {
			if (Status & SDC_STATUS_CMDCRCFAIL) {
				if ((pCommand->CmdIndex == CMD1_SEND_OP_COND) ||
					(pCommand->CmdIndex == ACMD41_SD_SEND_OP_COND) ||
					(pCommand->CmdIndex == CMD12_STOP_TRANSMISSION)) {
					ret = SDC_RET_OK;	/* ignore CRC error if it's a resp for SEND_OP_COND  or STOP_TRANSMISSION. */
					break;
				}
			}
			ret = SDC_RET_CMD_FAILED;
			break;
		}

		timeout--;
	}

	IP_SDC_ResetCommand(LPC_SDC);

	return ret;
}

/* Function to send a command to card and get its response (if any)*/
STATIC int32_t executeCmd(IP_SDC_001_CMD_Type *pCommand, IP_SDC_001_RESP_Type *pResp)
{
	int32_t Ret = SDC_RET_FAILED;

	/* Send Command to card */
	Ret = sendCmd(pCommand, CMD_TIMEOUT);
	if (Ret != SDC_RET_OK) {
		return Ret;
	}

	/* Get response (if any) */
	if (pCommand->RespType != SDC_NO_RESPONSE) {
		IP_SDC_GetResp(LPC_SDC, pResp);
		/* If the response is not R1, in the response field, the Expected Cmd data
		        won't be the same as the CMD data in SendCmd(). Below four cmds have
		        R2 or R3 response. We don't need to check if MCI_RESP_CMD is the same
		        as the Expected or not. */
		if ((pCommand->CmdIndex != CMD1_SEND_OP_COND) &&
			(pCommand->CmdIndex != ACMD41_SD_SEND_OP_COND) &&
			(pCommand->CmdIndex != CMD2_ALL_SEND_CID) &&
			(pCommand->CmdIndex != CMD9_SEND_CSD) &&
			(pResp->CmdIndex != pCommand->CmdIndex)) {
			return SDC_RET_CMD_FAILED;
		}
	}

	return SDC_RET_OK;
}

/* Check R1 response, the result is stored in pCheckResult parameter. */
/* This function return 1 to exit the command execution, 0 to retry sending command */
int32_t checkR1Response(uint32_t resp, int32_t *pCheckResult)
{
	int32_t Ret = 1;

	if (!(resp & SDC_CARD_STATUS_READY_FOR_DATA)) {
		*pCheckResult = SDC_RET_NOT_READY;
		Ret = 0;
	}
	else if (resp & SDC_CARD_STATUS_ERR_MASK) {
		*pCheckResult =  SDC_RET_FAILED;
	}
	else {
		*pCheckResult =  SDC_RET_OK;
	}
	return Ret;
}

/* Send APP_CMD to card*/
STATIC int32_t sendAppCmd(uint16_t rca)

{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_CMD_Type Command;
	IP_SDC_001_RESP_Type Response;
	uint32_t RetryCnt =  20;

	Command = SDMMC_Commands[SDC_CMD55_APP_CMD];
	Command.Argument = CMD55_RCA(rca);
	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				if (Ret != SDC_RET_OK) {
					return Ret;
				}
				if (Response.Data[0] & SDC_CARD_STATUS_ACMD_ENABLE) {
					return SDC_RET_OK;
				}
				else {
					Ret = SDC_RET_FAILED;
				}
			}
		}
		RetryCnt--;
	}
	return SDC_RET_FAILED;
}

/* Send Reset command to card*/
STATIC INLINE int32_t cardReset(void)
{
	return executeCmd((IP_SDC_001_CMD_Type *) &SDMMC_Commands[SDC_CMD0_GO_IDLE_STATE], NULL);
}

/* Send Interface condition to card*/
STATIC int32_t sendIfCond(void)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	uint32_t RetryCnt =  20;

	while (RetryCnt > 0) {
		Ret = executeCmd((IP_SDC_001_CMD_Type *) &SDMMC_Commands[SDC_CMD8_SEND_IF_COND], &Response);
		if (Ret == SDC_RET_OK) {
			if ((Response.Data[0] & CMDRESP_R7_VOLTAGE_ACCEPTED) &&
				(CMDRESP_R7_CHECK_PATTERN(Response.Data[0]) == CMD8_DEF_PATTERN)) {
				return SDC_RET_OK;
			}
			return SDC_RET_BAD_PARAMETERS;
		}
		RetryCnt--;
	}
	return Ret;
}

/* Send Operation condition to card */
STATIC int32_t sendOpCond(uint32_t *pOCR)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	uint32_t RetryCnt =  0x200;

	while (RetryCnt > 0) {
		Ret = executeCmd((IP_SDC_001_CMD_Type *) &SDMMC_Commands[SDC_CMD1_SEND_OP_COND], &Response);
		if (Ret == SDC_RET_OK) {
			*pOCR = Response.Data[0];
			if (*pOCR & SDC_OCR_IDLE) {
				if ((Response.Data[0] & SDC_OCR_27_36) != SDC_OCR_27_36) {
					return SDC_RET_BAD_PARAMETERS;
				}
				return SDC_RET_OK;
			}
		}
		RetryCnt--;
	}
	return SDC_RET_FAILED;
}

/* Send ACMD41 command to card.If *Ocr = 0, it gets OCR. Otherwise, it starts initialization card.  */
/* Open Drain bit must be cleared before calling this function */
STATIC int32_t sendAppOpCond(uint16_t rca, bool hcs, uint32_t *pOcr, bool *pCCS)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint32_t RetryCnt =  0x2000;	/* The host repeatedly issues ACMD41 for at least 1 second or
									       until the busy bit are set to 1 */

	Command = SDMMC_Commands[SDC_ACMD41_SEND_APP_OP_COND];
	Command.Argument = ACMD41_OCR(*pOcr);
	if (hcs) {
		Command.Argument |= ACMD41_HCS;
	}

	while (RetryCnt > 0) {
		Ret = sendAppCmd(rca);
		if (Ret == SDC_RET_OK) {
			Ret = executeCmd(&Command, &Response);
			if (Ret == SDC_RET_OK) {
				if (Response.Data[0] & CMDRESP_R3_INIT_COMPLETE) {
					if (*pOcr == 0) {
						*pOcr = CMDRESP_R3_OCR_VAL(Response.Data[0]);
						return SDC_RET_OK;
					}
					if ((CMDRESP_R3_OCR_VAL(Response.Data[0]) & *pOcr) != *pOcr) {
						return SDC_RET_BAD_PARAMETERS;
					}
					*pCCS = (Response.Data[0] & CMDRESP_R3_HC_CCS) ? true : false;
					return SDC_RET_OK;
				}
			}
		}
		else {
			return Ret;
		}
		RetryCnt--;
	}
	return SDC_RET_FAILED;
}

/* Get CID */
STATIC int32_t getCID(uint32_t *pCID)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	uint32_t RetryCnt =  20;

	while (RetryCnt > 0) {
		Ret = executeCmd((IP_SDC_001_CMD_Type *) &SDMMC_Commands[SDC_CMD2_ALL_SEND_CID], &Response);
		if (Ret == SDC_RET_OK) {
			pCID[3] = Response.Data[0];
			pCID[2] = Response.Data[1];
			pCID[1] = Response.Data[2];
			pCID[0] = Response.Data[3];
			return SDC_RET_OK;
		}
		RetryCnt--;
	}
	return Ret;
}

/* Set Addr */
STATIC int32_t setAddr(uint16_t addr)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint32_t RetryCnt =  20;

	Command = SDMMC_Commands[SDC_CMD3_SET_RELATIVE_ADDR];
	Command.Argument = CMD3_RCA(addr);
	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				return Ret;
			}
		}
		RetryCnt--;
	}
	return Ret;
}

STATIC int32_t getAddr(uint16_t *pRCA)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint32_t RetryCnt =  20;

	*pRCA = 0;
	Command = SDMMC_Commands[SDC_CMD3_SEND_RELATIVE_ADDR];
	Command.Argument = 0;
	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (!(CMDRESP_R6_CARD_STATUS(Response.Data[0]) & SDC_CARD_STATUS_READY_FOR_DATA)) {
				Ret = SDC_RET_NOT_READY;
			}
			else if (SDC_CARD_STATUS_CARDSTATE(CMDRESP_R6_CARD_STATUS(Response.Data[0])) != SDC_CARD_STATE_STBY) {
				Ret = SDC_RET_ERR_STATE;
			}
			else {
				*pRCA = CMDRESP_R6_RCA_VAL(Response.Data[0]);
				return SDC_RET_OK;
			}
		}
		RetryCnt--;
	}
	return Ret;
}

STATIC int32_t getCSD(uint16_t rca, uint32_t *pCSD)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint32_t RetryCnt =  20;

	Command = SDMMC_Commands[SDC_CMD9_SEND_CSD];
	Command.Argument = CMD9_RCA(rca);
	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			pCSD[3] = Response.Data[0];
			pCSD[2] = Response.Data[1];
			pCSD[1] = Response.Data[2];
			pCSD[0] = Response.Data[3];
			return Ret;
		}
		RetryCnt--;
	}
	return Ret;
}

/* Select card*/
STATIC int32_t selectCard(uint16_t addr)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint32_t RetryCnt =  20;

	Command = SDMMC_Commands[SDC_CMD7_SELECT_CARD];
	Command.Argument = CMD7_RCA(addr);
	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				return Ret;
			}
		}
		RetryCnt--;
	}
	return Ret;
}

/* Get card status */
STATIC int32_t getStatus(uint16_t rca, uint32_t *pStatus)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint32_t RetryCnt =  20;

	*pStatus = (uint32_t) -1;
	Command = SDMMC_Commands[SDC_CMD13_SEND_STATUS];
	Command.Argument = CMD13_RCA(rca);
	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			checkR1Response(Response.Data[0], &Ret);
			*pStatus = Response.Data[0];
			return Ret;
		}
		RetryCnt--;
	}
	return Ret;
}

/* Set bus width.  */
STATIC int32_t getAppStatus(uint16_t rca, uint32_t *pStatus)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	uint8_t RetryCnt =  0x20;

	while (RetryCnt > 0) {
		Ret = sendAppCmd(rca);
		if (Ret == SDC_RET_OK) {
			Ret = executeCmd((IP_SDC_001_CMD_Type *) &SDMMC_Commands[SDC_ACMD13_SEND_SD_STATUS], &Response);
			if (Ret == SDC_RET_OK) {
				if (checkR1Response(Response.Data[0], &Ret)) {
					return Ret;
				}
			}
		}
		RetryCnt--;
	}
	return SDC_RET_FAILED;
}

/* Helper function to get a bit field withing multi-word  buffer. Used to get
   fields with-in CSD & EXT-CSD */
static uint32_t getBits(int32_t start, int32_t end, uint32_t *data)
{
	uint32_t v;
	uint32_t i = end >> 5;
	uint32_t j = start & 0x1f;

	if (i == (start >> 5)) {
		v = (data[i] >> j);
	}
	else {
		v = ((data[i] << (32 - j)) | (data[start >> 5] >> j));
	}

	return v & ((1 << (end - start + 1)) - 1);
}

/* Function to process the CSD & EXT-CSD of the card */
static void processCSD(SDC_CARD_Type *pCardInfo)
{
	int32_t CSize = 0;
	int32_t CSizeMult = 0;
	int32_t Mult = 0;

	/* compute block length based on CSD response */
	pCardInfo->BlockLen = 1 << getBits(80, 83, pCardInfo->CSD);

	if ((pCardInfo->CardType & CARD_TYPE_HC) && (pCardInfo->CardType & CARD_TYPE_SD)) {
		/* See section 5.3.3 CSD Register (CSD Version 2.0) of SD2.0 spec  an explanation for the calculation of these values */
		CSize = getBits(48, 63, (uint32_t *) pCardInfo->CSD) + 1;
		pCardInfo->BlockNR = CSize << 10;	/* 512 byte blocks */
	}
	else {
		/* See section 5.3 of the 4.1 revision of the MMC specs for  an explanation for the calculation of these values */
		CSize = getBits(62, 73, (uint32_t *) pCardInfo->CSD);
		CSizeMult = getBits(47, 49, (uint32_t *) pCardInfo->CSD);
		Mult = 1 << (CSizeMult + 2);
		pCardInfo->BlockNR = (CSize + 1) * Mult;

		/* adjust blocknr to 512/block */
		if (pCardInfo->BlockLen > SDC_SECTOR_SIZE) {
			pCardInfo->BlockNR = pCardInfo->BlockNR * (pCardInfo->BlockLen >> 9);
		}
	}

	pCardInfo->DeviceSize = pCardInfo->BlockNR << 9;	/* blocknr * 512 */
}

/* Set bus width.  */
STATIC int32_t setBusWidth(uint16_t rca, uint8_t width)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint8_t RetryCnt =  0x20;

	Command = SDMMC_Commands[SDC_ACMD6_SET_BUS_WIDTH];
	Command.Argument = ACMD6_BUS_WIDTH(width);

	while (RetryCnt > 0) {
		Ret = sendAppCmd(rca);
		if (Ret == SDC_RET_OK) {
			Ret = executeCmd(&Command, &Response);
			if (Ret == SDC_RET_OK) {
				if (checkR1Response(Response.Data[0], &Ret)) {
					return Ret;
				}
			}
		}
		RetryCnt--;
	}
	return SDC_RET_FAILED;
}

/* Puts current selected card in trans state */
static int32_t setTranState(uint16_t rca)
{
	int32_t Ret = 0;
	uint32_t status = 0;
	SDC_CARD_STATE_T state;

	/* get current state of the card */
	Ret = getStatus(rca, &status);
	if (Ret != SDC_RET_OK) {
		/* unable to get the card state. So return immediatly. */
		return Ret;
	}

	/* check card state in response */
	state = (SDC_CARD_STATE_T) SDC_CARD_STATUS_CARDSTATE(status);
	switch (state) {
	case SDC_CARD_STATE_STBY:
		/* put card in 'Trans' state */
		Ret = selectCard(rca);
		if (Ret != SDC_RET_OK) {
			/* unable to put the card in Trans state. So return immediatly. */
			return Ret;
		}
		getStatus(rca, &status);
		if ((SDC_CARD_STATE_T) SDC_CARD_STATUS_CARDSTATE(status) != SDC_CARD_STATE_TRAN) {
			return SDC_RET_ERR_STATE;
		}
		break;

	case SDC_CARD_STATE_TRAN:
		/*do nothing */
		break;

	default:
		/* card shouldn't be in other states so return */
		return SDC_RET_ERR_STATE;
	}

	return SDC_RET_OK;
}

/* Set bus width.  */
STATIC int32_t setBlockLength(uint32_t rca, uint32_t blockLen)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint8_t RetryCnt =  0x20;

	Command = SDMMC_Commands[SDC_CMD16_SET_BLOCK_LEN];
	Command.Argument = blockLen;

	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				return Ret;
			}
		}
		RetryCnt--;
	}
	return SDC_RET_FAILED;
}

/* Sets card data width and block size */
static int32_t setCardParams(SDC_CARD_Type *pCardInfo)
{
	int32_t Ret;

	Chip_SDC_SetClock(SDC_TRAN_CLOCK_RATE);
	if (pCardInfo->CardType & CARD_TYPE_SD) {
		IP_SDC_ClockControl(LPC_SDC, SDC_CLOCK_WIDEBUS_MODE, ENABLE);
		Ret = setBusWidth(pCardInfo->RCA, ACMD6_BUS_WIDTH_4);
		if (Ret != SDC_RET_OK) {
			return Ret;
		}
	}
	else {
		IP_SDC_ClockControl(LPC_SDC, SDC_CLOCK_WIDEBUS_MODE, DISABLE);
	}

	/* set block length */
	Ret = setBlockLength(pCardInfo->RCA, SDC_SECTOR_SIZE);
	return Ret;
}

STATIC int32_t readBlocks(uint32_t cardType, uint32_t startBlock, uint32_t blockNum)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint8_t RetryCnt =  0x20;

	if (blockNum == 1) {
		Command = SDMMC_Commands[SDC_CMD17_READ_SINGLE_BLOCK];
	}
	else {
		Command = SDMMC_Commands[SDC_CMD18_READ_MULTIPLE_BLOCK];
	}

	/* Select single or multiple read based on number of blocks */
	/* if high capacity card use block indexing */
	if (cardType & CARD_TYPE_HC) {
		Command.Argument = startBlock;
	}
	else {	/*fix at 512 bytes*/
		Command.Argument = startBlock << 9;
	}

	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				return Ret;
			}
		}
		RetryCnt--;
	}
	return Ret;
}

STATIC int32_t writeBlocks(uint32_t cardType, uint32_t startBlock, uint32_t blockNum)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	IP_SDC_001_CMD_Type Command;
	uint8_t RetryCnt =  0x20;

	if (blockNum == 1) {
		Command = SDMMC_Commands[SDC_CMD24_WRITE_BLOCK];
	}
	else {
		Command = SDMMC_Commands[SDC_CMD25_WRITE_MULTIPLE_BLOCK];
	}

	/* if high capacity card use block indexing */
	if (cardType & CARD_TYPE_HC) {
		Command.Argument = startBlock;
	}
	else {	/*fix at 512 bytes*/
		Command.Argument = startBlock << 9;

	}

	while (RetryCnt > 0) {
		Ret = executeCmd(&Command, &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				return Ret;
			}
		}
		RetryCnt--;
	}
	return Ret;
}

STATIC int32_t stopTranmission(uint32_t rca)
{
	uint32_t Status;
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_RESP_Type Response;
	uint32_t RetryCnt =  20;

	Ret = getStatus(rca, &Status);
	if (Ret != SDC_RET_OK) {
		return SDC_RET_ERR_STATE;
	}

	if (SDC_CARD_STATUS_CARDSTATE(Status) == SDC_CARD_STATE_TRAN) {
		return SDC_RET_OK;
	}

	if ((SDC_CARD_STATUS_CARDSTATE(Status) != SDC_CARD_STATE_DATA) &&
		(SDC_CARD_STATUS_CARDSTATE(Status) != SDC_CARD_STATE_RCV)) {
		return SDC_RET_ERR_STATE;
	}

	while (RetryCnt > 0) {
		Ret = executeCmd((IP_SDC_001_CMD_Type *) &SDMMC_Commands[SDC_CMD12_STOP_TRANSMISSION], &Response);
		if (Ret == SDC_RET_OK) {
			if (checkR1Response(Response.Data[0], &Ret)) {
				if (Ret != SDC_RET_OK) {
					return Ret;
				}
				Ret = getStatus(rca, &Status);
				if ((SDC_CARD_STATUS_CARDSTATE(Status) == SDC_CARD_STATE_TRAN) ||
					(SDC_CARD_STATUS_CARDSTATE(Status) == SDC_CARD_STATE_PRG)) {
					return SDC_RET_OK;
				}
				return SDC_RET_ERR_STATE;
			}
		}
		RetryCnt--;
	}
	return Ret;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initializes the SDC card controller */
void Chip_SDC_Init(void)
{
	uint32_t i = 0;
	Chip_SYSCON_PCLKEnable(SYSCON_CLK_PCSDC);
	Chip_SYSCON_PeriphReset(SYSCON_RESET_PCSDC);

	/* Initialize GPDMA controller */
	Chip_GPDMA_Init();

	/* Initialize SDC peripheral */
	IP_SDC_Init(LPC_SDC);

	/* Power-up SDC Peripheral */
	IP_SDC_PowerControl(LPC_SDC, SDC_POWER_UP, 0);

	/* delays for the supply output is stable*/
	for ( i = 0; i < 0x80000; i++ ) {}

	Chip_SDC_SetClock(SDC_IDENT_CLOCK_RATE);
	IP_SDC_ClockControl(LPC_SDC, SDC_CLOCK_ENABLE, ENABLE);

	/* Power-on SDC Interface */
	IP_SDC_PowerControl(LPC_SDC, SDC_POWER_ON, 0);

}

/* Set SD_CLK Clock */
void Chip_SDC_SetClock(uint32_t freq)
{
	uint32_t PClk;
	uint32_t ClkValue = 0;

	PClk = Chip_SYSCON_GetPCLKRate();
	ClkValue = (PClk + 2 * freq - 1) / (2 * freq);
	if (ClkValue) {
		ClkValue -= 1;
	}
	IP_SDC_SetClockDiv(LPC_SDC, ClkValue);
}

/* Shutdown the SDC card controller */
void Chip_SDC_DeInit(void)
{
	IP_SDC_DeInit(LPC_SDC);
	Chip_SYSCON_PCLKDisable(SYSCON_CLK_PCSDC);
}

int32_t Chip_SDC_FIFOIRQHandler(uint8_t *txBuf, uint32_t *txCnt,
								uint8_t *rxBuf, uint32_t *rxCnt)
{
	uint32_t Status;
	Status = IP_SDC_GetStatus(LPC_SDC);

	if (txBuf ) {
		if (Status & SDC_STATUS_TXFIFOHALFEMPTY) {
			if (*txCnt % 64) {
				IP_SDC_WriteFIFO(LPC_SDC, (uint32_t *) &txBuf[*txCnt], false);
			}
			else {
				IP_SDC_WriteFIFO(LPC_SDC, (uint32_t *) &txBuf[*txCnt], true);
			}
			*txCnt += 32;
		}
	}

	if (rxBuf ) {
		if (Status & SDC_STATUS_RXFIFOHALFFULL) {
			if (*rxCnt % 64) {
				IP_SDC_ReadFIFO(LPC_SDC, (uint32_t *) &rxBuf[*rxCnt], false);
			}
			else {
				IP_SDC_ReadFIFO(LPC_SDC, (uint32_t *) &rxBuf[*rxCnt], true);
			}
			*rxCnt += 32;
		}
	}

	IP_SDC_ClearStatus(LPC_SDC, SDC_STATUS_FIFO);

	return 1;
}

int32_t Chip_SDC_IRQHandler(uint8_t *txBuf, uint32_t *txCnt,
							uint8_t *rxBuf, uint32_t *rxCnt)
{
	uint32_t Status;

	Status = IP_SDC_GetStatus(LPC_SDC);

	if ( Status & SDC_STATUS_DATAERR) {
		IP_SDC_ClearStatus(LPC_SDC, SDC_STATUS_DATAERR);
		return -1;	/* Data transfer error */
	}

	if ( Status & SDC_STATUS_DATAEND) {
		IP_SDC_ClearStatus(LPC_SDC, SDC_STATUS_DATAEND);
		IP_SDC_SetIntMask(LPC_SDC, 0);
		return 0;
	}

	if ( Status & SDC_STATUS_DATABLOCKEND) {
		IP_SDC_ClearStatus(LPC_SDC, SDC_STATUS_DATABLOCKEND);
		return 1;
	}

	if (Status & SDC_STATUS_FIFO) {
		return Chip_SDC_FIFOIRQHandler(txBuf, txCnt, rxBuf, rxCnt);
	}

	return 1;
}

/* Get card's current state (idle, transfer, program, etc.) */
SDC_CARD_STATE_T Chip_SDC_GetCardState(SDC_CARD_Type *pCardInfo)
{
	uint32_t Status;
	int32_t Ret;

	/* get current state of the card */
	Ret = getStatus(pCardInfo->RCA, &Status);
	if (Ret != SDC_RET_OK) {
		return SDC_CARD_STATE_UNKNOWN;
	}

	/* check card state in response */
	return (SDC_CARD_STATE_T) SDC_CARD_STATUS_CARDSTATE(Status);
}

/* Get current card status */
uint32_t Chip_SDC_GetCardStatus(SDC_CARD_Type *pCardInfo)
{
	uint32_t Status;
	getStatus(pCardInfo->RCA, &Status);
	return Status;
}

/* Get current sd status */
int32_t Chip_SDC_GetSDStatus(SDC_CARD_Type *pCardInfo, uint32_t *pStatus)
{
	int32_t Ret;
	uint16_t ByteNum = 64;
	SDC_EVENT_Type  Event;
	IP_SDC_001_DATA_TRANSFER_Type Transfer;

	/* Put to tran state */
	if (setTranState(pCardInfo->RCA) != SDC_RET_OK) {
		return 0;
	}

#ifdef SDC_DMA_ENABLE
	Chip_SDC_SetIntMask(SDC_MASK0_DATA | SDC_MASK0_RXDATAERR);
	Event.DmaChannel = Chip_DMA_GetFreeChannel(GPDMA_CONN_SDC);
	/* DMA Setup */
	Chip_DMA_Transfer(Event.DmaChannel,
					  GPDMA_CONN_SDC,
					  (uint32_t) pStatus,
					  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_PERIPHERAL,
					  ByteNum);
#else
	Chip_SDC_SetIntMask(SDC_MASK0_DATA | SDC_MASK0_RXDATAERR | SDC_MASK0_RXFIFO);

	Event.Buffer = pStatus;
	Event.Size = ByteNum;
	Event.Index = 0;
	Event.Dir = 1;
#endif
	pCardInfo->fnEventSetup(&Event);

	/* set transfer information */
	Transfer.BlockNum = 1;
	Transfer.BlockSize = SDC_BLOCK_SIZE_64;	/* 512 bit */
	Transfer.Dir = SDC_TRANSFER_DIR_FROMCARD;
#ifdef SDC_DMA_ENABLE
	Transfer.DMAUsed = true;
#else
	Transfer.DMAUsed = false;
#endif
	Transfer.Mode = SDC_TRANSFER_MODE_BLOCK;
	Transfer.Timeout = DATA_TIMER_VALUE_R;
	IP_SDC_SetDataTransfer(LPC_SDC, &Transfer);

	/* Send ACMD13 command */
	Ret = getAppStatus(pCardInfo->RCA, pStatus);
	if (Ret != SDC_RET_OK) {
		ByteNum = 0;
		goto send_end;
	}

	/* Wait for transfer Finish */
	if (pCardInfo->fnWaitEvent() < 0) {
		ByteNum = 0;
	}

send_end:
#ifdef SDC_DMA_ENABLE
	Chip_DMA_Stop(Event.DmaChannel);
#endif
	if (Chip_SDC_GetCardState(pCardInfo) == SDC_CARD_STATE_DATA) {
		/* Send Stop transmission command */
		stopTranmission(pCardInfo->RCA);
	}
	
	/*Wait for card enters to tran state*/
	while ((Chip_SDC_GetCardState(pCardInfo) != SDC_CARD_STATE_TRAN)) {}

	return ByteNum;
}

/* Function to enumerate the SD/MMC/SDHC/MMC+ cards */
int32_t Chip_SDC_Acquire(SDC_CARD_Type *pCardInfo)
{
	int32_t Ret;

	/* Initialize card info */
	pCardInfo->Speed = SDC_TRAN_CLOCK_RATE;
	pCardInfo->CardType = 0;

	/* During identification phase, the clock should be less than
	   400Khz. Once we pass this phase, the normal clock can be set up
	   to 25Mhz on SD card and 20Mhz on MMC card. */
	Chip_SDC_SetClock(SDC_IDENT_CLOCK_RATE);

	/* Clear Open Drain output control for SD */
	IP_SDC_PowerControl(LPC_SDC, SDC_POWER_ON, 0);

	/* Card Reset */
	Ret = cardReset();
	if (Ret != 0) {
		return Ret;
	}

	pCardInfo->fnMsDelay(MS_ACQUIRE_DELAY);

	/* Send interface operation condiftion */
	Ret = sendIfCond();
	if (Ret == SDC_RET_BAD_PARAMETERS) {
		return Ret;		/* Non-compatible voltage range or check pattern is not correct */

	}
	/* Get Card Type */
	if (Ret == SDC_RET_OK) {/* Ver2.00 or later SD Memory Card*/
		bool CCS;
		uint32_t OCR = SDC_OCR_27_36;
		pCardInfo->CardType |= CARD_TYPE_SD;
		Ret = sendAppOpCond(0, true, &OCR, &CCS);
		if (CCS) {	/* High Capacity or Extended Capacity SD Memory Card */
			pCardInfo->CardType |= CARD_TYPE_HC;
		}
	}
	else {	/*Ver2.00 or later SD Memory Card(voltage mismatch) or Ver1.X SD Memory Card
			   or not SD Memory Card*/
		bool CCS;
		uint32_t OCR = SDC_OCR_27_36;
		Ret = sendAppOpCond(0, false, &OCR, &CCS);
		if (Ret == SDC_RET_OK) {
			pCardInfo->CardType |= CARD_TYPE_SD;
		}
		else if (Ret == SDC_RET_BAD_PARAMETERS) {
			return Ret;
		}
		else {	/* MMC Card setup */
			uint32_t OCR;
			/* Enter to Open Drain mode */
			IP_SDC_PowerControl(LPC_SDC, SDC_POWER_ON, SDC_PWR_OPENDRAIN);
			pCardInfo->fnMsDelay(MS_ACQUIRE_DELAY);
			Ret = sendOpCond(&OCR);
			if (Ret != SDC_RET_OK) {
				return Ret;
			}

		}
	}

	/* Read CID */
	getCID(pCardInfo->CID);

	/* RCA send, for SD get RCA */
	if (pCardInfo->CardType & CARD_TYPE_SD) {
		getAddr(&pCardInfo->RCA);
	}
	else {
		pCardInfo->RCA = 1;
		setAddr(pCardInfo->RCA);
		IP_SDC_PowerControl(LPC_SDC, SDC_POWER_ON, 0);	/* enter to push-pull mode */
	}

	/* Get CSD */
	getCSD(pCardInfo->RCA, pCardInfo->CSD);

	/* Compute card size, block size and no. of blocks  based on CSD response recived. */
	if (pCardInfo->CID[0]) {
		processCSD(pCardInfo);

		if (setTranState(pCardInfo->RCA) != SDC_RET_OK) {
			return 0;
		}

		if (Chip_SDC_GetCardState(pCardInfo) != SDC_CARD_STATE_TRAN) {
			return 0;
		}

		if (setCardParams(pCardInfo) != 0) {
			return 0;
		}
	}

	return (pCardInfo->CID[0]) ? 1 : 0;
}

/* Performs the read of data from the SD/MMC card */
int32_t Chip_SDC_ReadBlocks(SDC_CARD_Type *pCardInfo, void *buffer, int32_t startBlock, int32_t blockNum)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_DATA_TRANSFER_Type Transfer;
	SDC_EVENT_Type  Event;
	int32_t ByteNum = blockNum *  SDC_SECTOR_SIZE;

	/* if card is not acquired return immediately */
	if (( startBlock < 0) || ( (startBlock + blockNum) > pCardInfo->BlockNR) ) {
		return 0;
	}

	/* Put to tran state */
	if (setTranState(pCardInfo->RCA) != SDC_RET_OK) {
		return 0;
	}

#ifdef SDC_DMA_ENABLE
	Chip_SDC_SetIntMask(SDC_MASK0_DATA | SDC_MASK0_RXDATAERR);
	Event.DmaChannel = Chip_DMA_GetFreeChannel(GPDMA_CONN_SDC);
	/* DMA Setup */
	Chip_DMA_Transfer(Event.DmaChannel,
					  GPDMA_CONN_SDC,
					  (uint32_t) buffer,
					  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_PERIPHERAL,
					  ByteNum);
#else
	Chip_SDC_SetIntMask(SDC_MASK0_DATA | SDC_MASK0_RXDATAERR | SDC_MASK0_RXFIFO);

	Event.Buffer = buffer;
	Event.Size = ByteNum;
	Event.Index = 0;
	Event.Dir = 1;
#endif
	pCardInfo->fnEventSetup(&Event);

	/* set transfer information */
	Transfer.BlockNum = blockNum;
	Transfer.BlockSize = SDC_BLOCK_SIZE_512;
	Transfer.Dir = SDC_TRANSFER_DIR_FROMCARD;
#ifdef SDC_DMA_ENABLE
	Transfer.DMAUsed = true;
#else
	Transfer.DMAUsed = false;
#endif
	Transfer.Mode = SDC_TRANSFER_MODE_BLOCK;
	Transfer.Timeout = DATA_TIMER_VALUE_R;
	IP_SDC_SetDataTransfer(LPC_SDC, &Transfer);

	Ret = readBlocks(pCardInfo->CardType, startBlock, blockNum);
	if (Ret != SDC_RET_OK) {
		ByteNum = 0;
		goto send_end;
	}

	/* Wait for transfer Finish */
	if (pCardInfo->fnWaitEvent() < 0) {
		ByteNum = 0;
	}

send_end:
#ifdef SDC_DMA_ENABLE
	Chip_DMA_Stop(Event.DmaChannel);
#endif

	if ((blockNum > 1) || (Chip_SDC_GetCardState(pCardInfo) == SDC_CARD_STATE_DATA)) {
		/* Send Stop transmission command */
		stopTranmission(pCardInfo->RCA);
	}

	/*Wait for card enter to tran state*/
	while (Chip_SDC_GetCardState(pCardInfo) != SDC_CARD_STATE_TRAN) {}

	return ByteNum;
}

/* Performs write of data to the SD/MMC card */
int32_t Chip_SDC_WriteBlocks(SDC_CARD_Type *pCardInfo, void *buffer, int32_t startBlock, int32_t blockNum)
{
	int32_t Ret = SDC_RET_FAILED;
	IP_SDC_001_DATA_TRANSFER_Type Transfer;
	SDC_EVENT_Type  Event;
	int32_t ByteNum = blockNum *  SDC_SECTOR_SIZE;

	/* if card is not acquired return immediately */
	if (( startBlock < 0) || ( (startBlock + blockNum) > pCardInfo->BlockNR) ) {
		return 0;
	}

	/* Put to tran state */
	if (setTranState(pCardInfo->RCA) != SDC_RET_OK) {
		return 0;
	}

	Ret = writeBlocks(pCardInfo->CardType, startBlock, blockNum);
	if (Ret != SDC_RET_OK) {
		return 0;
	}

	/*Wait for card enter to rcv state*/
	while (Chip_SDC_GetCardState(pCardInfo) != SDC_CARD_STATE_RCV) {}

#ifdef SDC_DMA_ENABLE
	Chip_SDC_SetIntMask(SDC_MASK0_DATA | SDC_MASK0_TXDATAERR);

	Event.DmaChannel = Chip_DMA_GetFreeChannel(GPDMA_CONN_SDC);

	/* DMA Setup */
	Chip_DMA_Transfer(Event.DmaChannel,
					  (uint32_t) buffer,
					  GPDMA_CONN_SDC,
					  GPDMA_TRANSFERTYPE_M2P_CONTROLLER_PERIPHERAL,
					  ByteNum);
#else
	Chip_SDC_SetIntMask(SDC_MASK0_DATA | SDC_MASK0_TXDATAERR | SDC_MASK0_TXFIFO);
	Event.Buffer = buffer;
	Event.Size = ByteNum;
	Event.Index = 0;
	Event.Dir = 0;
#endif
	pCardInfo->fnEventSetup(&Event);

	/* set transfer information */
	Transfer.BlockNum = blockNum;
	Transfer.BlockSize = SDC_BLOCK_SIZE_512;
	Transfer.Dir = SDC_TRANSFER_DIR_TOCARD;
#ifdef SDC_DMA_ENABLE
	Transfer.DMAUsed = true;
#else
	Transfer.DMAUsed = false;
#endif
	Transfer.Mode = SDC_TRANSFER_MODE_BLOCK;
	Transfer.Timeout = DATA_TIMER_VALUE_W;
	IP_SDC_SetDataTransfer(LPC_SDC, &Transfer);

	/* Wait for transfer done */
	if (pCardInfo->fnWaitEvent() < 0) {
		ByteNum = 0;
	}
#ifdef SDC_DMA_ENABLE
	Chip_DMA_Stop(Event.DmaChannel);
#endif
	if ((blockNum > 1) || (Chip_SDC_GetCardState(pCardInfo) == SDC_CARD_STATE_RCV)) {
		/* Send Stop transmission command */
		stopTranmission(pCardInfo->RCA);
	}

	/*Wait for card enter to tran state*/
	while (Chip_SDC_GetCardState(pCardInfo) != SDC_CARD_STATE_TRAN) {}

	return ByteNum;
}
