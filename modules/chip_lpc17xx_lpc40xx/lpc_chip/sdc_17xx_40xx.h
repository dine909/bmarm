/*
 * @brief LPC17xx/40xx SD Card Interface driver
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

#ifndef __SDC_17XX_40XX_H_
#define __SDC_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup SDC_17XX_40XX CHIP: LPC17xx/40xx SDC driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/**
 * @brief Card bus clock rate definitions
 */
/* Card bus clock in Card Identification Mode */
#define SDC_IDENT_CLOCK_RATE         (400000)	/* 400KHz */
/* Card bus clock in Data Transfer Mode */
#define SDC_TRAN_CLOCK_RATE        (20000000)	/* 20MHz */

/**
 * @brief OCR Register definitions
 */
/** Support voltage range 2.0-2.1 (this bit is reserved in SDC)*/
#define SDC_OCR_20_21               (((uint32_t) 1) << 8)
/** Support voltage range 2.1-2.2 (this bit is reserved in SDC)*/
#define SDC_OCR_21_22               (((uint32_t) 1) << 9)
/** Support voltage range 2.2-2.3 (this bit is reserved in SDC)*/
#define SDC_OCR_22_23               (((uint32_t) 1) << 10)
/** Support voltage range 2.3-2.4 (this bit is reserved in SDC)*/
#define SDC_OCR_23_24               (((uint32_t) 1) << 11)
/** Support voltage range 2.4-2.5 (this bit is reserved in SDC)*/
#define SDC_OCR_24_25               (((uint32_t) 1) << 12)
/** Support voltage range 2.5-2.6 (this bit is reserved in SDC)*/
#define SDC_OCR_25_26               (((uint32_t) 1) << 13)
/** Support voltage range 2.6-2.7 (this bit is reserved in SDC)*/
#define SDC_OCR_26_27               (((uint32_t) 1) << 14)
/** Support voltage range 2.7-2.8 */
#define SDC_OCR_27_28               (((uint32_t) 1) << 15)
/** Support voltage range 2.8-2.9*/
#define SDC_OCR_28_29               (((uint32_t) 1) << 16)
/** Support voltage range 2.9-3.0 */
#define SDC_OCR_29_30               (((uint32_t) 1) << 17)
/** Support voltage range 3.0-3.1 */
#define SDC_OCR_30_31               (((uint32_t) 1) << 18)
/** Support voltage range 3.1-3.2 */
#define SDC_OCR_31_32               (((uint32_t) 1) << 19)
/** Support voltage range 3.2-3.3 */
#define SDC_OCR_32_33               (((uint32_t) 1) << 20)
/** Support voltage range 3.3-3.4 */
#define SDC_OCR_33_34               (((uint32_t) 1) << 21)
/** Support voltage range 3.4-3.5 */
#define SDC_OCR_34_35               (((uint32_t) 1) << 22)
/** Support voltage range 3.5-3.6 */
#define SDC_OCR_35_36               (((uint32_t) 1) << 23)
/** Support voltage range 2.7-3.6 */
#define SDC_OCR_27_36               ((uint32_t) 0x00FF8000)
/** Card Capacity Status (CCS). (this bit is reserved in MMC) */
#define SDC_OCR_HC_CCS              (((uint32_t) 1) << 30)
/** Card power up status bit */
#define SDC_OCR_IDLE                (((uint32_t) 1) << 31)
#define SDC_OCR_BUSY                (((uint32_t) 0) << 31)

/**
 * @brief SD/MMC Standard Command list definitions
 */

typedef enum {
	/* Basic Commands (Class 0) */
	SDC_CMD0_GO_IDLE_STATE,				/*!< GO_IDLE_STATE(MMC) or RESET(SD) */
	SDC_CMD1_SEND_OP_COND,				/*!< SEND_OP_COND(MMC)*/
	SDC_CMD2_ALL_SEND_CID,				/*!< ALL_SEND_CID */
	SDC_CMD3_SET_RELATIVE_ADDR,				/*!< SET_RELATE_ADDR */
	SDC_CMD3_SEND_RELATIVE_ADDR,			/*!< SEND_RELATE_ADDR */
	SDC_CMD7_SELECT_CARD,				/*!< SELECT/DESELECT_CARD */
	SDC_CMD8_SEND_IF_COND,				/*!<SEND_IF_COND */
	SDC_CMD9_SEND_CSD,					/*!< SEND_CSD */
	SDC_CMD12_STOP_TRANSMISSION,			/*!<STOP_TRANSMISSION */
	SDC_CMD13_SEND_STATUS,				/*!< SEND_STATUS */

	/* Block-Oriented Read Commands (class 2) */
	SDC_CMD16_SET_BLOCK_LEN,				/*!<SET_BLOCK_LEN */
	SDC_CMD17_READ_SINGLE_BLOCK,			/*!<READ_SINGLE_BLOCK */
	SDC_CMD18_READ_MULTIPLE_BLOCK,		/*!<READ_MULTIPLE_BLOCK */
	/* Class 3 */

	/* Block-Oriented Write Commands (class 4) */
	SDC_CMD24_WRITE_BLOCK,		/*!<WRITE_BLOCK */
	SDC_CMD25_WRITE_MULTIPLE_BLOCK,			/*!<WRITE_MULTIPLE_BLOCK */

	/* Erase Commands (class 5) */
	SDC_CMD32_ERASE_WR_BLK_START,		/*!<ERASE_WR_BLK_START */
	SDC_CMD33_ERASE_WR_BLK_END,				/*!<ERASE_WR_BLK_END */
	SDC_CMD38_ERASE,						/*!<ERASE */

	/* Application-Specific Commands (class 8) */
	SDC_CMD55_APP_CMD,					/*!<APP_CMD, the following will a ACMD */
	SDC_ACMD6_SET_BUS_WIDTH,			/*!<SET_BUS_WIDTH*/
	SDC_ACMD13_SEND_SD_STATUS,			/*!<SD_SEND_SD_STATUS*/
	SDC_ACMD41_SEND_APP_OP_COND,		/*!< SD_SEND_OP_COND */

} SDC_COMMAND_T;

/**
 * @brief SD/MMC Command descriptions (Physical Layer Simplified Specification Version 3.01)
 */
/*		Command							   type  argument	     response */

/* class 0 */
#define CMD0_GO_IDLE_STATE           0		/* bc                          */
#define CMD1_SEND_OP_COND          1		/* bcr  [31:0]  OCR       R3  */
#define CMD2_ALL_SEND_CID           2		/* bcr                           R2  */
#define CMD3_SET_RELATIVE_ADDR      3		/* bcr   [31:16] RCA      R1  */
#define CMD3_SEND_RELATIVE_ADDR     3		/* bcr                           R6  */
#define CMD4_SET_DSR               4		/* bc   [31:16] DSR            */
#define CMD7_SELECT_CARD           7		/* ac   [31:16] RCA        R1b  */
#define CMD8_SEND_IF_COND          8		/* bcr [11:8]VHS [7:0]check pattern      R7  */
#define CMD9_SEND_CSD              9		/* ac   [31:16] RCA        R2  */
#define CMD10_SEND_CID             10		/* ac   [31:16] RCA        R2  */
#define CMD12_STOP_TRANSMISSION    12		/* ac                      R1b */
#define CMD13_SEND_STATUS          13		/* ac   [31:16] RCA        R1  */
#define CMD15_GO_INACTIVE_STATE    15		/* ac   [31:16] RCA            */

/* class 2 */
#define CMD16_SET_BLOCKLEN         16		/* ac   [31:0]  block len  R1  */
#define CMD17_READ_SINGLE_BLOCK    17		/* adtc [31:0]  data addr  R1  */
#define CMD18_READ_MULTIPLE_BLOCK  18		/* adtc [31:0]  data addr  R1  */

/* class 4 */
#define CMD24_WRITE_BLOCK          24		/* adtc [31:0]  data addr  R1  */
#define CMD25_WRITE_MULTIPLE_BLOCK 25		/* adtc  [31:0]  data addr  R1  */

/* class 5 */
#define CMD32_ERASE_WR_BLK_START    32		/* ac   [31:0]  data addr  R1  */
#define CMD33_ERASE_WR_BLK_END      33		/* ac   [31:0]  data addr  R1  */
#define CMD38_ERASE                 38		/* ac                      R1b */

/* class 8 */
#define CMD55_APP_CMD              55		/* ac   [31:16] RCA        R1  */

/* Application commands */
#define ACMD6_SET_BUS_WIDTH         6		/* ac   [1:0]   bus width  R1   */
#define ACMD13_SEND_SD_STATUS       13		/* ac   ......................... R1   */
#define ACMD41_SD_SEND_OP_COND      41		/* bcr [30]HCS [28]XPC [24]S18R [23:0]  OCR        R3  */

/**
 * @brief SD/MMC Response type definitions
 */
#define CMDRESP_R1_TYPE         (SDC_SHORT_RESPONSE)
#define CMDRESP_R1b_TYPE        (SDC_SHORT_RESPONSE)
#define CMDRESP_R2_TYPE         (SDC_LONG_RESPONSE)
#define CMDRESP_R3_TYPE         (SDC_SHORT_RESPONSE)
#define CMDRESP_R6_TYPE         (SDC_SHORT_RESPONSE)
#define CMDRESP_R7_TYPE         (SDC_SHORT_RESPONSE)

/**
 * @brief Card Status (coded in 32 bits) in R1 & R1b Response definitions
 */
/** The command's argument was out of the allowed range for this card. */
#define SDC_CARD_STATUS_OUT_OF_RANGE    (((uint32_t) 1 ) << 31)
/** A misaligned address which did not match the block length was used in the command. */
#define SDC_CARD_STATUS_ADDRESS_ERROR   (((uint32_t) 1 ) << 30)
/** The transferred block length is not allowed for this card, or the number of transferred bytes does not match
   the block length.*/
#define SDC_CARD_STATUS_BLOCK_LEN_ERROR (((uint32_t) 1 ) << 29)
/**An error in the sequence of erase commands occurred.*/
#define SDC_CARD_STATUS_ERASE_SEQ_ERROR (((uint32_t) 1 ) << 28)
/**An invalid selection of write-blocks for erase occurred.*/
#define SDC_CARD_STATUS_ERASE_PARAM_ERROR (((uint32_t) 1 ) << 27)
/**Set when the host attempts to write to a protected block or to the temporary or permanent write protected card.*/
#define SDC_CARD_STATUS1_WP_VIOLATION    (((uint32_t) 1 ) << 26)
/**When set, signals that the card is locked by the host */
#define SDC_CARD_STATUS_SDC_IS_LOCKED  (((uint32_t) 1 ) << 25)
/** Set when a sequence or password error has been detected in lock/unlock card command.*/
#define SDC_CARD_STATUS_LOCK_UNLOCK_FAILED   (((uint32_t) 1 ) << 24)
/** The CRC check of the previous command failed.*/
#define SDC_CARD_STATUS_COM_CRC_ERROR   (((uint32_t) 1 ) << 23)
/** Command not legal for the card state*/
#define SDC_CARD_STATUS_ILLEGAL_COMMAND (((uint32_t) 1 ) << 22)
/**Card internal ECC was applied but failed to correct the data. */
#define SDC_CARD_STATUS_SDC_ECC_FAILED (((uint32_t) 1 ) << 21)
/**Internal card controller error. */
#define SDC_CARD_STATUS_CC_ERROR        (((uint32_t) 1 ) << 20)
/**A general or an unknown error occurred during the operation */
#define SDC_CARD_STATUS_ERROR       (((uint32_t) 1 ) << 19)
/** Can be either one of the following errors:
    - The read only section of the CSD does not match the card content.
    - An attempt to reverse the copy (set as original) or permanent WP (unprotected) bits was made. */
#define SDC_CARD_STATUS_CSD_OVERWRITE   (((uint32_t) 1 ) << 16)
/**Set when only partial address space was erased due to existing write protected blocks or the
   temporary or permanent write protected card was erased.*/
#define SDC_CARD_STATUS_WP_ERASE_SKIP   (((uint32_t) 1 ) << 15)
/** The command has been executed without using the internal ECC.*/
#define SDC_CARD_STATUS_SDC_ECC_DISABLED   (((uint32_t) 1 ) << 14)
/** An erase sequence was cleared before executing because an out of erase sequence command was received*/
#define SDC_CARD_STATUS_ERASE_RESET     (((uint32_t) 1 ) << 13)
/** The current card state*/
#define SDC_CARD_STATUS_CARDSTATE(x)              ((x >> 9) & 0x0F)
/** Corresponds to buffer empty signaling on the bus */
#define SDC_CARD_STATUS_READY_FOR_DATA  (((uint32_t) 1 ) << 8)
/** The card will expect ACMD, or an indication that the command has been interpreted as ACMD*/
#define SDC_CARD_STATUS_ACMD_ENABLE     (((uint32_t) 1 ) << 5)
/* Card error status */
#define SDC_CARD_STATUS_ERR_MASK         (0xFDF88008)

/**
 * @brief Card Current State definitions
 */
typedef enum {
	SDC_CARD_STATE_IDLE = 0,			/*!< Idle state*/
	SDC_CARD_STATE_READY = 1,			/*!< Ready state */
	SDC_CARD_STATE_IDENT = 2,			/*!< Identification state*/
	SDC_CARD_STATE_STBY = 3,			/*!< Stand-by state */
	SDC_CARD_STATE_TRAN = 4,			/*!< Transfer state */
	SDC_CARD_STATE_DATA = 5,			/*!< Sending-data state */
	SDC_CARD_STATE_RCV = 6,				/*!< Receive-data state */
	SDC_CARD_STATE_PRG = 7,				/*!< Programming state */
	SDC_CARD_STATE_DIS = 8,				/*!< Disconnect state */
	SDC_CARD_STATE_UNKNOWN = -1,		/*!< Unknown state */
} SDC_CARD_STATE_T;

/**
 * @brief R3 response definitions
 */
#define CMDRESP_R3_OCR_VAL(n)           (((uint32_t) n) & 0xFFFFFF)
#define CMDRESP_R3_S18A                 (((uint32_t) 1 ) << 24)
#define CMDRESP_R3_HC_CCS               (((uint32_t) 1 ) << 30)
#define CMDRESP_R3_INIT_COMPLETE        (((uint32_t) 1 ) << 31)

/**
 * @brief R6 response definitions
 */
#define CMDRESP_R6_RCA_VAL(n)           (((uint32_t) (n >> 16)) & 0xFFFF)
#define CMDRESP_R6_CARD_STATUS(n)       (((uint32_t) (n & 0x1FFF)) | \
										 ((n & (1 << 13)) ? (1 << 19) : 0) | \
										 ((n & (1 << 14)) ? (1 << 22) : 0) | \
										 ((n & (1 << 15)) ? (1 << 23) : 0))

/**
 * @brief R7 response definitions
 */
/** Echo-back of check-pattern */
#define CMDRESP_R7_CHECK_PATTERN(n)     (((uint32_t) n ) & 0xFF)
/** Voltage accepted */
#define CMDRESP_R7_VOLTAGE_ACCEPTED     (((uint32_t) 1 ) << 8)

/**
 * @brief CMD3 command definitions
 */
/** Card Address */
#define CMD3_RCA(n)         (((uint32_t) (n & 0xFFFF) ) << 16)

/**
 * @brief CMD7 command definitions
 */
/** Card Address */
#define CMD7_RCA(n)         (((uint32_t) (n & 0xFFFF) ) << 16)

/**
 * @brief CMD8 command definitions
 */
/** Check pattern */
#define CMD8_CHECKPATTERN(n)            (((uint32_t) (n & 0xFF) ) << 0)
/** Recommended pattern */
#define CMD8_DEF_PATTERN                    (0xAA)
/** Voltage supplied.*/
#define CMD8_VOLTAGESUPPLIED_27_36     (((uint32_t) 1 ) << 8)

/**
 * @brief CMD9 command definitions
 */
#define CMD9_RCA(n)         (((uint32_t) (n & 0xFFFF) ) << 16)

/**
 * @brief CMD13 command definitions
 */
#define CMD13_RCA(n)            (((uint32_t) (n & 0xFFFF) ) << 16)

/**
 * @brief APP_CMD command definitions
 */
#define CMD55_RCA(n)            (((uint32_t) (n & 0xFFFF) ) << 16)

/**
 * @brief ACMD41 command definitions
 */
#define ACMD41_OCR(n)                   (((uint32_t) n) & 0xFFFFFF)
#define ACMD41_S18R                     (((uint32_t) 1 ) << 24)
#define ACMD41_XPC                      (((uint32_t) 1 ) << 28)
#define ACMD41_HCS                      (((uint32_t) 1 ) << 30)

/**
 * @brief ACMD6 command definitions
 */
#define ACMD6_BUS_WIDTH(n)              ((uint32_t) n & 0x03)
#define ACMD6_BUS_WIDTH_1               (0)
#define ACMD6_BUS_WIDTH_4               (2)

#define SDC_SECTOR_SIZE             512

/**
 * @brief SDC Return code definitions
 */
typedef enum {
	SDC_RET_OK = 0,
	SDC_RET_CMD_FAILED = -1,
	SDC_RET_BAD_PARAMETERS = -2,
	SDC_RET_BUS_NOT_IDLE = -3,
	SDC_RET_TIMEOUT = -4,
	SDC_RET_ERR_STATE = -5,
	SDC_RET_NOT_READY = -6,
	SDC_RET_FAILED = -7,
} SDC_RET_CODE_T;

/**
 * @brief Card type definitions
 */
#define CARD_TYPE_SD    (((uint32_t) 1) << 0)
#define CARD_TYPE_4BIT  (((uint32_t) 1) << 1)
#define CARD_TYPE_8BIT  (((uint32_t) 1) << 2)
#define CARD_TYPE_HC    (SDC_OCR_HC_CCS)/*!< high capacity card > 2GB */

#ifdef SDC_DMA_ENABLE
/**
 * @brief SDC Event structure
 */
typedef struct {
	uint8_t DmaChannel;		/*!<DMA Channel used for transfer data */
} SDC_EVENT_Type;
#else
/**
 * @brief SDC Event structure
 */
typedef struct {
	void *Buffer;		/*!<pointer to buffer storing data*/
	uint32_t Size;			/*!<transfer size*/
	uint32_t Index;			/*!<current transfer index*/
	uint8_t  Dir;			/*!<0: transmit, 1: receive*/
} SDC_EVENT_Type;
#endif /*SDC_DMA_ENABLE*/

/* Function prototype for event setup function */
typedef void (*SDC_EVSETUP_FUNC_T)(SDC_EVENT_Type *pEvent);

/* Function prototype for wait for event function */
typedef int32_t (*SDC_EVWAIT_FUNC_T)(void);

/* Function prototype for milliSecond delay function */
typedef void (*SDC_MSDELAY_FUNC_T)(uint32_t);

/* Card specific setup data */
typedef struct {
	uint32_t Response[4];						/*!< Most recent response */
	uint32_t CID[4];							/*!< CID of acquired card  */
	uint32_t CSD[4];							/*!< CSD of acquired card */
	// uint32_t ExtCSD[512 / 4];
	uint32_t CardType;
	uint16_t RCA;								/*!< Relative address assigned to card */
	uint32_t Speed;
	uint32_t BlockLen;							/*!< Card sector size*/
	uint32_t DeviceSize;
	uint32_t BlockNR;
	uint32_t ClockRate;
	SDC_EVSETUP_FUNC_T fnEventSetup;			/*!< Function to setup event information*/
	SDC_EVWAIT_FUNC_T fnWaitEvent;				/*!< Function to wait for event*/
	SDC_MSDELAY_FUNC_T fnMsDelay;				/*!< Function to sleep in ms*/
} SDC_CARD_Type;

/**
 * @brief	Initializes the SDC card controller
 * @return	None
 */
void Chip_SDC_Init(void);

/**
 * @brief	Set the frequency of SD_CLK
 * @param	freq		: expected frequency
 * @return	None
 */
void Chip_SDC_SetClock(uint32_t freq);

/**
 * @brief	Shutdown the SDC card controller
 * @return	None
 */
void Chip_SDC_DeInit(void);

/**
 * @brief	Sets the SD interface interrupt mask
 * @param	iVal	: Interrupts to enable, Or'ed values SDC_MASK0_*
 * @return	None
 */
STATIC INLINE void Chip_SDC_SetIntMask(uint32_t iVal)
{
	IP_SDC_SetIntMask(LPC_SDC, iVal);
}

/**
 * @brief	SD card interrupt service routine
 * @param	txBuf	: pointer to TX Buffer. If it is NULL, dont send data to card.
 * @param	txCnt	: pointer to buffer storing the current transmit index.
 * @param	rxBuf	: pointer to RX Buffer. If it is NULL, dont read data from card.
 * @param	rxCnt	: pointer to buffer storing the current receive index.
 * @return	None
 */
int32_t Chip_SDC_IRQHandler (uint8_t *txBuf, uint32_t *txCnt,
							 uint8_t *rxBuf, uint32_t *rxCnt);

/**
 * @brief	Returns the current SD status, clears pending ints, and disables all ints
 * @return	Current status of Or'ed values SDC_STATUS_*
 */
STATIC INLINE uint32_t Chip_SDC_GetStatus(void)
{
	return IP_SDC_GetStatus(LPC_SDC);
}

/**
 * @brief	Function to enumerate the SD/MMC/SDHC/MMC+ cards
 * @param	pCardInfo	: Pointer to pre-allocated card info structure
 * @return	1 if a card is acquired, otherwise 0
 */
int32_t Chip_SDC_Acquire(SDC_CARD_Type *pCardInfo);

/**
 * @brief	Get card's current state (idle, transfer, program, etc.)
 * @param	pCardInfo	: Card information
 * @return	Current SD card  state
 */
SDC_CARD_STATE_T Chip_SDC_GetCardState(SDC_CARD_Type *pCardInfo);

/**
 * @brief	Get 'card status' of SD Memory card
 * @param	pCardInfo	: Card information
 * @return	Current SD card status
 */
uint32_t Chip_SDC_GetCardStatus(SDC_CARD_Type *pCardInfo);

/**
 * @brief	Get 'sd status' of SD Memory card
 * @param	pCardInfo	: Card information
 * @param	pStatus		: buffer storing status. it must be 64-byte-length.
 * @return	the number of byte read
 */
int32_t Chip_SDC_GetSDStatus(SDC_CARD_Type *pCardInfo, uint32_t *pStatus);

/**
 * @brief	Performs the read of data from the SD/MMC card
 * @param	pCardInfo	: Pointer to Card information structure
 * @param	buffer		: Pointer to data buffer to copy to
 * @param	startblock	: Start block number
 * @param	blockNum	: Number of block to read
 * @return	Bytes read, or 0 on error
 */
int32_t Chip_SDC_ReadBlocks(SDC_CARD_Type *pCardInfo, void *buffer, int32_t startblock, int32_t blockNum);

/**
 * @brief	Performs write of data to the SD/MMC card
 * @param	pCardInfo	: Pointer to Card information structure
 * @param	buffer		: Pointer to data buffer to copy to
 * @param	startblock	: Start block number
 * @param	blockNum	: Number of block to write
 * @return	Number of bytes actually written, or 0 on error
 */
int32_t Chip_SDC_WriteBlocks(SDC_CARD_Type *pCardInfo, void *buffer, int32_t startblock, int32_t blockNum);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SDC_17XX_40XX_H_ */
