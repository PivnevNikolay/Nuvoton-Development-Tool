/*
02.11.2021
Blinking LED PB14 vTaskDelayUntil();
http://microsin.net/programming/arm/freertos-part1.html
http://easyelectronics.ru/freertos_manual.html
https://microtechnics.ru/stm32cubemx-bystryj-start-s-freertos-dlya-stm32/
*/
#include <stdio.h>
#include "NuMicro.h"
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

//TickType_t LastWakeTime;
uint32_t LastWakeTime;
//const TickType_t xFrequency = 1000;

void SYS_Init(void)
{
		SYS_UnlockReg();
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
		SYS_LockReg(); 
}

void UART0_Init()
{
    SYS_ResetModule(UART0_RST);
    UART_Open(UART0, 115200);
}

void LED_Task(void *arg)
{
    (void)arg;
	GPIO_SetMode(PB, BIT14, GPIO_MODE_OUTPUT); 
	LastWakeTime = xTaskGetTickCount();	
	while(1) {
	GPIO_TOGGLE(PB14); 
	printf("LED_Task_running\n");		
	//vTaskDelayUntil(&LastWakeTime, xFrequency);
	vTaskDelayUntil(&LastWakeTime, 1000);

	}
}

int main()
{	  
    SYS_Init();    
    UART0_Init();
    printf("\nLED Blink_FreeRTOS Sample Program\n");
/*---------- Create LED task, which will blink the RED LED PB14------------*/
    xTaskCreate(
        LED_Task,       /* Task function */
        "LED Blink",    /* Task name (string) */
        40,            /* Task stack, allocated from heap */
        0,              /* No param passed to task function */
        1,              /* Low priority */
        0 );            /* Not using the task handle */
    vTaskStartScheduler();
    while(1);
}
