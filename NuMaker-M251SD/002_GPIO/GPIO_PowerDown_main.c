/*******************************************************************************
 * Show how to wake up system from Power-down mode by GPIO interrupt.
 * Как вывести систему из режима Power-down с помощью прерывания GPIO. 
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Function for System Entry to Power Down Mode                                                           */
/*  Функция входа системы в режим пониженного энергопотребления                                            */
/*---------------------------------------------------------------------------------------------------------*/
void PowerDownFunction(void)
{
    /* Check if all the debug messages are finished ... Проверьте, все ли сообщения отладки завершены */
    UART_WAIT_TX_EMPTY(UART0);
/**
 *    @brief        Wait specified UART port transmission is over
 *                  Дождитесь завершения передачи указанного порта UART 
 *    @param[in]    uart    The pointer of the specified UART module
 *                          Указатель указанного модуля UART 
 *    @return       None
 *
 *    @details      This macro wait specified UART port transmission is over.
 *                  Этот макрос ожидает завершения передачи указанного порта UART.   
 *    #define UART_WAIT_TX_EMPTY(uart)    while(!((((uart)->FIFOSTS) & UART_FIFOSTS_TXEMPTYF_Msk) >> UART_FIFOSTS_TXEMPTYF_Pos))
 */		

    /* Enter to Power-down mode ... Войдите в режим пониженного энергопотребления */
    CLK_PowerDown();
	
/*	void CLK_PowerDown(void)
{
    uint32_t u32HIRCTRIMCTL, u32MIRCTRIMCTL;

    // Set the processor uses deep sleep as its low power mode 
    //устанавливаем режим процессора, использующий глубокий сон в качестве режима низкого энергопотребления. 
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Set system Power-down enabled
	  //отключение питания системы 
    CLK->PWRCTL |= (CLK_PWRCTL_PDEN_Msk);

    // Store HIRC/MIRC control register 
	  //Сохраняем регистр управления HIRC / MIRC 
    u32HIRCTRIMCTL = SYS->HIRCTRIMCTL;
    u32MIRCTRIMCTL = SYS->MIRCTRIMCTL;

    // Disable HIRC/MIRC auto trim
	  //Отключить автоматическую обрезку HIRC / MIRC ???
    SYS->HIRCTRIMCTL &= (~SYS_HIRCTRIMCTL_FREQSEL_Msk);
    SYS->MIRCTRIMCTL &= (~SYS_MIRCTRIMCTL_FREQSEL_Msk);

    // Chip enter Power-down mode after CPU run WFI instruction
	  //Чип переходит в режим пониженного энергопотребления после того, как ЦП запустит инструкцию WFI ???
    __WFI();

    // Restore HIRC/MIRC control register
	  //	Восстановить регистр управления HIRC / MIRC ???
    SYS->HIRCTRIMCTL = u32HIRCTRIMCTL;
    SYS->MIRCTRIMCTL = u32MIRCTRIMCTL;
}*/
}
/*---------------------------------------------------------------------------------------------------------*/
/*
 * @brief       GPIO PB IRQ
 * @details     The PB default IRQ, declared in startup_M251.s.  
 * IRQ по умолчанию для PB, объявленный в startup_M251.s. 
 */
void GPB_IRQHandler(void)
{
    /* To check if PB.3 interrupt occurred */
    if (GPIO_GET_INT_FLAG(PB, BIT3))
    {
        GPIO_CLR_INT_FLAG(PB, BIT3);
        printf("PB.3 INT occurred.\n");
    }
    else
    {
        /* Un-expected interrupt. Just clear all PB interrupts */
        PB->INTSRC = PB->INTSRC;
        printf("Un-expected interrupts.\n");
    }
}
/*---------------------------------------------------------------------------------------------------------*/
void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC clock (Internal RC 48 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);

    /* Select UART module clock source as HIRC and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    Uart0DefaultMPF();

}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART module */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
    printf("+-------------------------------------------------------+\n");
    printf("|    GPIO Power-Down and Wake-up by PB.3 Sample Code    |\n");
    printf("+-------------------------------------------------------+\n\n");

    /* Configure PB.3 as Input mode and enable interrupt by rising edge trigger */
    GPIO_SetMode(PB, BIT3, GPIO_MODE_INPUT);
    GPIO_EnableInt(PB, 3, GPIO_INT_RISING);
    NVIC_EnableIRQ(GPB_IRQn);

    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    // GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);
    //  GPIO_ENABLE_DEBOUNCE(PB, BIT3);

    /* Unlock protected registers before entering Power-down mode */
    SYS_UnlockReg();

    /* Waiting for PB.3 rising-edge interrupt event */
    while (1)
    {
        printf("Enter to Power-Down ......\n");

        /* Enter to Power-down mode */
        PowerDownFunction();

        printf("System waken-up done.\n\n");
    }

}
/*---------------------------------------------------------------------------------------------------------*/
