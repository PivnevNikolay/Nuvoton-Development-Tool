/**---------------------------------------------
*\date 29.12.2022
*\brief
*     NuMaker-M483KG V1.1
*     M483KGCAE2A 256 KB flash 128 KB RAM
*   ------------
*  |            |
*  |       PG14 | <-- Button SW3
*  |            |
*  |       PG13 | <-- Button SW2
*  |            |
*  |        PH4 | --> LED_R
*  |            |
*  |        PH5 | --> LED_Y
*  |            |
*  |        PH6 | --> LED_G
*
*\authors ScuratovaAnna + PivnevNikolay
-----------------------------------------------*/
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

