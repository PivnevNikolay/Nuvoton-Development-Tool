#include "stdio.h"
#include "NuMicro.h"
#define Led  PB0
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

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
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Select UART clock source from HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK0, 0);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();


    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
   // Uart0DefaultMPF();
    SYS->GPA_MFPH = (SYS->GPA_MFPH & ~SYS_GPA_MFPH_PA15MFP_Msk) | SYS_GPA_MFPH_PA15MFP_UART0_RXD;
    SYS->GPA_MFPH = (SYS->GPA_MFPH & ~SYS_GPA_MFPH_PA14MFP_Msk) | SYS_GPA_MFPH_PA14MFP_UART0_TXD ;


    /* Lock protected registers */
    SYS_LockReg();
}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART module */
    //SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main()
{
	 /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

   /* Init UART0 for printf */
    UART0_Init();


 //PB->MODE = (PB->MODE & ~(GPIO_MODE_MODE14_Msk)) | (GPIO_MODE_OUTPUT << GPIO_MODE_MODE14_Pos);
    GPIO_SetMode(PB, BIT0, GPIO_MODE_OUTPUT);
    printf("\n Start SEMIHOST test: \n");
    while(1){
    GPIO_TOGGLE(Led);
    printf("System clock rate: %d Hz \n\r", SystemCoreClock);
    printf("Led ON\n\r");
    TIMER_Delay(TIMER0, 0xF4240);//Delay 1 second
    //CLK_SysTickDelay(349525);
    GPIO_TOGGLE(Led);
    printf("CPU Freq rate: %d Hz \n\r", CLK_GetCPUFreq());
    printf("Led OFF\n\r");
    TIMER_Delay(TIMER0, 0xF4240);//Delay 1 second
    //CLK_SysTickDelay(349525);
  }
}
