/******************************************************************************
 * @file     main.c
 * 20.12.2020
 * Blink.
 * Based on the project template for M031 MCU. 
 * This example shows the simplest thing you can do with an NuMaker-M031TC to see physical output: it blinks the on-board LED.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"
//#include <stdbool.h>
//bool flag;


void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Switch UART0 clock source to HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Enable UART peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Enable IP clock */
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, 0);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();

    /*----------------------------------------------------------------------*/
    /* Init I/O Multi-function                                              */
    /*----------------------------------------------------------------------*/
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) |
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Lock protected registers */
    SYS_LockReg();
}


void UART0_Init(void)
{
    /* Reset UART0 */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}



int main()
{
	 /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

   /* Init UART0 for printf */
    UART0_Init();


 PB->MODE = (PB->MODE & ~(GPIO_MODE_MODE14_Msk)) | (GPIO_MODE_OUTPUT << GPIO_MODE_MODE14_Pos);
		//GPIO_SetMode(PB, BIT14, GPIO_MODE_OUTPUT);
    while(1){
			GPIO_TOGGLE(PB14);
			printf("System clock rate: %d Hz \n\r", SystemCoreClock);
			printf("Led ON\n\r");
			TIMER_Delay(TIMER0, 0xF4240);//Delay 1 second
			//CLK_SysTickDelay(349525);//значение CLK_SysTickDelay(349525) является максимальным
			GPIO_TOGGLE(PB14);
			printf("CPU Freq rate: %d Hz \n\r", CLK_GetCPUFreq());
			printf("Led OFF\n\r");
			TIMER_Delay(TIMER0, 0xF4240);//Delay 1 second
			//CLK_SysTickDelay(349525);////значение CLK_SysTickDelay(349525) является максимальным
		}
}
