/*
 * @brief LPC17xx/40xx EEPROM driver
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

#ifndef EEPROM_17XX_40XX_H_
#define EEPROM_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup EEPROM_17XX_40XX CHIP: LPC17xx/40xx EEPROM Driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/* EEPROM mode type */
typedef EEPROM_001_Mode_T EEPROM_Mode_T;

/**
 * @brief	Initializes EEPROM
 * @return	Nothing
 */
void Chip_EEPROM_Init(void);

/**
 * @brief	De-initializes EEPROM
 * @return	Nothing
 */
STATIC INLINE void Chip_EEPROM_DeInit(void)
{
	IP_EEPROM_DeInit(LPC_EEPROM);
}

/**
 * @brief	Write data to EEPROM at specific address
 * @param	page_offset	: offset of data in page register(0 - 63)
 * @param	page_address: page address (0-62)
 * @param	data		: buffer that contain data that will be written to buffer
 * @param	mode		: Read mode, should be:
 *                  - MODE_8_BIT    : read 8 bit mode
 *                  - MODE_16_BIT   : read 16 bit mode
 *                  - MODE_32_BIT   : read 32 bit mode
 * @param	size		: number written data (bytes)
 * @return	SUCCESS on successful write of data, or ERROR
 * @note	This function actually write data into EEPROM memory and automatically
 * write into next page if current page is overflowed
 */
STATIC INLINE Status Chip_EEPROM_Write(uint16_t page_offset,
									   uint16_t page_address,
									   void *data,
									   EEPROM_Mode_T mode,
									   uint32_t size)
{
	return IP_EEPROM_Write(LPC_EEPROM, page_offset, page_address, data, mode, size);
}

/**
 * @brief	Read data to EEPROM at specific address
 * @param	page_offset	: offset of data in page register(0 - 63)
 * @param	page_address: page address (0-62)
 * @param	data		: buffer that contain data read from read data register
 * @param	mode		: Read mode, should be:
 *                  - MODE_8_BIT    : read 8 bit mode
 *                  - MODE_16_BIT   : read 16 bit mode
 *                  - MODE_32_BIT   : read 32 bit mode
 * @param	size		: number read data (bytes)
 * @return	Nothing
 */
STATIC INLINE void Chip_EEPROM_Read(uint16_t page_offset,
									uint16_t page_address,
									void *data,
									EEPROM_Mode_T mode,
									uint32_t size)
{
	IP_EEPROM_Read(LPC_EEPROM, page_offset, page_address, data, mode, size);
}

/**
 * @brief	Erase a page at the specific address
 * @param	address		: EEPROM page address (0-62)
 * @return	Nothing
 */
STATIC INLINE void Chip_EEPROM_Erase(uint16_t address)
{
	IP_EEPROM_Erase(LPC_EEPROM, address);
}

/**
 * @brief	Enable/Disable EEPROM power down mode
 * @param	NewState	: PowerDown mode state, should be:
 *                  - ENABLE: Enable power down mode
 *                  - DISABLE: Disable power down mode
 * @return	Nothing
 */
STATIC INLINE void Chip_EEPROM_PowerDown(FunctionalState NewState)
{
	IP_EEPROM_PowerDown(LPC_EEPROM, NewState);
}

/**
 * @}
 */

 #ifdef __cplusplus
}
#endif

#endif /* EEPROM_17XX_40XX_H_ */
