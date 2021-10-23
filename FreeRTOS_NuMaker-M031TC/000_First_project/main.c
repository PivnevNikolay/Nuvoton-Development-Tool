/*
Настройки параметров FreeRTOS --> 
http://microsin.net/programming/arm/freertos-customisation.html
FreeRTOS: практическое применение, часть 1 (управление задачами) --> 
http://microsin.net/programming/arm/freertos-part1.html
*/
#include <stdio.h>
#include "NuMicro.h"
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

void SYS_Init(void)
{
 /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    /* Enable UART0 clock */
    CLK_EnableModuleClock(UART0_MODULE);
    /* Switch UART0 clock source to HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
    /* Update System Core Clock */
    SystemCoreClockUpdate();
    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
}

void UART0_Init()
{
    SYS_ResetModule(UART0_RST);
    UART_Open(UART0, 115200);
}


void Task0(void *pvParameters)
{
    printf("Task0 running ...\n");
    for(;;)
    {   
        printf("Task0\n");
        vTaskDelay(1000);
    }
}

void Task1(void *pvParameters)
{
    printf("Task1 running ...\n");
    for(;;)
    {   
        printf("Task1\n");
        vTaskDelay(1000);
    }
}

int main()
{	  
    SYS_UnlockReg();
    SYS_Init();    
    SYS_LockReg(); 
    UART0_Init();
    printf("\nUART Sample Program\n");
    xTaskCreate(Task0, (const char *)"Task0", 40, NULL, 1, NULL);
    xTaskCreate(Task1, (const char *)"Task1", 40, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1);
}
