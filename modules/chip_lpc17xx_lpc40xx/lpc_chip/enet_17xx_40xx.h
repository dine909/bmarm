/*
 * @brief LPC17xx/40xx ethernet driver
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

#ifndef __ENET_17XX_40XX_H_
#define __ENET_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ENET_17XX_40XX CHIP: LPC17xx/40xx Ethernet driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/** @defgroup ENET_17XX_40XX_OPTIONS CHIP: LPC17xx/40xx Ethernet driver build options
 * @ingroup CHIP_17XX_40XX_DRIVER_OPTIONS ENET_17XX_40XX
 * The ethernet driver has options that configure it's operation at build-time.<br>
 *
 * USE_RMII:<br>
 * When defined, the driver will be built for RMII operation.<br>
 * When not defined, the driver will be built for MII operation.<br>
 *
 * For more information on driver options see<br>
 * @ref LPCOPEN_DESIGN_ARPPROACH<br>
 * @{
 */

/**
 * @}
 */

typedef IP_ENET_002_TXDESC_Type     ENET_TXDESC_Type;
typedef IP_ENET_002_TXSTAT_Type     ENET_TXSTAT_Type;
typedef IP_ENET_002_RXDESC_Type     ENET_RXDESC_Type;
typedef IP_ENET_002_RXSTAT_Type     ENET_RXSTAT_Type;
typedef IP_ENET_002_BUFF_STATUS     ENET_BUFF_STATUS;

/**
 * @brief	Resets the ethernet interface
 * @return	Nothing
 * @note	Resets the ethernet interface. This should be called prior to
 * Chip_ENET_Init with a small delay after this call.
 */
STATIC INLINE void Chip_ENET_Reset(void)
{
	IP_ENET_Reset(LPC_ETHERNET);
}

/**
 * @brief	Sets the address of the interface
 * @param	macAddr	: Pointer to the 6 bytes used for the MAC address
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_SetADDR(const uint8_t *macAddr)
{
	IP_ENET_SetADDR(LPC_ETHERNET, macAddr);
}

/**
 * @brief	Sets up the PHY link clock divider and PHY address
 * @param	div		: Divider index, not a divider value, see user manual
 * @param	addr	: PHY address, used with MII read and write
 * @return	Nothing
 * @note	The MII clock divider rate is divided from the peripheral clock returned
 * from the Chip_SYSCON_GetCPUCLKRate() function. Use Chip_ENET_FindMIIDiv()
 * with a desired clock rate to find the correct divider index value.
 */
STATIC INLINE void Chip_ENET_SetupMII(uint32_t div, uint8_t addr)
{
	IP_ENET_SetupMII(LPC_ETHERNET, div, addr);
}

/**
 * @brief	Starts a PHY write via the MII
 * @param	reg		: PHY register to write
 * @param	data	: Data to write to PHY register
 * @return	Nothing
 * @note	Start a PHY write operation. Does not block, requires calling
 * IP_ENET_IsMIIBusy to determine when write is complete.
 */
STATIC INLINE void Chip_ENET_StartMIIWrite(uint8_t reg, uint16_t data)
{
	IP_ENET_StartMIIWrite(LPC_ETHERNET, reg, data);
}

/**
 * @brief	Starts a PHY read via the MII
 * @param	reg	: PHY register to read
 * @return	Nothing
 * @note	Start a PHY read operation. Does not block, requires calling
 * IP_ENET_IsMIIBusy to determine when read is complete and calling
 * IP_ENET_ReadMIIData to get the data.
 */
STATIC INLINE void Chip_ENET_StartMIIRead(uint8_t reg)
{
	IP_ENET_StartMIIRead(LPC_ETHERNET, reg);
}

/**
 * @brief	Returns MII link (PHY) busy status
 * @return	Returns true if busy, otherwise false
 */
STATIC INLINE bool Chip_ENET_IsMIIBusy(void)
{
	return IP_ENET_IsMIIBusy(LPC_ETHERNET);
}

/**
 * @brief	Returns the value read from the PHY
 * @return	Read value from PHY
 */
STATIC INLINE uint16_t Chip_ENET_ReadMIIData(void)
{
	return IP_ENET_ReadMIIData(LPC_ETHERNET);
}

/**
 * @brief	Enables or disables ethernet transmit
 * @param	Enable	: true to enable transmit, false to disable
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_TXEnable(bool Enable)
{
	IP_ENET_TXEnable(LPC_ETHERNET, Enable);
}

/**
 * @brief	Enables or disables ethernet packet reception
 * @param	Enable	: true to enable receive, false to disable
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_RXEnable(bool Enable)
{
	IP_ENET_RXEnable(LPC_ETHERNET, Enable);
}

/**
 * @brief	Sets full or half duplex operation for the interface
 * @param	full	: true to selected full duplex, false for half duplex operation
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_SetDuplex(bool full)
{
	IP_ENET_SetDuplex(LPC_ETHERNET, full);
}

/**
 * @brief	Sets speed for the interface
 * @param	speed100	: true to select 100Mbps mode, false for 10Mbps
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_SetSpeed(bool speed100)
{
	IP_ENET_SetSpeed(LPC_ETHERNET, speed100);
}

/**
 * @brief	Configures the initial ethernet transmit descriptors
 * @param	pDescs	: Pointer to TX descriptor list
 * @param	pStatus	: Pointer to TX status list
 * @param	descNum	: the number of desciptors
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_InitTxDescriptors(ENET_TXDESC_Type *pDescs,
											   ENET_TXSTAT_Type *pStatus,
											   uint32_t descNum)
{
	IP_ENET_InitTxDescriptors(LPC_ETHERNET, pDescs, pStatus, descNum);
}

/**
 * @brief	Configures the initial ethernet receive descriptors
 * @param	pDescs	: Pointer to TX descriptor list
 * @param	pStatus	: Pointer to TX status list
 * @param	descNum	: the number of desciptors
 * @return	Nothing
 */
STATIC INLINE void Chip_ENET_InitRxDescriptors(ENET_RXDESC_Type *pDescs,
											   ENET_RXSTAT_Type *pStatus,
											   uint32_t descNum)
{
	IP_ENET_InitRxDescriptors(LPC_ETHERNET, pDescs, pStatus, descNum);
}

/**
 * @brief	Get the current Tx Produce Descriptor Index
 * @return	Tx Produce Index
 */
STATIC INLINE uint16_t Chip_ENET_GetTXProduceIndex(void)
{
	return IP_ENET_GetTXProduceIndex(LPC_ETHERNET);
}

/**
 * @brief	Get the current Tx Consume Descriptor Index
 * @return	Tx Consume Index
 */
STATIC INLINE uint16_t Chip_ENET_GetTXConsumeIndex(void)
{
	return IP_ENET_GetTXConsumeIndex(LPC_ETHERNET);
}

/**
 * @brief	Get the current Rx Produce Descriptor Index
 * @return	Rx Produce Index
 */
STATIC INLINE uint16_t Chip_ENET_GetRXProduceIndex(void)
{
	return IP_ENET_GetRXProduceIndex(LPC_ETHERNET);
}

/**
 * @brief	Get the current Rx Consume Descriptor Index
 * @return	Rx Consume Index
 */
STATIC INLINE uint16_t Chip_ENET_GetRXConsumeIndex(void)
{
	return IP_ENET_GetRXConsumeIndex(LPC_ETHERNET);
}

/**
 * @brief	Get the buffer status with the current Produce Index and Consume Index
 * @param	produceIndex	: Produce Index
 * @param	consumeIndex	: Consume Index
 * @param	buffSize	: Buffer size
 * @return	Status (One of status value: ENET_BUFF_EMPTY/ENET_BUFF_FULL/ENET_BUFF_PARTIAL_FULL)
 */
STATIC INLINE ENET_BUFF_STATUS Chip_ENET_GetBufferStatus(uint16_t produceIndex,
														 uint16_t consumeIndex,
														 uint16_t buffSize)
{
	return IP_ENET_GetBufferStatus(produceIndex, consumeIndex, buffSize);
}

/**
 * @brief	Increase the current Tx Produce Descriptor Index
 * @return	The new index value
 */
STATIC INLINE uint16_t Chip_ENET_IncTXProduceIndex(void)
{
	return IP_ENET_IncTXProduceIndex(LPC_ETHERNET);
}

/**
 * @brief	Increase the current Rx Consume Descriptor Index
 * @return	The new index value
 */
STATIC INLINE uint16_t Chip_ENET_IncRXConsumeIndex(void)
{
	return IP_ENET_IncRXConsumeIndex(LPC_ETHERNET);
}

/**
 * @brief	Initialize ethernet interface
 * @return	Nothing
 * @note	Performs basic initialization of the ethernet interface in a default
 * state. This is enough to place the interface in a usable state, but
 * may require more setup outside this function.
 */
void Chip_ENET_Init(void);

/**
 * @brief	De-initialize the ethernet interface
 * @return	Nothing
 */
void Chip_ENET_DeInit(void);

/**
 * @brief	Find the divider index for a desired MII clock rate
 * @param	clockRate	: Clock rate to get divider index for
 * @return	MII divider index to get the closest clock rate for clockRate
 * @note	Use this function to get a divider index for the Chip_ENET_SetupMII()
 * function determined from the desired MII clock rate.
 */
uint32_t Chip_ENET_FindMIIDiv(uint32_t clockRate);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ENET_17XX_40XX_H_ */
