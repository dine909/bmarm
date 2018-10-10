/*
 * @brief	LPC17xx/40xx System and Control driver
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

/* Determine PLL0 clock rate */
static uint32_t Chip_SYSCON_GetPLL0Rate(void)
{
	uint32_t clkhr = 0;

#if defined(CHIP_LPC175X_6X)
	/* Only valid if enabled */
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL0) & SYSCON_PLL0STS_ENABLED) {
		uint32_t msel, nsel;

		/* PLL0 input clock (FIN) is always SYSCLK */
		/* PLL0 rate is (FIN * 2 * MSEL) / NSEL, get MSEL and NSEL */
		msel = 1 + (LPC_SYSCON->PLL[SYSCON_PLL0].PLLCFG & 0x7FFF);
		nsel = 1 + ((LPC_SYSCON->PLL[SYSCON_PLL0].PLLCFG >> 16) & 0xFF);
		clkhr = (Chip_SYSCON_GetSYSCLKRate() * msel) / nsel;
	}
#else
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL0) & SYSCON_PLLSTS_ENABLED) {
		uint32_t msel;

		/* PLL0 input clock (FIN) is always SYSCLK */
		/* PLL0 rate is (FIN * MSEL) */
		msel = 1 + (LPC_SYSCON->PLL[SYSCON_PLL0].PLLCFG & 0x1F);
		clkhr = (Chip_SYSCON_GetSYSCLKRate() * msel);
	}
#endif

	return (uint32_t) clkhr;
}

/* Determine PLL1 clock rate */
static uint32_t Chip_SYSCON_GetPLL1Rate(void)
{
	uint32_t clkhr = 0;

#if defined(CHIP_LPC175X_6X)
	/* Only valid if enabled */
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL1) & SYSCON_PLL1STS_ENABLED) {
		uint32_t msel;

		/* PLL1 input clock (FIN) is always main oscillator */
		/* PLL1 rate is (FIN * MSEL) */
		msel = 1 + (LPC_SYSCON->PLL[SYSCON_PLL1].PLLCFG & 0x1F);
		clkhr = (Chip_SYSCON_GetMainOscRate() * msel);
	}
#else
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL1) & SYSCON_PLLSTS_ENABLED) {
		uint32_t msel;

		/* PLL1 input clock (FIN) is always main oscillator */
		/* PLL1 rate is (FIN * MSEL) */
		msel = 1 + (LPC_SYSCON->PLL[SYSCON_PLL1].PLLCFG & 0x1F);
		clkhr = (Chip_SYSCON_GetMainOscRate() * msel);
	}
#endif

	return (uint32_t) clkhr;
}

/* On 175x/6x devices, this is the input clock to the CPU divider.
   Additionally, on 177x/8x and 407x/8x devices, this is also the
   input clock to the peripehral divider. */
static uint32_t Chip_SYSCON_GetCPUPCLKInputClock(void)
{
	uint32_t clk;

#if defined(CHIP_LPC175X_6X)
	/* 175x/6x CPU clock source is determined by PLL0 connection status */
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL0) & SYSCON_PLL0STS_CONNECTED) {
		/* PLL0 rate */
		clk = Chip_SYSCON_GetPLL0Rate();
	}
	else {
		/* SYSCLK rate */
		clk = Chip_SYSCON_GetSYSCLKRate();
	}
#else
	/* 177x/8x and 407x/8x CPU/Periph clock source is determined by CCLKSEL
	   bit 8 */
	if (LPC_SYSCON->CCLKSEL & (1 << 8)) {
		/* PLL0 rate */
		clk = Chip_SYSCON_GetPLL0Rate();
	}
	else {
		/* SYSCLK rate */
		clk = Chip_SYSCON_GetSYSCLKRate();
	}
#endif

	return clk;
}

#if defined(CHIP_LPC175X_6X)
/* Maps a clock enable to a divider */
static const uint8_t clk2div[SYSCON_CLK_USB + 1] = {
	{(uint8_t) 0xFF},					/* SYSCON_CLK_RSVD1 */
	{(uint8_t) SYSCON_CLKDIV_TIMER0},	/* SYSCON_CLK_TIMER0 */
	{(uint8_t) SYSCON_CLKDIV_TIMER1},	/* SYSCON_CLK_TIMER1 */
	{(uint8_t) SYSCON_CLKDIV_UART0},	/* SYSCON_CLK_UART0 */
	{(uint8_t) SYSCON_CLKDIV_UART1},	/* SYSCON_CLK_UART1 */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_RSVD2 */
	{(uint8_t) SYSCON_CLKDIV_PWM1},		/* SYSCON_CLK_PWM1 */
	{(uint8_t) SYSCON_CLKDIV_I2C0},		/* SYSCON_CLK_I2C0 */
	{(uint8_t) SYSCON_CLKDIV_SPI},		/* SYSCON_CLK_SPI */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_RTC */
	{(uint8_t) SYSCON_CLKDIV_SSP1},		/* SYSCON_CLK_SSP1 */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_RSVD3 */
	{(uint8_t) SYSCON_CLKDIV_ADC},		/* SYSCON_CLK_ADC */
	{(uint8_t) SYSCON_CLKDIV_CAN1},		/* SYSCON_CLK_CAN1 */
	{(uint8_t) SYSCON_CLKDIV_CAN2},		/* SYSCON_CLK_CAN2 */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_GPIO */
	{(uint8_t) SYSCON_CLKDIV_RIT},		/* SYSCON_CLK_PCRIT */
	{(uint8_t) SYSCON_CLKDIV_MCPWM},	/* SYSCON_CLK_MCPWM */
	{(uint8_t) SYSCON_CLKDIV_RSVD1},	/* SYSCON_CLK_QEI */
	{(uint8_t) SYSCON_CLKDIV_I2C1},		/* SYSCON_CLK_I2C1 */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_RSVD4 */
	{(uint8_t) SYSCON_CLKDIV_SSP0},		/* SYSCON_CLK_SSP0 */
	{(uint8_t) SYSCON_CLKDIV_TIMER2},	/* SYSCON_CLK_TIMER2 */
	{(uint8_t) SYSCON_CLKDIV_TIMER3},	/* SYSCON_CLK_TIMER3 */
	{(uint8_t) SYSCON_CLKDIV_UART2},	/* SYSCON_CLK_UART2 */
	{(uint8_t) SYSCON_CLKDIV_UART3},	/* SYSCON_CLK_UART3 */
	{(uint8_t) SYSCON_CLKDIV_I2C2},		/* SYSCON_CLK_I2C2 */
	{(uint8_t) SYSCON_CLKDIV_I2S},		/* SYSCON_CLK_I2S */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_RSVD5 */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_GPDMA */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_ENET */
	{(uint8_t) 0xFF},					/* SYSCON_CLK_USB */
	{(uint8_t) 0xFF}					/* SYSCON_CLK_RSVD6 */
};

/* Maps a clock to a clock divider */
uint32_t Chip_SYSCON_CLK2CLKDIV(SYSCON_CLK_T clk)
{
	/* handle special clocks? */
	if (clk2div[clk] == 0xFF) {
		switch (clk) {
		case SYSCON_CLK_RTC:
			clkrate = Chip_SYSCON_GetRTCOscRate();
			break;

		case SYSCON_CLK_GPIO:
		case SYSCON_CLK_GPDMA:
		case SYSCON_CLK_ENET:
			clkrate = Chip_SYSCON_GetCPUPCLKInputClock();
			break;

		case SYSCON_CLK_USB:
			clkrate = Chip_SYSCON_GetUSBRate();
			break;

		default:
			clkrate = 0;
			break;
		}
	}
	else {
		clkrate = Chip_SYSCON_GetCPUPCLKInputClock() /
				  Chip_SYSCON_GetPCLKDiv(clk2div[clk]);
	}

	return clkrate;
}

#endif

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Returns the current SYSCLK clock rate */
uint32_t Chip_SYSCON_GetSYSCLKRate(void)
{
	uint32_t clk;

	/* Determine clock input rate to SYSCLK based on input selection */
	switch (Chip_SYSCON_GetSYSCLKSrc()) {
	case (uint32_t) SYSCON_PLLSRC_IRC:
	default:
		clk = SYSCON_IRC_FREQ;
		break;

	case (uint32_t) SYSCON_PLLSRC_MAINOSC:
		clk = CRYSTAL_MAIN_FREQ_IN;
		break;

#if defined(CHIP_LPC175X_6X)
	case (uint32_t) SYSCON_PLLSRC_RTC:
		clk = CRYSTAL_32K_FREQ_IN;
		break;
#endif
	}

	return clk;
}

/* Sets the current CPU clock source */
void Chip_SYSCON_SetCPUCLKSource(CLK_CPUCLK_T src)
{
#if defined(CHIP_LPC175X_6X)
	/* LPC175x/6x CPU clock source is based on PLL connect status */
	if (src == SYSCON_CPUCLK_PLL0) {
		/* Connect PLL0 */
		Chip_SYSCON_PLLEnable(SYSCON_PLL0, SYSCON_PLL_CONNECT);
	}
	else {
		Chip_SYSCON_PLLDisable(SYSCON_PLL0, SYSCON_PLL_CONNECT);
	}
#else
	/* LPC177x/8x and 407x/8x CPU clock source is based on CCLKSEL */
	if (src == SYSCON_CPUCLK_PLL0) {
		/* Connect PLL0 */
		LPC_SYSCON->CCLKSEL |= (1 << 8);
	}
	else {
		LPC_SYSCON->CCLKSEL &= ~(1 << 8);
	}
#endif
}

/* Returns the current CPU clock source */
CLK_CPUCLK_T Chip_SYSCON_GetCPUCLKSource(void)
{
	CLK_CPUCLK_T src;
#if defined(CHIP_LPC175X_6X)
	/* LPC175x/6x CPU clock source is based on PLL connect status */
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL0) & SYSCON_PLL0STS_CONNECTED) {
		src = SYSCON_CPUCLK_PLL0;
	}
	else {
		src = SYSCON_CPUCLK_SYSCLK;
	}
#else
	/* LPC177x/8x and 407x/8x CPU clock source is based on CCLKSEL */
	if (LPC_SYSCON->CCLKSEL & (1 << 8)) {
		src = SYSCON_CPUCLK_PLL0;
	}
	else {
		src = SYSCON_CPUCLK_SYSCLK;
	}
#endif

	return src;
}

/* Selects the CPU clock divider */
void Chip_SYSCON_SetCPUClkDiv(uint32_t cpuClkDiv)
{
#if defined(CHIP_LPC175X_6X)
	LPC_SYSCON->CCLKSEL = (cpuClkDiv - 1);
#else
	uint32_t temp;

	/* Save state of CPU clock source bit */
	temp = LPC_SYSCON->CCLKSEL & (1 << 8);
	LPC_SYSCON->CCLKSEL = temp | cpuClkDiv;
#endif
}

/* Gets the CPU clock divider */
uint32_t Chip_SYSCON_GetCPUClkDiv(void)
{
#if defined(CHIP_LPC175X_6X)
	return (LPC_SYSCON->CCLKSEL & 0xFF) + 1;
#else
	return LPC_SYSCON->CCLKSEL & 0x1F;
#endif
}

/* Returns the current CPU clock rate rate */
uint32_t Chip_SYSCON_GetCPUCLKRate(void)
{
	return Chip_SYSCON_GetCPUPCLKInputClock() / Chip_SYSCON_GetCPUClkDiv();
}

#if defined(CHIP_LPC175X_6X)
/* Selects a clock divider for a peripheral */
void Chip_SYSCON_SetPCLKDiv(SYSCON_CLKDIV_T clk, SYSCON_CLK_DIV_T pclkDiv)
{
	uint32_t temp, bitIndex, regIndex = (uint32_t) clk;

	/* Get register array index and clock index into the register */
	bitIndex = ((regIndex % 16) * 2);
	regIndex = regIndex / 16;

	/* Mask and update register */
	temp = LPC_SYSCON->PCLKSEL[regIndex] & ~(0x3 << bitIndex);
	temp |= (((uint32_t) pclkDiv) << bitIndex);
	LPC_SYSCON->PCLKSEL[regIndex] = temp;
}

/* Gets a clock divider for a peripheral */
uint32_t Chip_SYSCON_GetPLCKDiv(SYSCON_CLK_T clk)
{
	uint32_t div = 1, bitIndex, regIndex = (uint32_t) clk;

	/* Get register array index and clock index into the register */
	bitIndex = ((((uint32_t) regIndex) % 16) * 2);
	regIndex = regIndex / 16;

	/* Mask and update register */
	div = LPC_SYSCON->PCLKSEL[regIndex];
	div = (div >> bitIndex) & 0x3;
	if (div == 0) {
		div = 1;
	}
	else if (div == 1) {
		div = 2;
	}
	else if (div == 2) {
		div = 4;
	}
	else {
		/* Special case for CAN clock divider */
		if ((clk == SYSCON_CLK_CAN1) || (clk == SYSCON_CLK_CAN2)) {
			div = 6;
		}
		else {
			div = 8;
		}
	}

	return div;
}

/* Returns the clock rate for a peripheral */
uint32_t Chip_SYSCON_GetPCLKRate(SYSCON_CLKDIV_T clk) {
	uint32_t div, clkrate = 0;

	/* 175x/6x clock is derived from CPU clock with CPU divider */
	return Chip_SYSCON_GetCPUCLKRate() / Chip_SYSCON_GetPLCKDiv(clk);
}

#else
/* Returns the clock rate for all peripherals */
uint32_t Chip_SYSCON_GetPCLKRate(void)
{
	uint32_t clkrate;

	/* Get divider, a divider of 0 means the clock is disabled */
	clkrate = Chip_SYSCON_GetPCLKDiv();
	if (clkrate != 0) {
		/* Derived from periperhal clock input and peripheral clock divider */
		clkrate = Chip_SYSCON_GetCPUPCLKInputClock() / clkrate;
	}

	return clkrate;
}

#endif

#if !defined(CHIP_LPC175X_6X)
/* Selects the USB clock divider source */
void Chip_SYSCON_SetUSBClkDivSrc(SYSCON_USBDIVSRC_T usbClkDivSrc)
{
	uint32_t temp;

	/* Mask out current source, but keep divider */
	temp = LPC_SYSCON->USBCLKSEL & ~(0x3 << 8);
	LPC_SYSCON->USBCLKSEL = temp | (((uint32_t) usbClkDivSrc) << 8);
}

#endif

/* Sets the USB clock divider */
void Chip_SYSCON_SetUSBClkDiv(uint32_t usbClkDiv)
{
	uint32_t temp;

	/* Mask out current divider */
#if defined(CHIP_LPC175X_6X)
	temp = LPC_SYSCON->USBCLKSEL & ~(0xF);
#else
	temp = LPC_SYSCON->USBCLKSEL & ~(0x1F);
#endif
	LPC_SYSCON->USBCLKSEL = temp | usbClkDiv;
}

/* Gets the USB clock divider */
uint32_t Chip_SYSCON_GetUSBClkDiv(void)
{
#if defined(CHIP_LPC175X_6X)
	return (LPC_SYSCON->USBCLKSEL & 0xF) + 1;
#else
	return (LPC_SYSCON->USBCLKSEL & 0x1F) + 1;
#endif
}

/* Returns the USB clock (USB_CLK) rate */
uint32_t Chip_SYSCON_GetUSBRate(void)
{
	uint32_t div, clkrate;
#if defined(CHIP_LPC175X_6X)
	/* The USB clock rate is derived from PLL1 or PLL0 */
	if (Chip_SYSCON_PLLStatus(SYSCON_PLL1) & SYSCON_PLL1STS_CONNECTED) {
		/* Use PLL1 clock for USB source with divider of 1 */
		clkrate = Chip_SYSCON_GetPLL1Rate();
		div = 1;
	}
	else {
		clkrate = Chip_SYSCON_GetCPUPCLKInputClock();
		div = Chip_SYSCON_GetUSBClkDiv();
	}

#else
	/* Get clock from source drving USB */
	switch (Chip_SYSCON_GetUSBClkDivSrc()) {
	case SYSCON_USDIVSRC_SYSCLK:
	default:
		clkrate = Chip_SYSCON_GetSYSCLKRate();
		break;

	case SYSCON_USDIVSRC_PLL0:
		clkrate = Chip_SYSCON_GetPLL0Rate();
		break;

	case SYSCON_USDIVSRC_PLL1:
		clkrate = Chip_SYSCON_GetPLL1Rate();
		break;
	}

	div = Chip_SYSCON_GetUSBClkDiv();
#endif

	return clkrate / div;
}

#if !defined(CHIP_LPC175X_6X)
/* Selects the SPIFI clock divider source */
void Chip_SYSCON_SetSPIFIClkDivSrc(SYSCON_SPIFISRC_T usbClkDivSrc)
{
	uint32_t temp;

	/* Mask out current source, but keep divider */
	temp = LPC_SYSCON->SPIFICLKSEL & ~(0x3 << 8);
	LPC_SYSCON->SPIFICLKSEL = temp | (((uint32_t) usbClkDivSrc) << 8);
}

/* Sets the SPIFI clock divider */
void Chip_SYSCON_SetSPIFIClkDiv(uint32_t spifiClkDiv)
{
	uint32_t temp;

	/* Mask out current divider */
	temp = LPC_SYSCON->SPIFICLKSEL & ~(0x1F);
	LPC_SYSCON->SPIFICLKSEL = temp | (spifiClkDiv - 1);
}

/* Gets the SPIFI clock divider */
uint32_t Chip_SYSCON_GetSPIFIClkDiv(void)
{
	return (LPC_SYSCON->SPIFICLKSEL & 0x1F) + 1;
}

/* Returns the SPIFI clock rate */
uint32_t Chip_SYSCON_GetSPIFIRate(void)
{
	uint32_t div, clkrate;

	/* Get clock from source drving USB */
	switch (Chip_SYSCON_GetSPIFIClkDivSrc()) {
	case SYSCON_SPIFISRC_SYSCLK:
	default:
		clkrate = Chip_SYSCON_GetSYSCLKRate();
		break;

	case SYSCON_SPIFISRC_PLL0:
		clkrate = Chip_SYSCON_GetPLL0Rate();
		break;

	case SYSCON_SPIFISRC_PLL1:
		clkrate = Chip_SYSCON_GetPLL1Rate();
		break;
	}

	div = Chip_SYSCON_GetSPIFIClkDiv();

	return clkrate / div;
}

#endif

/* Enables or connects a PLL */
void Chip_SYSCON_PLLEnable(CLK_PLL_T pllNum, uint32_t flags) {
	uint32_t temp;

	temp = LPC_SYSCON->PLL[pllNum].PLLCON &
#if defined(CHIP_LPC175X_6X)
		   (SYSCON_PLL_ENABLE | SYSCON_PLL_CONNECT);
#else
		   (SYSCON_PLL_ENABLE);
#endif
	temp |= flags;
	LPC_SYSCON->PLL[pllNum].PLLCON = temp;
	Chip_SYSCON_PLLFeed(pllNum);
}

/* Disables or disconnects a PLL */
void Chip_SYSCON_PLLDisable(CLK_PLL_T pllNum, uint32_t flags) {
	uint32_t temp;

	temp = LPC_SYSCON->PLL[pllNum].PLLCON &
#if defined(CHIP_LPC175X_6X)
		   (SYSCON_PLL_ENABLE | SYSCON_PLL_CONNECT);
#else
		   (SYSCON_PLL_ENABLE);
#endif
	temp &= ~flags;
	LPC_SYSCON->PLL[pllNum].PLLCON = temp;
	Chip_SYSCON_PLLFeed(pllNum);
}

/* Sets up a PLL */
void Chip_SYSCON_PLLSetup(CLK_PLL_T pllNum, SYSCON_PLLSETUP_T *pllSetup) {
	uint32_t pllcfg;

#if defined(CHIP_LPC175X_6X)
	/* PLL0 and PLL1 are slightly different */
	if (pllNum == SYSCON_PLL0) {
		pllcfg = (pllSetup->msel) | (pllSetup->nsel_psel << 16);
	}
	else {
		pllcfg = (pllSetup->msel) | (pllSetup->nsel_psel << 5);
	}

#else
	pllcfg = (pllSetup->msel) | (pllSetup->nsel_psel << 5);
#endif

	LPC_SYSCON->PLL[pllNum].PLLCFG = pllcfg;
}

/* Returns the PLL current clock rate */
uint32_t Chip_SYSCON_GetPLLRate(CLK_PLL_T pllNum) {
	uint32_t clk;

	if (pllNum == SYSCON_PLL0) {
		clk = Chip_SYSCON_GetPLL0Rate();
	}
	else {
		clk = Chip_SYSCON_GetPLL1Rate();
	}

	return clk;
}

/* Returns and clears the current sleep mode entry flags */
uint32_t Chip_SYSCON_GetClrSleepFlags(uint32_t flags) {
	uint32_t savedFlags = LPC_SYSCON->PCON;

	LPC_SYSCON->PCON = flags;

	return savedFlags & (SYSCON_PD_SMFLAG | SYSCON_PD_DSFLAG |
						 SYSCON_PD_PDFLAG | SYSCON_PD_DPDFLAG);
}

/* Selects a source clock and divider rate for the CLKOUT pin */
void Chip_SYSCON_CLKOUTSetup(SYSCON_CLKOUT_SRC_T clk,
							 uint32_t clkDiv)
{
	uint32_t temp;

	temp = LPC_SYSCON->CLKOUTCFG & ~0x1FF;
	temp |= ((uint32_t) clk) | (clkDiv - 1);
	LPC_SYSCON->CLKOUTCFG = temp;
}

/* Enables power and clocking for a peripheral */
void Chip_SYSCON_PCLKEnable(SYSCON_CLK_T clk) {
	uint32_t bs = (uint32_t) clk;

#if defined(CHIP_LPC407X_8X)
	if (bs >= 32) {
		LPC_SYSCON->PCONP1 |= (1 << (bs - 32));
	}
	else {
		LPC_SYSCON->PCONP |= (1 << bs);
	}
#else
	LPC_SYSCON->PCONP |= (1 << bs);
#endif
}

/* Disables power and clocking for a peripheral */
void Chip_SYSCON_PCLKDisable(SYSCON_CLK_T clk) {
	uint32_t bs = (uint32_t) clk;

#if defined(CHIP_LPC407X_8X)
	if (bs >= 32) {
		LPC_SYSCON->PCONP1 &= ~(1 << (bs - 32));
	}
	else {
		LPC_SYSCON->PCONP |= ~(1 << bs);
	}
#else
	LPC_SYSCON->PCONP |= ~(1 << bs);
#endif
}

/* Returns power enables state for a peripheral */
bool Chip_SYSCON_PCLKIsEnabled(SYSCON_CLK_T clk)
{
	uint32_t bs = (uint32_t) clk;

#if defined(CHIP_LPC407X_8X)
	if (bs >= 32) {
		bs = LPC_SYSCON->PCONP1 & (1 << (bs - 32));
	}
	else {
		bs = LPC_SYSCON->PCONP & (1 << bs);
	}
#else
	bs = LPC_SYSCON->PCONP & (1 << bs);
#endif

	return (bool) (bs != 0);
}

/* Returns clock rate for a peripheral */
uint32_t Chip_SYSCON_GetPCLKRateFromClk(SYSCON_CLK_T clk)
{
	uint32_t clkrate;

	/* Is peripheral clock enabled? */
	if (Chip_SYSCON_PCLKIsEnabled(clk)) {
#if defined(CHIP_LPC175X_6X)
		/* 175x/6x rates are derived from the CPU clock rate, but also the
		   peripheral divider */
		clkrate = Chip_SYSCON_CLK2CLKDIV(clk);

#else
		/* Peripheral clock rate */
		clkrate = Chip_SYSCON_GetPCLKRate();
#endif
	}
	else {
		clkrate = 0;
	}

	return clkrate;
}

/* Resets a peripheral */
void Chip_SYSCON_PeriphReset(SYSCON_RESET_T periph)
{
	uint32_t bitIndex, regIndex = (uint32_t) periph;

	/* Get register array index and clock index into the register */
	bitIndex = (regIndex % 32);
	regIndex = regIndex / 32;

	/* Reset peripheral */
	LPC_SYSCON->RSTCON[regIndex] = (1 << bitIndex);
	LPC_SYSCON->RSTCON[regIndex] &= ~(1 << bitIndex);
}
