/*************************************************************************
 *
 * The usage of GPIO interrupt function.
 *
 **************************************************************************/
#include <stdio.h>
#include "NuMicro.h"


//                0    1   2    3     4   5    6    7   8    9
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
uint8_t z = 0;
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

void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    /* Select HCLK clock source as HIRC and and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
    CLK_EnableModuleClock(GPC_MODULE);
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
    printf("+------------------------------------------------+\n");
    printf("|    GPIO PA.0 and PA.1 Interrupt Sample Code    |\n");
    printf("+------------------------------------------------+\n\n");
	
    /*Configure PB.0 PB.1 PB.2 PB.3 PB.4 PB.5 PB.6 as Output mode*/
    GPIO_SetMode(PB, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6), GPIO_MODE_OUTPUT);
	
    /* Configure PA.0 PA.1 as Input mode and enable interrupt by rising edge trigger */
    GPIO_SetMode(PA, BIT0|BIT1,GPIO_MODE_INPUT);
    GPIO_EnableInt(PA, 0, GPIO_INT_RISING);
		GPIO_EnableInt(PA, 1, GPIO_INT_RISING);
    NVIC_EnableIRQ(GPA_IRQn);

		PB->DOUT=led[0];

    /* Waiting for interrupts */
    while (1);
}


