/*  02.01.2021
 *  Отладочная плата NK-M251SD  
 *  https://www.nuvoton.com/board/numaker-m251sd/
 *  Микроконтроллер M251SD2AE
 *  TTP223, модуль сенсорной кнопки емкостной
 *  https://www.chipdip.ru/product/ttp223
 *  
 *  
 */
#include "stdio.h"
#include "NuMicro.h"

void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC clock (Internal RC 48 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

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
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main()
{
 /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();
	  
	printf("\n\nCPU @ %dHz\n", SystemCoreClock);

    printf("+---------------------------------------------+\n");
    printf("|  PB.14(Output) and PC.0(Input) Sample Code  |\n");
    printf("+---------------------------------------------+\n\n");
	//Согласно схемы которая приведена User Manual светодиод подключен к плюсу питания VDD и к выводу PB14
	GPIO_SetMode(PB, BIT14, GPIO_MODE_OUTPUT);//Пин порта установлен в режим выхода — OUTPUT, PB14 как выход без подтяжек
    GPIO_SetMode(PC, BIT0, GPIO_MODE_INPUT);  //Пин порта установлен в режим INPUT, PC0 как вход
	//Подает HIGH значение на цифровой выход (pin) PB14 ниже приведены три способа как это сделать
    (PB)->DOUT = 0x4000;//B0100 0000 0000 0000
	//PB14 = 1;
	//GPIO_SET_OUT_DATA(PB, 0x4000);
    
	while (1){
		if(PC0 != 0){
		GPIO_TOGGLE(PB14);
		printf("Led Toggle\n");
		CLK_SysTickDelay(349525);
		}
	}
}



