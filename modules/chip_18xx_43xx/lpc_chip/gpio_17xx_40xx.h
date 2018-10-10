/*
 * @brief LPC17xx/40xx GPIO registers and control functions
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

#ifndef __GPIO_17XX_40XX_H_
#define __GPIO_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup GPIO_17XX_40XX CHIP: LPC17xx/40xx GPIO driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/**
 * @brief	Initialize GPIO block
 * @return	Nothing
 */
STATIC INLINE void Chip_GPIO_Init(void)
{}

/**
 * @brief	Set a GPIO port/bit state
 * @param	Port	: GPIO port to set
 * @param	Bit		: GPIO bit to set
 * @param	Setting	: true for high, false for low
 * @return	Nothing
 */
STATIC INLINE void Chip_GPIO_WritePortBit(uint8_t Port, uint8_t Bit, bool Setting)
{
	IP_GPIO_WritePortBit((LPC_GPIO_Type *) (LPC_GPIO + Port), Bit, Setting);
}

/**
 * @brief	Set a GPIO direction
 * @param	Port	: GPIO port to set
 * @param	Bit		: GPIO bit to set
 * @param	Setting	: true for output, false for input
 * @return	Nothing
 */
STATIC INLINE void Chip_GPIO_WriteDirBit(uint8_t Port, uint8_t Bit, bool Setting)
{
	IP_GPIO_WriteDirBit((LPC_GPIO_Type *) (LPC_GPIO + Port), Bit, Setting);
}

/**
 * @brief	Read a GPIO state
 * @param	Port	: GPIO port to read
 * @param	Bit		: GPIO bit to read
 * @return	true of the GPIO is high, false if low
 */
STATIC INLINE bool Chip_GPIO_ReadPortBit(uint8_t Port, uint8_t Bit)
{
	return IP_GPIO_ReadPortBit((LPC_GPIO_Type *) (LPC_GPIO + Port), Bit);
}

/**
 * @brief	Read a GPIO direction (out or in)
 * @param	Port	: GPIO port to read
 * @param	Bit		: GPIO bit to read
 * @return	true of the GPIO is an output, false if input
 */
STATIC INLINE bool Chip_GPIO_ReadDirBit(uint8_t Port, uint8_t Bit)
{
	return IP_GPIO_ReadDirBit((LPC_GPIO_Type *) (LPC_GPIO + Port), Bit);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_17XX_40XX_H_ */
