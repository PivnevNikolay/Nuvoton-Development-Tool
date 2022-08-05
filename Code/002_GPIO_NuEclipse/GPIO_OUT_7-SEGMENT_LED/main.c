/**---------------------------------------------
*\date 
*\brief 
*      1        1->PA.0
*     ---       2->PA.1
*  6 | 7 |2     3->PA.2
*     ---       4->PA.3
*  5 | 4 |3     5->PA.4
*     ---       6->PA.5
*               7->PA.6
*\authors ScuratovaAnna + PivnevNikolay
*/
#include <stdio.h>
#include "NuMicro.h"
#include "system_init.h"
///               0    1   2    3    4    5    6    7   8    9
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
//---------------------------------------//
int main()
{
  SYS_Init();
  UART0_Init();
  GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ), GPIO_MODE_OUTPUT);
  printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
//---------------------------------------//
  while(1){
   for ( uint8_t z =0;z<10;z++){
   PA->DOUT = led[z];
   printf("z = %u \n",z);
   CLK_SysTickDelay(349525);
  }
 }
}