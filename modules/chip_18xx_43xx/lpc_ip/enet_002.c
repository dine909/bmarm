/*
 * @brief Ethernet control functions
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

#include "enet_002.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Saved address for PHY and clock divider */
STATIC uint32_t phyAddr;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Resets ethernet interface */
void IP_ENET_Reset(IP_ENET_002_Type *pENET)
{
	/* This should be called prior to IP_ENET_Init. The MAC controller may
	   not be ready for a call to init right away so a small delay should
	   occur after this call. */
	pENET->MAC.MAC1 = ENET_MAC1_RESETTX | ENET_MAC1_RESETMCSTX | ENET_MAC1_RESETRX |
					  ENET_MAC1_RESETMCSRX | ENET_MAC1_SIMRESET | ENET_MAC1_SOFTRESET;
	pENET->CONTROL.COMMAND = ENET_COMMAND_REGRESET | ENET_COMMAND_TXRESET | ENET_COMMAND_RXRESET |
							 ENET_COMMAND_PASSRUNTFRAME;
}

/* Sets the address of the interface */
void IP_ENET_SetADDR(IP_ENET_002_Type *pENET, const uint8_t *macAddr)
{
	/* Save MAC address */
	pENET->MAC.SA[0] = ((uint32_t) macAddr[5]) | ((uint32_t) macAddr[4] << 8);
	pENET->MAC.SA[1] = ((uint32_t) macAddr[3]) | ((uint32_t) macAddr[2] << 8);
	pENET->MAC.SA[2] = ((uint32_t) macAddr[1]) | ((uint32_t) macAddr[0] << 8);
}

/* Initialize ethernet interface */
void IP_ENET_Init(IP_ENET_002_Type *pENET)
{
	/* Initial MAC configuration for  full duplex,
	   100Mbps, inter-frame gap use default values */
	pENET->MAC.MAC1 = ENET_MAC1_PARF;
	pENET->MAC.MAC2 = ENET_MAC2_FULLDUPLEX | ENET_MAC2_CRCEN | ENET_MAC2_PADCRCEN;
#if defined(USE_RMII)
	pENET->CONTROL.COMMAND = ENET_COMMAND_FULLDUPLEX | ENET_COMMAND_RMII;
#else
	pENET->CONTROL.COMMAND = ENET_COMMAND_FULLDUPLEX;
#endif
	pENET->MAC.IPGT = ENET_IPGT_FULLDUPLEX;
	pENET->MAC.IPGR = ENET_IPGR_P2_DEF;
	pENET->MAC.SUPP = ENET_SUPP_100Mbps_SPEED;
	pENET->MAC.MAXF = ENET_ETH_MAX_FLEN;
	pENET->MAC.CLRT = ENET_CLRT_DEF;

	/* Setup default filter */
	pENET->CONTROL.COMMAND |= ENET_COMMAND_PASSRXFILTER;

	/* Clear all MAC interrupts */
	pENET->MODULE_CONTROL.INTCLEAR = 0xFFFF;

	/* Disable MAC interrupts */
	pENET->MODULE_CONTROL.INTENABLE = 0;
}

/* Sets up the PHY link clock divider and PHY address */
void IP_ENET_SetupMII(IP_ENET_002_Type *pENET, uint8_t div, uint8_t addr)
{
	/* Save clock divider and PHY address in MII address register */
	phyAddr = ENET_MADR_PHYADDR(addr);

	/*  Write to MII configuration register and reset */
	pENET->MAC.MCFG = ENET_MCFG_CLOCKSEL(div) | ENET_MCFG_RES_MII;

	/* release reset */
	pENET->MAC.MCFG &= ~(ENET_MCFG_RES_MII);
}

/*De-initialize the ethernet interface */
void IP_ENET_DeInit(IP_ENET_002_Type *pENET)
{
	/* Disable packet reception */
	pENET->MAC.MAC1 &= ~ENET_MAC1_RXENABLE;
	pENET->CONTROL.COMMAND = 0;

	/* Clear all MAC interrupts */
	pENET->MODULE_CONTROL.INTCLEAR = 0xFFFF;

	/* Disable MAC interrupts */
	pENET->MODULE_CONTROL.INTENABLE = 0;
}

/* Starts a PHY write via the MII */
void IP_ENET_StartMIIWrite(IP_ENET_002_Type *pENET, uint8_t reg, uint16_t data)
{
	/* Write value at PHY address and register */
	pENET->MAC.MCMD = 0;
	pENET->MAC.MADR = phyAddr | ENET_MADR_REGADDR(reg);
	pENET->MAC.MWTD = data;
}

/*Starts a PHY read via the MII */
void IP_ENET_StartMIIRead(IP_ENET_002_Type *pENET, uint8_t reg)
{
	/* Read value at PHY address and register */
	pENET->MAC.MADR = phyAddr | ENET_MADR_REGADDR(reg);
	pENET->MAC.MCMD = ENET_MCMD_READ;
}

/* Returns MII link (PHY) busy status */
bool IP_ENET_IsMIIBusy(IP_ENET_002_Type *pENET)
{
	if (pENET->MAC.MIND & ENET_MIND_BUSY) {
		return true;
	}

	return false;
}

/* Read MII data */
uint16_t IP_ENET_ReadMIIData(IP_ENET_002_Type *pENET)
{
	pENET->MAC.MCMD = 0;
	return pENET->MAC.MRDD;
}

/* Enables or disables ethernet transmit */
void IP_ENET_TXEnable(IP_ENET_002_Type *pENET, bool Enable)
{
	if (Enable) {
		/* Descriptor list head pointers must be setup prior to enable */
		pENET->CONTROL.COMMAND |= ENET_COMMAND_TXENABLE;
	}
	else {
		pENET->CONTROL.COMMAND &= ~ENET_COMMAND_TXENABLE;
	}
}

/* Enables or disables ethernet packet reception */
void IP_ENET_RXEnable(IP_ENET_002_Type *pENET, bool Enable)
{
	if (Enable) {
		/* Descriptor list head pointers must be setup prior to enable */
		pENET->CONTROL.COMMAND |= ENET_COMMAND_RXENABLE;
		pENET->MAC.MAC1 |= ENET_MAC1_RXENABLE;
	}
	else {
		pENET->CONTROL.COMMAND &= ~ENET_COMMAND_RXENABLE;
		pENET->MAC.MAC1 &= ~ENET_MAC1_RXENABLE;
	}
}

/* Sets full or half duplex for the interface */
void IP_ENET_SetDuplex(IP_ENET_002_Type *pENET, bool full)
{
	if (full) {
		pENET->MAC.MAC2 |= ENET_MAC2_FULLDUPLEX;
		pENET->CONTROL.COMMAND |= ENET_COMMAND_FULLDUPLEX;
		pENET->MAC.IPGT = ENET_IPGT_FULLDUPLEX;
	}
	else {
		pENET->MAC.MAC2 &= ~ENET_MAC2_FULLDUPLEX;
		pENET->CONTROL.COMMAND &= ~ENET_COMMAND_FULLDUPLEX;
		pENET->MAC.IPGT = ENET_IPGT_HALFDUPLEX;
	}
}

/* Sets speed for the interface */
void IP_ENET_SetSpeed(IP_ENET_002_Type *pENET, bool speed100)
{
	if (speed100) {
		pENET->MAC.SUPP = ENET_SUPP_100Mbps_SPEED;
	}
	else {
		pENET->MAC.SUPP = 0;
	}
}

/* Configures the initial ethernet transmit descriptors */
void IP_ENET_InitTxDescriptors(IP_ENET_002_Type *pENET,
							   IP_ENET_002_TXDESC_Type *pDescs,
							   IP_ENET_002_TXSTAT_Type *pStatus,
							   uint32_t descNum)
{
	/* Setup descriptor list base addresses */
	pENET->CONTROL.TX.DESCRIPTOR = (uint32_t) pDescs;
	pENET->CONTROL.TX.DESCRIPTORNUMBER = descNum - 1;
	pENET->CONTROL.TX.STATUS = (uint32_t) pStatus;
	pENET->CONTROL.TX.PRODUCEINDEX = 0;
}

/* Configures the initial ethernet receive descriptors */
void IP_ENET_InitRxDescriptors(IP_ENET_002_Type *pENET,
							   IP_ENET_002_RXDESC_Type *pDescs,
							   IP_ENET_002_RXSTAT_Type *pStatus,
							   uint32_t descNum)
{
	/* Setup descriptor list base addresses */
	pENET->CONTROL.RX.DESCRIPTOR = (uint32_t) pDescs;
	pENET->CONTROL.RX.DESCRIPTORNUMBER = descNum - 1;
	pENET->CONTROL.RX.STATUS = (uint32_t) pStatus;
	pENET->CONTROL.RX.CONSUMEINDEX = 0;
}

/* Get status for the descriptor list */
IP_ENET_002_BUFF_STATUS IP_ENET_GetBufferStatus(uint16_t produceIndex, uint16_t consumeIndex, uint16_t buffSize)
{
	/* Empty descriptor list */
	if (consumeIndex == produceIndex) {
		return ENET_BUFF_EMPTY;
	}

	/* Full descriptor list */
	if ((consumeIndex == 0) &&
		(produceIndex == (buffSize - 1))) {
		return ENET_BUFF_FULL;
	}

	/* Wrap-around, full descriptor list */
	if (consumeIndex == (produceIndex + 1)) {
		return ENET_BUFF_FULL;
	}

	return ENET_BUFF_PARTIAL_FULL;
}

/* Increase the current Tx Produce Descriptor Index */
uint16_t IP_ENET_IncTXProduceIndex(IP_ENET_002_Type *pENET)
{
	/* Get current TX produce index */
	uint32_t idx = pENET->CONTROL.TX.PRODUCEINDEX;

	/* Start frame transmission by incrementing descriptor */
	idx++;
	if (idx > pENET->CONTROL.TX.DESCRIPTORNUMBER) {
		idx = 0;
	}
	pENET->CONTROL.TX.PRODUCEINDEX = idx;

	return idx;
}

/* Increase the current Rx Consume Descriptor Index */
uint16_t IP_ENET_IncRXConsumeIndex(IP_ENET_002_Type *pENET)
{
	/* Get current RX consume index */
	uint32_t idx = pENET->CONTROL.RX.CONSUMEINDEX;

	/* Consume descriptor */
	idx++;
	if (idx > pENET->CONTROL.RX.DESCRIPTORNUMBER) {
		idx = 0;
	}
	pENET->CONTROL.RX.CONSUMEINDEX = idx;

	return idx;
}
