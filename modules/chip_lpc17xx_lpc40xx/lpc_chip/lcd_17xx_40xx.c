/*
 * @brief LPC17xx/40xx LCD chip driver
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

static LCD_CURSOR_SIZE_OPT LCD_Cursor_Size = LCD_CURSOR_64x64;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize the LCD controller */
void Chip_LCD_Init(LCD_Config_Type *LCD_ConfigStruct)
{
	Chip_SYSCON_PCLKEnable(SYSCON_CLK_LCD);
	IP_LCD_Init(LPC_LCD, LCD_ConfigStruct);
}

/* Shutdown the LCD controller */
void Chip_LCD_DeInit(void)
{
	Chip_SYSCON_PCLKDisable(SYSCON_CLK_LCD);
}

/* Configure Cursor */
void Chip_LCD_Cursor_Config(LCD_CURSOR_SIZE_OPT cursor_size, bool sync)
{
	LCD_Cursor_Size = cursor_size;
	IP_LCD_Cursor_Config(LPC_LCD, cursor_size, sync);
}

/* Write Cursor Image into Internal Cursor Image Buffer */
void Chip_LCD_Cursor_WriteImage(uint8_t cursor_num, void *Image)
{
	int i, j;
	uint32_t *fifoptr, *crsr_ptr = (uint32_t *) Image;

	/* Check if Cursor Size was configured as 32x32 or 64x64*/
	if (LCD_Cursor_Size == LCD_CURSOR_32x32) {
		i = cursor_num * 64;
		j = i + 64;
	}
	else {
		i = 0;
		j = 256;
	}
	fifoptr = IP_LCD_Cursor_GetImageBufferAddress(LPC_LCD, 0);

	/* Copy Cursor Image content to FIFO */
	for (; i < j; i++) {

		*fifoptr = *crsr_ptr;
		crsr_ptr++;
		fifoptr++;
	}
}

/* Load LCD Palette */
void Chip_LCD_LoadPalette(void *palette) {
	LCD_PALETTE_ENTRY_Type pal_entry, *ptr_pal_entry;
	uint8_t i, *pal_ptr;

	ptr_pal_entry = &pal_entry;
	pal_ptr = (uint8_t *) palette;

	/* 256 entry in the palette table */
	for (i = 0; i < 256 / 2; i++) {
		pal_entry.Bl = (*pal_ptr++) >> 3;	/* blue first */
		pal_entry.Gl = (*pal_ptr++) >> 3;	/* get green */
		pal_entry.Rl = (*pal_ptr++) >> 3;	/* get red */
		pal_ptr++;	/* skip over the unused byte */
		/* do the most significant halfword of the palette */
		pal_entry.Bu = (*pal_ptr++) >> 3;	/* blue first */
		pal_entry.Gu = (*pal_ptr++) >> 3;	/* get green */
		pal_entry.Ru = (*pal_ptr++) >> 3;	/* get red */
		pal_ptr++;	/* skip over the unused byte */

		IP_LCD_Color_LoadPalette(LPC_LCD, (uint32_t *) &ptr_pal_entry, i);
	}
}
