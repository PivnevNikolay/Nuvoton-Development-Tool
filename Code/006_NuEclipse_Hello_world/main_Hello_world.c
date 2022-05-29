/**-----------------------------------------------------------------------------------------------
*\date  29.05.2022
*\brief 
*\authors ScuratovaAnna + PivnevNikolay
* Ссылка на Telegram канал: https://t.me/nuvoton_programming
*/
#include <stdio.h>
#include "NuMicro.h"
#define PLL_CLOCK           192000000
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    SYS_UnlockReg();
    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);
    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);
    /* Enable UART clock */
    CLK_EnableModuleClock(UART0_MODULE);
    /* Select UART clock source from HXT */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    SystemCoreClockUpdate();
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    /* Lock protected registers */
    SYS_LockReg();
}

    /*---------------------------------------------------------------------------------------------------------*/
    /* This is a template project for M480 series MCU.                                                         */
    /* This template application uses external crystal as HCLK source and configures UART0 to print out        */
    /* "Hello World", SystemCoreClock, CLK_GetHXTFreq                                                          */
    /*---------------------------------------------------------------------------------------------------------*/

int main()
{
    SYS_Init();
    UART_Open(UART0, 115200);
    printf("Hello World\n");
    printf("CPU -> %d Hz\n", SystemCoreClock);
    uint32_t GetHXT;
    GetHXT=CLK_GetHXTFreq();
    printf("HXTEN -> %d Hz\n", GetHXT);
    while(1);
}
