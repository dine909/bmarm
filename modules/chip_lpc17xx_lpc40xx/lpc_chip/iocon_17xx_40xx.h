/*
 * @brief LPC17xx/40xx IOCON registers and control functions
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

#ifndef __IOCON_17XX_40XX_H_
#define __IOCON_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup IOCON_17XX_40XX CHIP: LPC17xx/40xx I/O configuration driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/**
 * @brief IOCON register block
 */
typedef struct {
	__IO uint32_t p[5][32];
} LPC_IOCON_Type;

/* Pin modes */
/** Macros to set pin mode (inactive, pull-down, pull-up, repeater */
#define MD_PLN  (0x0 << 3)
#define MD_PDN  (0x1 << 3)
#define MD_PUP  (0x2 << 3)
#define MD_RPT  (0x3 << 3)

/** Macro to enable hysteresis- use with Chip_IOCON_PinMux */
#define MD_HYS_ENA  (0x1 << 5)
/** Macro to disable hysteresis- use with Chip_IOCON_PinMux */
#define MD_HYS_DIS  (0x0 << 5)
/** Macro to enable input inversion- use with Chip_IOCON_PinMux */
#define MD_IINV_ENA (0x1 << 6)
/** Macro to disable input inversion- use with Chip_IOCON_PinMux */
#define MD_IINV_DIS (0x0 << 6)
/** Macro to enable simulated open drain mode- use with Chip_IOCON_PinMux */
#define MD_OD_ENA   (0x1 << 10)
/** Macro to disable simulated open drain mode- use with Chip_IOCON_PinMux */
#define MD_OD_DIS   (0x0 << 10)
/** Macro to enable I2C 50ns glitch filter and slew rate control- use with Chip_IOCON_PinMux */
#define MD_HS_ENA   (0x0 << 8)
/** Macro to disable I2C 50ns glitch filter and slew rate control- use with Chip_IOCON_PinMux */
#define MD_HS_DIS   (0x1 << 8)
/** Macro to enable analog mode (ADC)- use with Chip_IOCON_PinMux */
#define MD_ANA_ENA  (0x0 << 7)
/** Macro to disable analog mode (ADC)- use with Chip_IOCON_PinMux */
#define MD_ANA_DIS  (0x1 << 7)
/** Macro to enable input filter- use with Chip_IOCON_PinMux */
#define MD_FILT_ENA (0x0 << 8)
/** Macro to disable input filter- use with Chip_IOCON_PinMux */
#define MD_FILT_DIS (0x1 << 8)
/** Macro to enable DAC- use with Chip_IOCON_PinMux */
#define MD_DAC_ENA  (0x1 << 16)
/** Macro to disable DAC- use with Chip_IOCON_PinMux */
#define MD_DAC_DIS  (0x0 << 16)
/** Macro to enable standard mode, slew rate control is enabled - use with Chip_IOCON_PinMux */
#define MD_STD_SLEW_RATE   (0x0 << 9)
/** Macro to enable fast mode, slew rate control is disabled - use with Chip_IOCON_PinMux */
#define MD_FAST_SLEW_RATE  (0x1 << 9)
/** Macro to enable high drive output- use with Chip_IOCON_PinMux */
#define MD_HD_ENA  (0x1 << 9)
/** Macro to disable high drive output- use with Chip_IOCON_PinMux */
#define MD_HD_DIS  (0x0 << 9)

/** Macro to select pin function 0- use with Chip_IOCON_PinMux */
#define FUNC0           0x0				/** Function 0  */
/** Macro to select pin function 1- use with Chip_IOCON_PinMux */
#define FUNC1           0x1				/** Function 1  */
/** Macro to select pin function 2- use with Chip_IOCON_PinMux */
#define FUNC2           0x2				/** Function 2	*/
/** Macro to select pin function 3- use with Chip_IOCON_PinMux */
#define FUNC3           0x3				/** Function 3	*/
/** Macro to select pin function 4- use with Chip_IOCON_PinMux */
#define FUNC4           0x4				/** Function 4  */
/** Macro to select pin function 5- use with Chip_IOCON_PinMux */
#define FUNC5           0x5				/** Function 5  */
/** Macro to select pin function 6- use with Chip_IOCON_PinMux */
#define FUNC6           0x6				/** Function 6	*/
/** Macro to select pin function 7- use with Chip_IOCON_PinMux */
#define FUNC7           0x7				/** Function 7	*/

/**
 * @brief	Initialize the IOCON peripheral
 * @return	Nothing
 */
STATIC INLINE void Chip_IOCON_Init(void)
{}

/**
 * @brief	Setup pin modes and function
 * @param	port gpio	: port number
 * @param	pin			: gpio pin number
 * @param	mode		: ORed values from MD_ macros
 * @param	func		: pin function selector- a number from 0-7
 * @return	Nothing
 */
STATIC INLINE void Chip_IOCON_PinMux(uint8_t port, uint8_t pin, uint32_t mode, uint32_t func)
{
	LPC_IOCON->p[port][pin] = (mode | func);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __IOCON_17XX_40XX_H_ */
