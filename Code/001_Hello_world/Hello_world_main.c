/**-----------------------------------------------------------------------------------------------
*\date  03.12.2021
*\brief 
*\authors ScuratovaAnna
* @file     main.c
* Based on the project template for M031 MCU.
* Example taken from the book "C Programming for Beginners" page 34
* Ссылка на Telegram канал: https://t.me/nuvoton_programming
*/
#include <stdio.h>
#include "NuMicro.h"

void UART_Open(UART_T *uart, uint32_t u32baudrate);

void SYS_Init(void)
{
    SYS_UnlockReg();
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);/
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
    SystemCoreClockUpdate();
    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    SYS_LockReg();
}

int main()
{
    SYS_Init();
    UART_Open(UART0, 115200);
    printf("I am learning a programming language %c \n",'C');
		printf("I just finished reading a Chapter %d \n", 2);
	  printf("I'm %.1f percent ready to go", 99.9);
		printf("to the next chapter! \n");
    while(1);
}
/* Program that uses characters, integers numbers and floats */