/**---------------------------------------------
*\date 29.12.2022
*\brief
*     NuMaker-M483KG V1.1
*     M483KGCAE2A 256 KB flash 128 KB RAM
*   ------------
*  |            |
*  |       PG14 | <-- Button SW4
*  |            |
*  |       PG15 | <-- Button SW3
*  |            |
*  |        PH4 | --> LED_R
*  |            |
*  |        PH5 | --> LED_Y
*  |            |
*  |        PH6 | --> LED_G
*
*\authors ScuratovaAnna + PivnevNikolay
-----------------------------------------------*/
//----------------------------------------------------------------------------------------------//
#include <stdio.h>
#include "NuMicro.h"
#include "stdbool.h"
#include "init.h"

#define Led_Y PH5

int main()
{
    SYS_Init();
    UART_Open(UART0, 115200);
    printf("Hello World\n");
    //------------------PH6---------------------//
    GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT);
    PH6=1;
    //------------------PH5---------------------//
    PH->MODE = (PH->MODE & ~(GPIO_MODE_MODE5_Msk)) | (GPIO_MODE_OUTPUT << GPIO_MODE_MODE5_Pos);
    PH->DOUT = (PH->DOUT | (GPIO_DOUT_DOUT5_Msk));
    //------------------PH4---------------------//
    PH->MODE &= ~((0x1UL << 9)|(0x1UL << 8));
    PH->MODE |=  (0x1UL << 8);
    PH->DOUT |=  (0x1UL << 4);

    while(1){
    PH4 ^= 1;
    Led_Y = !Led_Y;
    GPIO_TOGGLE(PH6);
    TIMER_Delay(TIMER0, 1000000);
    }
}
//----------------------------------------------------------------------------------------------//
#include <stdio.h>
#include "NuMicro.h"
#include "stdbool.h"
#include "init.h"

typedef uint8_t   u8;
//----------110--100--000--001--011--111--------//
u8 CW[6] = {0x60,0x40,0x00,0x10,0x30,0x70};

#define Led_Y PH5

int main()
{
    SYS_Init();
    UART_Open(UART0, 115200);
    printf("Hello World\n");
    //------------------PH6---------------------//
    GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT);
    PH6=1;
    //------------------PH5---------------------//
    PH->MODE = (PH->MODE & ~(GPIO_MODE_MODE5_Msk)) | (GPIO_MODE_OUTPUT << GPIO_MODE_MODE5_Pos);
    PH->DOUT = (PH->DOUT | (GPIO_DOUT_DOUT5_Msk));
    //------------------PH4---------------------//
    PH->MODE &= ~((0x1UL << 9)|(0x1UL << 8));
    PH->MODE |=  (0x1UL << 8);
    PH->DOUT |=  (0x1UL << 4);
    //------------------PG14---------------------//
    PG->MODE = (PG->MODE & ~(GPIO_MODE_MODE14_Msk)) | (GPIO_MODE_INPUT << GPIO_MODE_MODE14_Pos);
    GPIO->DBCTL = (GPIO_DBCTL_ICLKON_Msk | (GPIO_DBCTL_DBCLKSRC_LIRC) | (GPIO_DBCTL_DBCLKSEL_8));
    PG->DBEN |= BIT14;

    while(1){
   if(PG14 != 0){
     PH4 ^= 1;
     Led_Y = !Led_Y;
     GPIO_TOGGLE(PH6);
     TIMER_Delay(TIMER0, 1000000);
   }else{
     for (u8 i=0;i<6;i++){
     PH->DOUT = *(CW+i);
     TIMER_Delay(TIMER0,  100000);
    }
   }
  }
}
//----------------------------------------------------------------------------------------------//