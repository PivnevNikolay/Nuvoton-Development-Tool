/*******************************************************************************
 * Show how to wake up system from Power-down mode by GPIO interrupt.
 * Как вывести систему из режима Power-down с помощью прерывания GPIO. 
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

//                0    1   2    3     4   5    6    7   8    9
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
uint8_t z = 0;
uint8_t i = 0;

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
void GPA_IRQHandler(void)
{
    /* To check if PA.0 interrupt occurred */
    if (GPIO_GET_INT_FLAG(PA, BIT0))
    {
        GPIO_CLR_INT_FLAG(PA, BIT0);
        printf("PA.0 interrupt occurred.\n");
			  z++;
			  if(z>=9)z=9;
			  PB->DOUT=led[z];
			printf("z = %u \n",z);
    }
		/* To check if PA.1 interrupt occurred */
    else if (GPIO_GET_INT_FLAG(PA, BIT1))
    {
        GPIO_CLR_INT_FLAG(PA, BIT1);
        printf("PA.1 interrupt occurred.\n");
				z--;
			if(z==255)z=0;
			PB->DOUT=led[z];
			printf("z = %u \n",z);
    }
    else
    {
        /* Un-expected interrupt. Just clear all PA interrupts */
        PA->INTSRC = PA->INTSRC;
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
	
	  /*Configure PB.0 PB.1 PB.2 PB.3 PB.4 PB.5 PB.6 as Output mode*/
    GPIO_SetMode(PB, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6), GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PB, (BIT14), GPIO_MODE_OUTPUT);

    /* Configure PA.0 PA.1 as Input mode and enable interrupt by rising edge trigger */
    GPIO_SetMode(PA, BIT0|BIT1,GPIO_MODE_INPUT);
    GPIO_EnableInt(PA, 0, GPIO_INT_RISING);
	  GPIO_EnableInt(PA, 1, GPIO_INT_RISING);
    NVIC_EnableIRQ(GPA_IRQn);

	  PB->DOUT=led[0];

    /* Unlock protected registers before entering Power-down mode */
    SYS_UnlockReg();

    /* Waiting for PB.3 rising-edge interrupt event */
    while (1)
    {
			  for(i=0;i<=20;i++){
			  GPIO_TOGGLE(PB14);
			  CLK_SysTickDelay(349525);					
        printf("i = %u \n",i);
        /* Enter to Power-down mode */
					if(i==5){
        PowerDownFunction();
        printf("System waken-up done.\n");
			  CLK_SysTickDelay(349525);
					}
				}
    }

}
/*---------------------------------------------------------------------------------------------------------*/
