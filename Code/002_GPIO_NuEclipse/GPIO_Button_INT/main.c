#include <stdio.h>
#include <stdbool.h>
#include "NuMicro.h"
#include "system_init.h"
volatile uint32_t sw1_int_cnt = 0;

void GPF_IRQHandler(void)
{
    volatile u32 u32temp;
    printf("+---------------------------------------+\n");
    printf("+------------Detect PF---------------+\n");
    printf("+---------------------------------------+\n");
    /* Check if PF.11 the interrupt occurred */
    if(GPIO_GET_INT_FLAG(PF, BIT11)) {
    sw1_int_cnt++;
    GPIO_TOGGLE(PH4);
    printf("press\n");
    /* Clear PF.11 interrupt flag */
    GPIO_CLR_INT_FLAG(PF, BIT11);
    }else {
        /* Un-expected interrupt. Just clear all PB interrupts */
        u32temp = PF->INTSRC;
        PF->INTSRC =  u32temp;
        //PF->INTSRC = PF->INTSRC; --> in the BSP GPIO_INT example, the code is written like this!
        printf("Un-expected interrupts.\n");
    }
}

int main()
{
    uint32_t sw1_cnt = 0;
    SYS_Init();
    UART0_Init();
    //Set PH.4(LED_R)
    GPIO_SetMode(PH,BIT4, GPIO_MODE_OUTPUT);
    GPIO_SET_OUT_DATA(PH, 0x10);//Pin_Set
    //Set PF.11(SW_3)
    GPIO_SetMode(PF,BIT11, GPIO_MODE_INPUT);
    GPIO_EnableInt(PF, 11, GPIO_INT_RISING);//!!! 11 not BIT11
    NVIC_EnableIRQ(GPF_IRQn);
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC,  GPIO_DBCTL_DBCLKSEL_512);
    GPIO_ENABLE_DEBOUNCE(PF, BIT11);
    printf("CPU @ %u Hz\n", SystemCoreClock);
//---------------------------------------------------------------------------//
    while(1){
    if (sw1_int_cnt != sw1_cnt) {
    sw1_cnt = sw1_int_cnt;
    printf("SW1 interrupt count: %d\n", sw1_cnt);
    }
  }
}
