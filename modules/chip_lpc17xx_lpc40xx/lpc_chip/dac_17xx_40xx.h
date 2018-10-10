/*
 * @brief LPC17xx/40xx D/A conversion driver
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

#ifndef __DAC_17XX_40XX_H_
#define __DAC_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup DAC_17XX_40XX CHIP: LPC17xx/40xx D/A conversion driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/**
 * @brief	Initial DAC configuration
 *              - Maximum	current is 700 uA
 *              - Value to AOUT is 0
 * @return	Nothing
 */
void Chip_DAC_Init(void);

/**
 * @brief	Shutdown DAC
 * @return	Nothing
 */
STATIC INLINE void Chip_DAC_DeInit(void) {}

/**
 * @brief	Update value to DAC buffer
 * @param	dac_value	: value 10 bit to be converted to output
 * @return	Nothing
 */
STATIC INLINE void Chip_DAC_UpdateValue(uint32_t dac_value)
{
	IP_DAC_UpdateValue(LPC_DAC, dac_value);
}

/**
 * @brief	Set maximum update rate for DAC
 * @param	bias	: Using Bias value, should be:
 *              - 0 is 1MHz
 *              - 1 is 400kHz
 * @return	Nothing
 */
STATIC INLINE void Chip_DAC_SetBias(uint32_t bias)
{
	IP_DAC_SetBias(LPC_DAC, bias);
}

/**
 * @brief	Enables the DMA operation and controls DMA timer
 * @param	DAC_ConverterConfigStruct : pointer to DAC_CONVERTER_CFG_Type
 *                  - DBLBUF_ENA :enable/disable DACR double buffering feature
 *                  - CNT_ENA    :enable/disable timer out counter
 *                  - DMA_ENA    :enable/disable DMA access
 * @return	Nothing
 */
STATIC INLINE void Chip_DAC_ConfigDAConverterControl(DAC_CONVERTER_CFG_Type *DAC_ConverterConfigStruct)
{
	IP_DAC_ConfigDAConverterControl(LPC_DAC, DAC_ConverterConfigStruct);
}

/**
 * @brief	Set reload value for interrupt/DMA counter
 * @param	time_out	: time out to reload for interrupt/DMA counter
 * @return	Nothing
 */
STATIC INLINE void Chip_DAC_SetDMATimeOut(uint32_t time_out)
{
	IP_DAC_SetDMATimeOut(LPC_DAC, time_out);
}

/**
 * @brief	Get status for interrupt/DMA time out
 * @return	interrupt/DMA time out status, should be SET or RESET
 */
STATIC INLINE IntStatus Chip_DAC_GetIntStatus(void)
{
	return IP_DAC_GetIntStatus(LPC_DAC);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __DAC_17XX_40XX_H_ */
