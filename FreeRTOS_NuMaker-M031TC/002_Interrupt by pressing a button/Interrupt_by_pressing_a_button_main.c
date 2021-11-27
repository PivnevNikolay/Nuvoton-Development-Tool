/*
27.11.2021
https://www.freertos.org/taskresumefromisr.html
*/
#include <stdio.h>
#include "NuMicro.h"
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

uint32_t LastWakeTime;
TaskHandle_t myIntTaskHandle= NULL;
TaskHandle_t myTask1Handle = NULL;
BaseType_t xYieldRequired;
/*-------------------------------------------------------------------------*/
void GPAB_IRQHandler(void)
{
volatile uint32_t temp;
BaseType_t xYieldRequired;
  if(GPIO_GET_INT_FLAG(PB, BIT2))
    {
        GPIO_CLR_INT_FLAG(PB, BIT2);
        printf("PB.2 INT occurred.\n");	
    }
  else
    {
        temp = PB->INTSRC;
        PB->INTSRC = temp;
        printf("Un-expected interrupts.\n");
    }
    xYieldRequired = xTaskResumeFromISR(myIntTaskHandle);
    portYIELD_FROM_ISR(xYieldRequired);
}
/*-------------------------------------------------------------------------*/
void SYS_Init(void)
{
    SYS_UnlockReg();
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
    SystemCoreClockUpdate();
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    SYS_LockReg(); 
}
/*-------------------------------------------------------------------------*/
void UART0_Init()
{
    SYS_ResetModule(UART0_RST);
    UART_Open(UART0, 115200);
}
/*-------------------------------------------------------------------------*/
void myIntTask(void *arg)
{
	while(1) {
		vTaskSuspend(NULL);
		printf("you pressed a button\r\n");
	}
}
/*-------------------------------------------------------------------------*/
void LED_Task(void *arg)
{
  (void)arg;
		LastWakeTime = xTaskGetTickCount();	
	while(1) {
		GPIO_TOGGLE(PB14); 
		printf("LED_Task_running\n");
		vTaskDelayUntil(&LastWakeTime, 1000);
	}
}
/*-------------------------------------------------------------------------*/
int main()
{	  
    SYS_Init();    
    UART0_Init();
	GPIO_SetMode(PB, BIT14, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT2, GPIO_MODE_INPUT);
    GPIO_EnableInt(PB, 2, GPIO_INT_RISING);
    NVIC_EnableIRQ(GPIO_PAPB_IRQn);//GPAB_IRQHandler
    printf("\nLED Blink_FreeRTOS Sample Program\n");
/*---------- Create LED task, which will blink the RED LED PB14------------*/
    xTaskCreate(
        LED_Task,       /* Task function */
        "LED Blink",    /* Task name (string) */
        50,            /* Task stack, allocated from heap */
        0,              /* No param passed to task function */
        tskIDLE_PRIORITY,              /* Low priority */
        &myTask1Handle );            /* Not using the task handle */
/*-Create a task for the button(PB2), when pressed, an interruption occurs-*/
        xTaskCreate(
        myIntTask,       /* Task function */
        "IntTask",    /* Task name (string) */
        50,            /* Task stack, allocated from heap */
        0,              /* No param passed to task function */
        tskIDLE_PRIORITY,              /* Low priority */
        &myIntTaskHandle ); 
    vTaskStartScheduler();
    while(1);
}
/*-----------------------------------END-----------------------------------*/