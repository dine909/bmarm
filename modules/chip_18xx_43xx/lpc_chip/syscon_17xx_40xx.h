/*
 * @brief	LPC17xx/40xx System and Control driver
 *        Note this peripheral is called the Clock and Power controller
 *        on the 175x/6x devices.
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

#ifndef _SYSCON_17XX_40XX_H_
#define _SYSCON_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CLOCK_17XX_40XX CHIP: LPC17xx/40xx Clock Driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/** @defgroup CLOCK_17XX_40XX_OPTIONS CHIP: LPC17xx/40xx Clock Driver driver options
 * @ingroup CHIP_17XX_40XX_DRIVER_OPTIONS CLOCK_17XX_40XX
 * The clock driver has options that configure it's operation at build-time.<br>
 *
 * CHIP_LPC175X_6X, CHIP_LPC177X_8X, CHIP_LPC407X_8X:<br>
 * One of these CHIP defines must be present to determine which device this driver
 * will be built for <br>
 *
 * CRYSTAL_MAIN_FREQ_IN:<br>
 * This define is the external crystal frequency used for the main oscillator.<br>
 *
 * CRYSTAL_32K_FREQ_IN:<br>
 * This define is rate of the RTC(32K) crystal frequency used for the chip.<br>
 *
 * For more information on driver options see<br>
 * @ref LPCOPEN_DESIGN_ARPPROACH<br>
 * @{
 */

/**
 * @}
 */

/**
 * @brief LPC17XX/40XX Clock and Power PLL register block structure
 */
typedef struct {
	__IO uint32_t PLLCON;					/*!< (R/W)  PLL Control Register */
	__IO uint32_t PLLCFG;					/*!< (R/W)  PLL Configuration Register */
	__I  uint32_t PLLSTAT;					/*!< (R/ )  PLL Status Register */
	__O  uint32_t PLLFEED;					/*!< ( /W)  PLL Feed Register */
	uint32_t RESERVED1[4];
} SYSCON_PLL_REGS_T;

/**
 * Selectable PLLs
 */
typedef enum {
	SYSCON_PLL0,
	SYSCON_PLL1
} CLK_PLL_T;

/**
 * @brief LPC17XX/40XX Clock and Power register block structure
 */
typedef struct {
	__IO uint32_t FLASHCFG;					/*!< Offset: 0x000 (R/W)  Flash Accelerator Configuration Register */
	uint32_t RESERVED0[15];
	__IO uint32_t MEMMAP;					/*!< Offset: 0x000 (R/W)  Flash Accelerator Configuration Register */
	uint32_t RESERVED1[15];
	SYSCON_PLL_REGS_T PLL[SYSCON_PLL1 + 1];		/*!< Offset: 0x080: PLL0 and PLL1 */
	__IO uint32_t PCON;						/*!< Offset: 0x0C0 (R/W)  Power Control Register */
	__IO uint32_t PCONP;					/*!< Offset: 0x0C4 (R/W)  Power Control for Peripherals Register */
#if defined(CHIP_LPC175X_6X)
	uint32_t RESERVED2[15];
#elif defined(CHIP_LPC177X_8X)
	uint32_t RESERVED2[14];
	__IO uint32_t EMCCLKSEL;				/*!< Offset: 0x100 (R/W)  External Memory Controller Clock Selection Register */
#else
	__IO uint32_t PCONP1;					/*!< Offset: 0x0C8 (R/W)  Power Control 1 for Peripherals Register */
	uint32_t RESERVED2[13];
	__IO uint32_t EMCCLKSEL;				/*!< Offset: 0x100 (R/W)  External Memory Controller Clock Selection Register */
#endif
	__IO uint32_t CCLKSEL;					/*!< Offset: 0x104 (R/W)  CPU Clock Selection Register */
	__IO uint32_t USBCLKSEL;				/*!< Offset: 0x108 (R/W)  USB Clock Selection Register */
	__IO uint32_t CLKSRCSEL;				/*!< Offset: 0x10C (R/W)  Clock Source Select Register */
	__IO uint32_t CANSLEEPCLR;				/*!< Offset: 0x110 (R/W)  CAN Sleep Clear Register */
	__IO uint32_t CANWAKEFLAGS;				/*!< Offset: 0x114 (R/W)  CAN Wake-up Flags Register */
	uint32_t RESERVED3[10];
	__IO uint32_t EXTINT;					/*!< Offset: 0x140 (R/W)  External Interrupt Flag Register */
	uint32_t RESERVED4;
	__IO uint32_t EXTMODE;					/*!< Offset: 0x148 (R/W)  External Interrupt Mode Register */
	__IO uint32_t EXTPOLAR;					/*!< Offset: 0x14C (R/W)  External Interrupt Polarity Register */
	uint32_t RESERVED5[12];
	__IO uint32_t RSID;						/*!< Offset: 0x180 (R/W)  Reset Source Identification Register */
#if defined(CHIP_LPC175X_6X)
	uint32_t RESERVED6[7];
#elif defined(CHIP_LPC177X_8X)
	uint32_t RESERVED6;
	uint32_t MATRIXARB;
	uint32_t RESERVED6A[5];
#else
	uint32_t RESERVED6;
	uint32_t MATRIXARB;
	uint32_t RESERVED6A[5];
#endif
	__IO uint32_t SCS;						/*!< Offset: 0x1A0 (R/W)  System Controls and Status Register */
	__IO uint32_t RESERVED7;
#if defined(CHIP_LPC175X_6X)
	__IO uint32_t PCLKSEL[2];				/*!< Offset: 0x1A8 (R/W)  Peripheral Clock Selection Register */
	uint32_t RESERVED8[5];
#else
	__IO uint32_t PCLKSEL;				/*!< Offset: 0x1A8 (R/W)  Peripheral Clock Selection Register */
	uint32_t RESERVED9;
	__IO uint32_t PBOOST;					/*!< Offset: 0x1B0 (R/W)  Power Boost control register */
	__IO uint32_t SPIFICLKSEL;
	__IO uint32_t LCD_CFG;					/*!< Offset: 0x1B8 (R/W)  LCD Configuration and clocking control Register */
	uint32_t RESERVED10;
#endif
	__IO uint32_t USBIntSt;					/*!< Offset: 0x1C0 (R/W)  USB Interrupt Status Register */
#if defined(CHIP_LPC175X_6X)
	__IO uint32_t reserved;				/*!< Offset: 0x1C4 (R/W)  DMA Request Select Register */
#else
	__IO uint32_t DMAREQSEL;				/*!< Offset: 0x1C4 (R/W)  DMA Request Select Register */
#endif
	__IO uint32_t CLKOUTCFG;				/*!< Offset: 0x1C8 (R/W)  Clock Output Configuration Register */
#if defined(CHIP_LPC175X_6X)
	uint32_t RESERVED11[6];
#else
	__IO uint32_t RSTCON[2];				/*!< Offset: 0x1CC (R/W)  RESET Control0/1 Registers */
	uint32_t RESERVED11[2];
	__IO uint32_t EMCDLYCTL;				/*!< Offset: 0x1DC (R/W) SDRAM programmable delays          */
	__IO uint32_t EMCCAL;					/*!< Offset: 0x1E0 (R/W) Calibration of programmable delays */
#endif
} LPC_SYSCON_T;

#define SYSCON_OSCRANGE_15_25 (1 << 4)	/* SCS register - main oscillator range 15 to 25MHz */
#define SYSCON_OSCEC          (1 << 5)	/* SCS register - main oscillator enable */
#define SYSCON_OSCSTAT        (1 << 6)	/* SCS register - main oscillator is ready status */

/* Internal oscillator frequency */
#define SYSCON_IRC_FREQ (4000000)

/**
 * @brief	Sets the FLASH accelerator access time
 * @param	accessClks: Number of access clocks for FLASH, between 1 and 6
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_SetFlashAccel(uint32_t accessClks)
{
	LPC_SYSCON->FLASHCFG = ((accessClks - 1) << 12);
}

/**
 * @brief	Returns the main oscillator clock rate
 * @return	main oscillator clock rate
 */
STATIC INLINE uint32_t Chip_SYSCON_GetMainOscRate(void)
{
	return CRYSTAL_MAIN_FREQ_IN;
}

/**
 * @brief	Returns the internal oscillator (IRC) clock rate
 * @return	internal oscillator (IRC) clock rate
 */
STATIC INLINE uint32_t Chip_SYSCON_GetIntOscRate(void)
{
	return SYSCON_IRC_FREQ;
}

/**
 * @brief	Returns the RTC oscillator clock rate
 * @return	RTC oscillator clock rate
 */
STATIC INLINE uint32_t Chip_SYSCON_GetRTCOscRate(void)
{
	return CRYSTAL_32K_FREQ_IN;
}

/**
 * PLL source clocks
 */
typedef enum {
	SYSCON_PLLSRC_IRC,			/* PLL is sourced from the internal oscillator (IRC) */
	SYSCON_PLLSRC_MAINOSC,	/* PLL is sourced from the main oscillator */
#if defined(CHIP_LPC175X_6X)
	SYSCON_PLLSRC_RTC,			/* PLL is sourced from the RTC oscillator */
#else
	SYSCON_PLLSRC_RESERVED1,
#endif
	SYSCON_PLLSRC_RESERVED2
} SYSCON_PLLSRCCLK_T;

/**
 * @brief	Selects a input clock source for SYSCLK
 * @param	clkSRC:	input clock source for SYSCLK
 * @return	Nothing
 * @note	SYSCLK is used for sourcing PLL0, SPIFI FLASH, the USB clock
 * divider, and the CPU clock divider.
 */
STATIC INLINE void Chip_SYSCON_SetSYSCLKSrc(SYSCON_PLLSRCCLK_T clkSRC)
{
	LPC_SYSCON->CLKSRCSEL = clkSRC;
}

/**
 * @brief	Returns the input clock source for SYSCLK
 * @return	input clock source for SYSCLK
 */
STATIC INLINE SYSCON_PLLSRCCLK_T Chip_SYSCON_GetSYSCLKSrc(void)
{
	return (SYSCON_PLLSRCCLK_T) (LPC_SYSCON->CLKSRCSEL & 0x3);
}

/**
 * @brief	Returns the current SYSCLK clock rate
 * @return	SYSCLK clock rate
 * @note	SYSCLK is used for sourcing PLL0, SPIFI FLASH, the USB clock
 * divider, and the CPU clock divider.
 */
uint32_t Chip_SYSCON_GetSYSCLKRate(void);

/**
 * Selectable CPU clock sources
 */
typedef enum {
	SYSCON_CPUCLK_SYSCLK,
	SYSCON_CPUCLK_PLL0
} CLK_CPUCLK_T;

/**
 * @brief	Sets the current CPU clock source
 * @return	Nothing
 * @note	When setting the clock source to the PLL, it should
 * be enabled and locked.
 */
void Chip_SYSCON_SetCPUCLKSource(CLK_CPUCLK_T src);

/**
 * @brief	Returns the current CPU clock source
 * @return	CPU clock source
 * @note	On 177x/8x and 407x/8x devices, this is also the peripheral
 * clock source.
 */
CLK_CPUCLK_T Chip_SYSCON_GetCPUCLKSource(void);

/**
 * @brief	Sets the CPU clock divider
 * @param	cpuClkDiv:	CPU clock divider, between 1 and divider max
 * @return	Nothing
 * @note	The maximum divider for the 175x/6x is 256. The maximum divider for
 * the 177x/8x and 407x/8x is 32. Note on 175x/6x devices, the divided CPU
 * clock rate is used as the input to the peripheral clock dividers,
 * while 177x/8x and 407x/8x devices use the undivided CPU clock rate.
 */
void Chip_SYSCON_SetCPUClkDiv(uint32_t cpuClkDiv);

/**
 * @brief	Gets the CPU clock divider
 * @return	CPU clock divider, between 1 and divider max
 * @note	The maximum divider for the 175x/6x is 256. The maximum divider for
 * the 177x/8x and 407x/8x is 32. Note on 175x/6x devices, the divided CPU
 * clock rate is used as the input to the peripheral clock dividers,
 * while 177x/8x and 407x/8x devices use the undivided CPU clock rate.
 */
uint32_t Chip_SYSCON_GetCPUClkDiv(void);

/**
 * @brief	Returns the current CPU clock rate
 * @return	CPU clock rate
 * @note	The CPU clock rate is determined from the CPU input clock
 * and the CPU divider.
 */
uint32_t Chip_SYSCON_GetCPUCLKRate(void);

#if !defined(CHIP_LPC175X_6X)
/**
 * EMC clock divider values
 */
typedef enum {
	SYSCON_EMC_DIV1,
	SYSCON_EMC_DIV2
} SYSCON_EMCDIV_T;

/**
 * @brief	Selects a EMC divider rate
 * @param	emcDiv:	Source clock for PLL
 * @return	Nothing
 * @note	This function controls division of the clock before it is used by the EMC.
 * The EMC uses the same base clock as the CPU and the APB peripherals. The
 * EMC clock can tun at half or the same as the CPU clock. This is intended to
 * be used primarily when the CPU is running faster than the external bus can
 * support.
 */
STATIC INLINE void Chip_SYSCON_SetEMCClkDiv(SYSCON_EMCDIV_T emcDiv)
{
	LPC_SYSCON->EMCCLKSEL = (uint32_t) emcDiv;
}

#endif

#if defined(CHIP_LPC175X_6X)
/**
 * Clock and power peripheral clock divider rates used with the
 * SYSCON_CLKDIVSEL_T clock types (devices only)
 */
typedef enum {
	SYSCON_CLK_DIV4,			/* Divider by 4 */
	SYSCON_CLK_DIV1,			/* Divider by 1 */
	SYSCON_CLK_DIV2,			/* Divider by 2 */
	SYSCON_CLK_DIV8,			/* Divider by 8, not for use with CAN */
	SYSCON_CLK_DIV6_CCAN = SYSCON_CLK_DIV8	/* Divider by 6, CAN only */
} SYSCON_CLK_DIV_T;

/**
 * Peripheral clock dividers for LPC175x/6x
 * This is a list of clocks that can be divided on fht 175x/6x
 */
typedef enum {
	SYSCON_CLKDIV_WDT,		/* Watchdog divider */
	SYSCON_CLKDIV_TIMER0,	/* Timer 0 divider */
	SYSCON_CLKDIV_TIMER1,	/* Timer 1 divider */
	SYSCON_CLKDIV_UART0,	/* UART 0 divider */
	SYSCON_CLKDIV_UART1,	/* UART 1 divider */
	SYSCON_CLKDIV_RSVD1,
	SYSCON_CLKDIV_PWM1,		/* PWM 1 divider */
	SYSCON_CLKDIV_I2C0,		/* I2C 0 divider */
	SYSCON_CLKDIV_SPI,		/* SPI divider */
	SYSCON_CLKDIV_RSVD2,
	SYSCON_CLKDIV_SSP1,		/* SSP 1 divider */
	SYSCON_CLKDIV_DAC,		/* DAC divider */
	SYSCON_CLKDIV_ADC,		/* ADC divider */
	SYSCON_CLKDIV_CAN1,		/* CAN 1 divider */
	SYSCON_CLKDIV_CAN2,		/* CAN 2 divider */
	SYSCON_CLKDIV_ACF,		/* ACF divider */
	SYSCON_CLKDIV_QEI,		/* QEI divider */
	SYSCON_CLKDIV_GPIOINT,	/* GPIOINT divider */
	SYSCON_CLKDIV_PCB,		/* PCB divider */
	SYSCON_CLKDIV_I2C1,		/* I2C 1 divider */
	SYSCON_CLKDIV_RSVD3,
	SYSCON_CLKDIV_SSP0,		/* SSP 0 divider */
	SYSCON_CLKDIV_TIMER2,	/* Timer 2 divider */
	SYSCON_CLKDIV_TIMER3,	/* Timer 3 divider */
	SYSCON_CLKDIV_UART2,	/* UART 2 divider */
	SYSCON_CLKDIV_UART3,	/* UART 3 divider */
	SYSCON_CLKDIV_I2C2,		/* I2C 2 divider */
	SYSCON_CLKDIV_I2S,		/* I2S divider */
	SYSCON_CLKDIV_RSVD4,
	SYSCON_CLKDIV_RIT,		/* Repetitive timer divider */
	SYSCON_CLKDIV_SYSCON,	/* SYSCON divider */
	SYSCON_CLKDIV_MCPWM		/* Motor control PWM divider */
} SYSCON_CLKDIV_T;

/**
 * @brief	Selects a clock divider for a peripheral
 * @param	clk:		Clock to set divider for
 * @param	pclkDiv:	Divider for the clock
 * @return	Nothing
 * @note	Selects the divider for a peripheral. A peripheral clock is generated
 * from the CPU clock divided by its peripheral clock divider.
 * Only peripheral clocks that are defined in the PCLKSEL registers of
 * the clock and power controller can used this function.
 * (LPC175X/6X only)
 */
void Chip_SYSCON_SetPCLKDiv(SYSCON_CLKDIV_T clk, SYSCON_CLK_DIV_T pclkDiv);

/**
 * @brief	Gets a clock divider for a peripheral
 * @param	clk:		Clock to set divider for
 * @return	The divider for the clock
 * @note	Selects the divider for a peripheral. A peripheral clock is generated
 * from the CPU clock divided by its peripheral clock divider.
 * Only peripheral clocks that are defined in the PCLKSEL registers of
 * the clock and power controller can used this function.
 * (LPC175X/6X only)
 */
uint32_t Chip_SYSCON_GetPCLKDiv(SYSCON_CLKDIV_T clk);

/**
 * @brief	Returns the clock rate for a peripheral (from divider clock)
 * @param	clk:	Clock to get rate for
 * @return	peripheral clock rate
 * @note	If a peripheral clock is disabled, the returned rate will be 0.
 * The peripherals divider value will be used to determine the correct
 * clock rate. The WDT, DAC, and a few other clocks have rates that
 * are not determine by this function. (LPC175X/6X only)
 */
uint32_t Chip_SYSCON_GetPCLKRate(SYSCON_CLKDIV_T clk);

#else
/**
 * @brief	Sets a clock divider for all peripherals
 * @param	pclkDiv:	Divider for all peripherals, 0 = disable
 * @return	Nothing
 * @note	All the peripherals in the device use the same clock divider. The
 * divider is based on the CPU's clock rate. Use 0 to disable all
 * peripheral clocks or a divider of 1 to 15. (LPC177X/8X and 407X/8X)
 */
STATIC INLINE void Chip_SYSCON_SetPCLKDiv(uint32_t pclkDiv)
{
	LPC_SYSCON->PCLKSEL = pclkDiv;
}

/**
 * @brief	Gets the clock divider for all peripherals
 * @return	Divider for all peripherals, 0 = disabled
 * @note	All the peripherals in the device use the same clock divider. The
 * divider is based on the CPU's clock rate. (LPC177X/8X and 407X/8X)
 */
STATIC INLINE uint32_t Chip_SYSCON_GetPCLKDiv(void)
{
	return LPC_SYSCON->PCLKSEL & 0x1F;
}

/**
 * @brief	Returns the clock rate for all peripherals
 * @return	peripheral clock rate
 * @note	All the peripherals in the device use the same clock rate.
 * (LPC177X/8X and 407X/8X)
 */
uint32_t Chip_SYSCON_GetPCLKRate(void);

#endif

#if !defined(CHIP_LPC175X_6X)
/**
 * Clock sources for the USB divider. On 175x/6x devices, only the USB
 * PLL1 can be used as an input for the USB divider
 */
typedef enum {
	SYSCON_USDIVSRC_SYSCLK,		/* SYSCLK clock as USB divider source */
	SYSCON_USDIVSRC_PLL0,		/* PLL0 clock as USB divider source */
	SYSCON_USDIVSRC_PLL1,		/* PLL1 clock as USB divider source */
	SYSCON_USDIVSRC_RESERVED
} SYSCON_USBDIVSRC_T;

/**
 * @brief	Sets the USB clock divider source
 * @param	usbClkDivSrc:	USB clock divider source clock
 * @return	Nothing
 * @note	This function doesn't apply for LPC175x/6x devices. The divider must be
 * be selected with the selected source to give a valid USB clock with a
 * rate of 48MHz.
 */
void Chip_SYSCON_SetUSBClkDivSrc(SYSCON_USBDIVSRC_T usbClkDivSrc);

/**
 * @brief	Gets the USB clock divider source
 * @return	USB clock divider source clock
 */
STATIC INLINE SYSCON_USBDIVSRC_T Chip_SYSCON_GetUSBClkDivSrc(void)
{
	return (SYSCON_USBDIVSRC_T) ((LPC_SYSCON->USBCLKSEL >> 8) & 0x3);
}

#endif

/**
 * @brief	Sets the USB clock divider
 * @param	usbClkDiv:	USB clock divider to generate 48MHz from USB source clock
 * @return	Nothing
 * @note	Divider values are between 1 and 32 (16 max for 175x/6x)
 */
void Chip_SYSCON_SetUSBClkDiv(uint32_t usbClkDiv);

/**
 * @brief	Gets the USB clock divider
 * @return	USB clock divider
 * @note	Divider values are between 1 and 32 (16 max for 175x/6x)
 */
uint32_t Chip_SYSCON_GetUSBClkDiv(void);

/**
 * @brief	Gets the USB clock (USB_CLK) rate
 * @return	USB clock (USB_CLK) clock rate
 * @note	The clock source and divider are used to generate the USB clock rate.
 */
uint32_t Chip_SYSCON_GetUSBRate(void);

#if !defined(CHIP_LPC175X_6X)
/**
 * Clock sources for the SPIFI clock divider
 */
typedef enum {
	SYSCON_SPIFISRC_SYSCLK,		/* SYSCLK clock as SPIFI divider source */
	SYSCON_SPIFISRC_PLL0,		/* PLL0 clock as SPIFI divider source */
	SYSCON_SPIFISRC_PLL1,		/* PLL1 clock as SPIFI divider source */
	SYSCON_SPIFISRC_RESERVED
} SYSCON_SPIFISRC_T;

/**
 * @brief	Sets the SPIFI clock divider source
 * @param	spifiClkDivSrc:	SPIFI clock divider source clock
 * @return	Nothing
 */
void Chip_SYSCON_SetSPIFIClkDivSrc(SYSCON_SPIFISRC_T spifiClkDivSrc);

/**
 * @brief	Gets the SPIFI clock divider source
 * @return	SPIFI clock divider source clock
 */
STATIC INLINE SYSCON_SPIFISRC_T Chip_SYSCON_GetSPIFIClkDivSrc(void)
{
	return (SYSCON_SPIFISRC_T) ((LPC_SYSCON->SPIFICLKSEL >> 8) & 0x3);
}

/**
 * @brief	Sets the SPIFI clock divider
 * @param	spifiClkDiv:	SPIFI clock divider, 0 to disable
 * @return	Nothing
 * @note	Divider values are between 1 and 32 (16 max for 175x/6x)
 */
void Chip_SYSCON_SetSPIFIClkDiv(uint32_t spifiClkDiv);

/**
 * @brief	Gets the SPIFI clock divider
 * @return	SPIFI clock divider
 * @note	Divider values are between 1 and 31, 0 is disabled
 */
uint32_t Chip_SYSCON_GetSPIFIClkDiv(void);

/**
 * @brief	Returns the SPIFI clock rate
 * @return	SPIFI clock clock rate
 */
uint32_t Chip_SYSCON_GetSPIFIRate(void);

#endif

#define SYSCON_PLL_ENABLE   (1 << 0)/* PLL enable flag */
#if defined(CHIP_LPC175X_6X)
#define SYSCON_PLL_CONNECT (1 << 1)	/* PLL connect flag only applies to 175x/6x */
#endif

/**
 * @brief	Enables or connects a PLL
 * @param	pllNum:	PLL number
 * @param	flags:	SYSCON_PLL_ENABLE or SYSCON_PLL_CONNECT
 * @return	Nothing
 * @note	This will also perform a PLL feed sequence. Connect only applies to the
 * LPC175x/6x devices.
 */
void Chip_SYSCON_PLLEnable(CLK_PLL_T pllNum, uint32_t flags);

/**
 * @brief	Disables or disconnects a PLL
 * @param	pllNum:	PLL number
 * @param	flags:	SYSCON_PLL_ENABLE or SYSCON_PLL_CONNECT
 * @return	Nothing
 * @note	This will also perform a PLL feed sequence. Connect only applies to the
 * LPC175x/6x devices.
 */
void Chip_SYSCON_PLLDisable(CLK_PLL_T pllNum, uint32_t flags);

#if defined(CHIP_LPC175X_6X)
#define SYSCON_PLL0STS_ENABLED   (1 << 24)	/* PLL0 enable flag */
#define SYSCON_PLL0STS_CONNECTED (1 << 25)	/* PLL0 connect flag */
#define SYSCON_PLL0STS_LOCKED    (1 << 26)	/* PLL0 connect flag */
#define SYSCON_PLL1STS_ENABLED   (1 << 8)	/* PLL1 enable flag */
#define SYSCON_PLL1STS_CONNECTED (1 << 9)	/* PLL1 connect flag */
#define SYSCON_PLL1STS_LOCKED    (1 << 10)	/* PLL1 connect flag */
#else
#define SYSCON_PLLSTS_ENABLED   (1 << 8)	/* PLL enable flag */
#define SYSCON_PLLSTS_LOCKED    (1 << 10)	/* PLL connect flag */
#endif

/**
 * @brief	Returns PLL status
 * @param	pllNum:		PLL number
 * @return	Current enabled flags, Or'ed SYSCON_PLLSTS_* states
 * @note	Note flag positions for PLL0 and PLL1 differ on the LPC175x/6x devices.
 */
STATIC INLINE uint32_t Chip_SYSCON_PLLStatus(CLK_PLL_T pllNum)
{
	return LPC_SYSCON->PLL[pllNum].PLLSTAT;
}

/**
 * @brief	Feeds a PLL
 * @param	pllNum:	PLL number
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_PLLFeed(CLK_PLL_T pllNum)
{
	LPC_SYSCON->PLL[pllNum].PLLFEED = 0xAA;
	LPC_SYSCON->PLL[pllNum].PLLFEED = 0x55;
}

/**
 * PLL frequency setup structure
 * See the User Manual for limitations on these values for stable PLL
 * operation. Be careful with these values - they must be safe values for the
 * msl, nsel, and psel registers so must be already decremented by 1 or the
 * the correct value for psel (0 = div by 1, 1 = div by 2, etc.).
 */
typedef struct {
	uint32_t msel;			/* Must be pre-decremented */
	uint32_t nsel_psel;		/* Must be pre-decremented */
} SYSCON_PLLSETUP_T;

/**
 * @brief	Sets up a PLL
 * @param	pllNum:		PLL number
 * @param	pllSetup:	Pointer to PLL setup data
 * @return	Nothing
 */
void Chip_SYSCON_PLLSetup(CLK_PLL_T pllNum, SYSCON_PLLSETUP_T *pllSetup);

/**
 * @brief	Returns the PLL current clock rate
 * @param	pllNum:	PLL number
 * @return	Current PLL rate
 * @note	This function will look at the PLL's input clock and configuration
 * to determine the clock rate. If the PLL is not enabled, a rate of
 * 0 is returned.
 */
uint32_t Chip_SYSCON_GetPLLRate(CLK_PLL_T pllNum);

#define SYSCON_PD_SMFLAG (1 << 8)	/* Sleep Mode entry flag */
#define SYSCON_PD_DSFLAG (1 << 9)	/* Deep Sleep entry flag */
#define SYSCON_PD_PDFLAG (1 << 10)	/* Power-down entry flag */
#define SYSCON_PD_DPDFLAG (1 << 11)	/* Deep Power-down entry flag */

/**
 * @brief	Returns and clears the current sleep mode entry flags
 * @param	flags:	One or more flags to clear, SYSCON_PD_*
 * @return	An Or'ed value of the sleep flags, SYSCON_PD_*
 * @note	These flags indicate the successful entry of one or more
 * sleep modes.
 */
uint32_t Chip_SYSCON_GetClrSleepFlags(uint32_t flags);

/**
 * Clock sources for the CLKOUT pin
 */
typedef enum {
	SYSCON_CLKOUT_CPU,			/* CPU clock as CLKOUT source */
	SYSCON_CLKOUT_MAINOSC,		/* Main oscillator clock as CLKOUT source */
	SYSCON_CLKOUT_IRC,			/* IRC oscillator clock as CLKOUT source */
	SYSCON_CLKOUT_USB,			/* USB clock as CLKOUT source */
	SYSCON_CLKOUT_RTC,			/* RTC clock as CLKOUT source */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLKOUT_RESERVED1,
	SYSCON_CLKOUT_RESERVED2,
#else
	SYSCON_CLKOUT_SPIFI,		/* SPIFI clock as CLKOUT source */
	SYSCON_CLKOUT_WATCHDOGOSC,	/* Watchdog oscillator as CLKOUT source */
#endif
	SYSCON_CLKOUT_RESERVED3
} SYSCON_CLKOUT_SRC_T;

/**
 * @brief	Selects a source clock and divider rate for the CLKOUT pin
 * @param	clk:	Clock to set divider for
 * @param	clkDiv:	Divider for the clock source on CLKOUT, 1 to 16
 * @return	Nothing
 * @note	This function will disable the CLKOUT signal if its enabled. Use
 * Chip_SYSCON_CLKOUTEnable to re-enable CLKOUT after a call to this
 * function.
 */
void Chip_SYSCON_CLKOUTSetup(SYSCON_CLKOUT_SRC_T clk, uint32_t clkDiv);

/**
 * @brief	Enables the clock on the CLKOUT pin
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_CLKOUTEnable(void)
{
	LPC_SYSCON->CLKOUTCFG |= (1 << 8);
}

/**
 * @brief	Disables the clock on the CLKOUT pin
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_CLKOUTDisable(void)
{
	LPC_SYSCON->CLKOUTCFG &= ~(1 << 8);
}

/**
 * @brief	Returns the CLKOUT activity indication status
 * @return	true if CLKOUT is enabled, false if disabled and stopped
 * @note	CLKOUT activity indication. Reads as true when CLKOUT is
 * enabled. Read as false when CLKOUT has been disabled via
 * the CLKOUT_EN bit and the clock has completed being stopped.
 */
STATIC INLINE bool Chip_SYSCON_CLKOUTStatus(void)
{
	return (bool) ((LPC_SYSCON->CLKOUTCFG & (1 << 9)) != 0);
}

#if !defined(CHIP_LPC175X_6X)
/**
 * @brief	Enable power boost for clock operation over 100MHz
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_EnableBoost(void)
{
	LPC_SYSCON->PBOOST = 0x3;
}

/**
 * @brief	Disable power boost for clock operation under 100MHz
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_DisableBoost(void)
{
	LPC_SYSCON->PBOOST = 0x0;
}

/**
 * @brief	Set the LCD clock prescaler
 * @param	div:	Divider value, minimum of 1
 * @return	Nothing
 */
STATIC INLINE void Chip_SYSCON_SetLCDDiv(uint32_t div)
{
	LPC_SYSCON->LCD_CFG = (div - 1);
}

/**
 * @brief	Get the LCD clock prescaler
 * @return	Current divider value
 */
STATIC INLINE uint32_t Chip_SYSCON_GetLCDDiv(void)
{
	return (LPC_SYSCON->LCD_CFG & 0x1F) + 1;
}

#endif

/**
 * Peripheral clocks
 * This is a list of clock and power enables
 */
typedef enum {
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_RSVD0,
#else
	SYSCON_CLK_LCD,					/* LCD clock */
#endif
	SYSCON_CLK_TIMER0,			/* Timer 0 clock */
	SYSCON_CLK_TIMER1,			/* Timer 1 clock */
	SYSCON_CLK_UART0,				/* UART 0 clock */
	SYSCON_CLK_UART1,				/* UART 1 clock */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_RSVD5,
#else
	SYSCON_CLK_PWM0,				/* PWM0 clock */
#endif
	SYSCON_CLK_PWM1,				/* PWM1 clock */
	SYSCON_CLK_I2C0,				/* I2C0 clock */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_SPI,					/* SPI clock */
#else
	SYSCON_CLK_UART4,				/* UART 4 clock */
#endif
	SYSCON_CLK_RTC,					/* RTC clock */
	SYSCON_CLK_SSP1,				/* SSP1 clock */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_RSVD11,
#else
	SYSCON_CLK_EMC,					/* EMC clock */
#endif
	SYSCON_CLK_ADC,					/* ADC clock */
	SYSCON_CLK_CAN1,				/* CAN1 clock */
	SYSCON_CLK_CAN2,				/* CAN2 clock */
	SYSCON_CLK_GPIO,				/* GPIO clock */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_PCRIT,				/* RIT clock */
#else
	SYSCON_CLK_SPIFI,				/* SPIFI clock */
#endif
	SYSCON_CLK_MCPWM,				/* MCPWM clock */
	SYSCON_CLK_QEI,					/* QEI clock */
	SYSCON_CLK_I2C1,				/* I2C1 clock */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_RSVD20,
#else
	SYSCON_CLK_SSP2,				/* SSP2 clock */
#endif
	SYSCON_CLK_SSP0,				/* SSP0 clock */
	SYSCON_CLK_TIMER2,			/* Timer 2 clock */
	SYSCON_CLK_TIMER3,			/* Timer 3 clock */
	SYSCON_CLK_UART2,				/* UART 2 clock */
	SYSCON_CLK_UART3,				/* UART 3 clock */
	SYSCON_CLK_I2C2,				/* I2C2 clock */
	SYSCON_CLK_I2S,					/* I2S clock */
#if defined(CHIP_LPC175X_6X)
	SYSCON_CLK_RSVD28,
#else
	SYSCON_CLK_PCSDC,				/* SD Card interface clock */
#endif
	SYSCON_CLK_GPDMA,				/* GP DMA clock */
	SYSCON_CLK_ENET,				/* EMAC/Ethernet clock */
	SYSCON_CLK_USB,					/* USB clock */
	SYSCON_CLK_RSVD32,
	SYSCON_CLK_RSVD33,
	SYSCON_CLK_RSVD34,
#if defined(CHIP_LPC407X_8X)
	SYSCON_CLK_PCCMP,				/* Comparator clock (PCONP1) */
#else
	SYSCON_CLK_RSVD35,
#endif
} SYSCON_CLK_T;

/**
 * @brief	Enables power and clocking for a peripheral
 * @param	clk:	Clock to enable
 * @return	Nothing
 * @note	Only peripheral clocks that are defined in the PCONP registers of the clock
 * and power controller can be enabled and disabled with this function.
 * Some clocks need to be enabled elsewhere (ie, USB) and will return
 * false to indicate it can't be enabled with this function.
 */
void Chip_SYSCON_PCLKEnable(SYSCON_CLK_T clk);

/**
 * @brief	Disables power and clocking for a peripheral
 * @param	clk:	Clock to disable
 * @return	Nothing
 * @note	Only peripheral clocks that are defined in the PCONP registers of the clock
 * and power controller can be enabled and disabled with this function.
 * Some clocks need to be disabled elsewhere (ie, USB) and will return
 * false to indicate it can't be disabled with this function.
 */
void Chip_SYSCON_PCLKDisable(SYSCON_CLK_T clk);

/**
 * @brief	Returns power enables state for a peripheral
 * @param	clk:	Clock to check
 * @return	true if the clock is enabled, false if disabled
 */
bool Chip_SYSCON_PCLKIsEnabled(SYSCON_CLK_T clk);

/**
 * @brief	Returns clock rate for a peripheral (from peripheral clock)
 * @param	clk:	Clock to get rate of
 * @return	Clock rate for the peripheral
 * @note	This covers most common peripheral clocks, but not every clock
 * in the system. LPC177x/8x and LPC407x/8x devices use the same
 * clock for all periphreals, while the LPC175x/6x have unique
 * dividers that may alter the peripheral clock rate.
 */
uint32_t Chip_SYSCON_GetPCLKRateFromClk(SYSCON_CLK_T clk);

#if !defined(CHIP_LPC175X_6X)
/**
 * Peripheral reset numbers
 * This is a list of peripherals that can be reset
 */
typedef enum {
	SYSCON_RESET_LCD,					/* LCD reset */
	SYSCON_RESET_TIMER0,			/* Timer 0 reset */
	SYSCON_RESET_TIMER1,			/* Timer 1 reset */
	SYSCON_RESET_UART0,				/* UART 0 reset */
	SYSCON_RESET_UART1,				/* UART 1 reset */
	SYSCON_RESET_PWM0,				/* PWM0 reset */
	SYSCON_RESET_PWM1,				/* PWM1 reset */
	SYSCON_RESET_I2C0,				/* I2C0 reset */
	SYSCON_RESET_UART4,				/* UART 4 reset */
	SYSCON_RESET_RTC,					/* RTC reset */
	SYSCON_RESET_SSP1,				/* SSP1 reset */
	SYSCON_RESET_EMC,					/* EMC reset */
	SYSCON_RESET_ADC,					/* ADC reset */
	SYSCON_RESET_CAN1,				/* CAN1 reset */
	SYSCON_RESET_CAN2,				/* CAN2 reset */
	SYSCON_RESET_GPIO,				/* GPIO reset */
	SYSCON_RESET_SPIFI,				/* SPIFI reset */
	SYSCON_RESET_MCPWM,				/* MCPWM reset */
	SYSCON_RESET_QEI,					/* QEI reset */
	SYSCON_RESET_I2C1,				/* I2C1 reset */
	SYSCON_RESET_SSP2,				/* SSP2 reset */
	SYSCON_RESET_SSP0,				/* SSP0 reset */
	SYSCON_RESET_TIMER2,			/* Timer 2 reset */
	SYSCON_RESET_TIMER3,			/* Timer 3 reset */
	SYSCON_RESET_UART2,				/* UART 2 reset */
	SYSCON_RESET_UART3,				/* UART 3 reset */
	SYSCON_RESET_I2C2,				/* I2C2 reset */
	SYSCON_RESET_I2S,					/* I2S reset */
	SYSCON_RESET_PCSDC,				/* SD Card interface reset */
	SYSCON_RESET_GPDMA,				/* GP DMA reset */
	SYSCON_RESET_ENET,				/* EMAC/Ethernet reset */
	SYSCON_RESET_USB,					/* USB reset */
	SYSCON_RESET_IOCON,				/* IOCON reset */
	SYSCON_RESET_DAC,					/* DAC reset */
	SYSCON_RESET_CANACC,			/* CAN acceptance filter reset */
} SYSCON_RESET_T;

/**
 * @brief	Resets a peripheral
 * @param	periph:	Peripheral to reset
 * @return	Nothing
 */
void Chip_SYSCON_PeriphReset(SYSCON_RESET_T periph);

#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _SYSCON_17XX_40XX_H_ */
