#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include "NuMicro.h"
#include "system_init.h"

volatile u8 encoder_Counter = 50;
volatile bool flag= false;
volatile bool resetFlag =false;
volatile u8 GPIOState;
volatile u8 BUF[3]={0,0,0};

void encoder_Tick(void);

void GPA_IRQHandler(void){
  volatile u32 u32temp;
   if(GPIO_GET_INT_FLAG(PA, BIT5)) {
     encoder_Tick();
     GPIO_CLR_INT_FLAG(PA, BIT5);
    }
    else if(GPIO_GET_INT_FLAG(PA, BIT4)) {
     encoder_Tick();
     GPIO_CLR_INT_FLAG(PA, BIT4);
    }
   else {
   u32temp = PA->INTSRC;
   PA->INTSRC =  u32temp;
   printf("Un-expected interrupts.\n");
  }
}

int main()
{
    SYS_Init();
  UART_Open(UART0, 115200);
  GPIO_SetMode(PA,BIT5,GPIO_MODE_INPUT);
  GPIO_SetMode(PA,BIT4,GPIO_MODE_INPUT);
  GPIO_EnableInt(PA, 5, GPIO_INT_BOTH_EDGE);//D8
  GPIO_EnableInt(PA, 4, GPIO_INT_BOTH_EDGE);//D9
  NVIC_EnableIRQ(GPA_IRQn);
  GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC,  GPIO_DBCTL_DBCLKSEL_8);
  GPIO_ENABLE_DEBOUNCE(PA, BIT5);//D8
  GPIO_ENABLE_DEBOUNCE(PA, BIT4);//D9
  printf("Hello World\n");

  while(1){
  if (flag) {
    printf("Encoder_counter = %d\n",encoder_Counter);
    flag = false;
    BUF[3]=0;
  }
 }
}

void encoder_Tick(void) {
	GPIOState =PA4|PA5<<1;
	switch (resetFlag) {
    case 0:
     BUF[0]=GPIOState;
     resetFlag=true;
    break;
    case 1:
     BUF[1]=GPIOState;
     BUF[2]=BUF[0]+BUF[1];
     if (BUF[2] == 1u || BUF[2] == 5u) {encoder_Counter++;}
     else if (BUF[2] == 2u || BUF[2] == 4u) {encoder_Counter--;}
     resetFlag =false;
     flag = true;
    break;
  }
}
