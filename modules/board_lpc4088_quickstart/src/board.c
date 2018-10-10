#include "chip.h"

/* Setup system clocking */
void SystemSetupClocking(void)
{
	SYSCON_PLLSETUP_T pllSetup;

	/* CPU clock source starts with IRC */
	Chip_SYSCON_SetSYSCLKSrc(SYSCON_PLLSRC_IRC);
	Chip_SYSCON_SetCPUCLKSource(SYSCON_CPUCLK_SYSCLK);

	/* Enable PBOOST for CPU clock over 100MHz */
	Chip_SYSCON_EnableBoost();

	/* Enable main oscillator used for PLLs */
	LPC_SYSCON->SCS = SYSCON_OSCEC;
	while ((LPC_SYSCON->SCS & SYSCON_OSCSTAT) == 0) {}

	/* PLL0 clock source is 12MHz oscillator, PLL1 can only be the
	   main oscillator */
	Chip_SYSCON_SetSYSCLKSrc(SYSCON_PLLSRC_MAINOSC);

	/* Setup PLL0 for a 120MHz clock
	   Input clock rate (FIN) is main oscillator = 12MHz
	   PLL output = 120MHz = FIN * MSEL = 120MHz, so MSEL = 10
	   FCCO must be between 156 MHz to 320 MHz, where FCCO = PLL output * 2 * P,
	   so P = 1 and FCCO = 120MHz * 2 * 1 = 240MHz */
	pllSetup.msel = 9;		/* Multiply by 10 */
	pllSetup.nsel_psel = 0;	/* Divide by 1 */
	Chip_SYSCON_PLLSetup(SYSCON_PLL0, &pllSetup);

	/* Enable PLL0 */
	Chip_SYSCON_PLLEnable(SYSCON_PLL0, SYSCON_PLL_ENABLE);

	/* Setup USB PLL1 for a 240MHz clock
	   Input clock rate (FIN) is main oscillator = 12MHz
	   PLL output = 240MHz = FIN * MSEL = 120MHz, so MSEL = 20
	   FCCO must be between 156 MHz to 320 MHz, where FCCO = PLL output * 2 * P,
	   so P = 1 and FCCO = 240MHz * 2 * 1 = 240MHz */
	pllSetup.msel = 19;		/* Multiply by 20 */
	pllSetup.nsel_psel = 0;	/* Divide by 1 */
	Chip_SYSCON_PLLSetup(SYSCON_PLL1, &pllSetup);

	/* Since the USB PLL clock is 240MHz, we need to divide by 5 to get 48Mhz
	   as needed by the USB peripheral. Also set the USB divider source to
	     the USB PLL. USB clock rate = 240 / 5 = 48MHz */
	Chip_SYSCON_SetUSBClkDivSrc(SYSCON_USDIVSRC_PLL1);
	Chip_SYSCON_SetUSBClkDiv(5);

	/* PLL1 is disabled until needed */
	Chip_SYSCON_PLLDisable(SYSCON_PLL1, SYSCON_PLL_ENABLE);

	/* Wait for main (CPU) PLL0 to lock */
	while ((Chip_SYSCON_PLLStatus(SYSCON_PLL0) & SYSCON_PLLSTS_LOCKED) == 0) {}

	/* The CPU is still sourced from the SYSCLK, so set the CPU divider to
	   1 and switch it to the PLL0 clock */
	Chip_SYSCON_SetCPUClkDiv(1);
	Chip_SYSCON_SetCPUCLKSource(SYSCON_CPUCLK_PLL0);

	/* Peripheral clocking will be derived from PLL0 with a divider of 2 (60MHz) */
	Chip_SYSCON_SetPCLKDiv(2);
}

