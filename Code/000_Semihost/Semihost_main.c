/*****************************************************************************
 * 16.03.2021
 * @file     main.c
 * @version  V0.10
 * @brief    Show how to print and get character with IDE console window.
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"
    //-----------------------------------------------------------------------------//
#if defined (__GNUC__) && !defined(__ARMCC_VERSION) && defined(OS_USE_SEMIHOSTING)
    extern void initialise_monitor_handles(void);
#endif
    //-----------------------------------------------------------------------------//
void SYS_Init(void)
{    
    //--------------------- Init System Clock -------------------------------------//        
  	/* Unlock protected registers */
    SYS_UnlockReg();
    /* Enable Internal RC 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    /* Enable UART clock */
    CLK_EnableModuleClock(UART0_MODULE);
    /* Select UART clock source from HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();
    
    //--------------------- Init I/O Multi-function --------------------------------//  
    
    Uart0DefaultMPF();
    /* Lock protected registers */
    SYS_LockReg();
}

int main(void)
{
	int8_t ch;  
    SYS_Init();
    //-----------------------------------------------------------------------------//
#if defined (__GNUC__) && !defined(__ARMCC_VERSION) && defined(OS_USE_SEMIHOSTING)
    initialise_monitor_handles();
#endif
    //-----------------------------------------------------------------------------//
    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);
    printf("\n hello world.\r\n");
    while (1)
    {
        ch = getchar();
        printf("%c\n", ch);			
    }
}
/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/