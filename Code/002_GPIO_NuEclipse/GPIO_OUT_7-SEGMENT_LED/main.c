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
//------------------------------------1----------------------------------//
#include <stdio.h>
#include "NuMicro.h"
#include "system_init.h"
///               0    1   2    3    4    5    6    7   8    9
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
//-----------------------------------------------------------------------//
int main()
{
  SYS_Init();
  UART0_Init();
  GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ), GPIO_MODE_OUTPUT);
  printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
//-----------------------------------------------------------------------//
  while(1){
   for ( uint8_t z =0;z<10;z++){
   //PA->DOUT = led[z];
   PA->DOUT = *(led+z);
   printf("z = %u \n",z);
   CLK_SysTickDelay(349525);
  }
 }
}
//-------------------------------------END-------------------------------//
//---------------------------------------2-------------------------------//
#include <stdio.h>
#include "NuMicro.h"
#include "system_init.h"
int main()
{
  SYS_Init();
  UART0_Init();
  PA->MODE = (PA->MODE & ~(GPIO_MODE_MODE0_Msk | GPIO_MODE_MODE1_Msk | GPIO_MODE_MODE2_Msk |
	 GPIO_MODE_MODE3_Msk | GPIO_MODE_MODE4_Msk | GPIO_MODE_MODE5_Msk | GPIO_MODE_MODE6_Msk)) |
	(GPIO_MODE_OUTPUT << GPIO_MODE_MODE0_Pos | GPIO_MODE_OUTPUT << GPIO_MODE_MODE1_Pos | GPIO_MODE_OUTPUT << GPIO_MODE_MODE2_Pos |
	 GPIO_MODE_OUTPUT << GPIO_MODE_MODE3_Pos | GPIO_MODE_OUTPUT << GPIO_MODE_MODE4_Pos | GPIO_MODE_OUTPUT << GPIO_MODE_MODE5_Pos |
	 GPIO_MODE_OUTPUT << GPIO_MODE_MODE6_Pos);
  printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
//---------------------------------------//
  while(1){
   for ( uint8_t z =0;z<5;z++){
   switch(z){
       case 0:
    PA->DOUT &= ~((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3)|(1 << 4)|(1 << 5)|(1<<6));
    PA->DOUT|= (1 << 0)|(1 <<1)|(1 << 2)|(1 << 3)|(1 << 4)|(1 <<5);//0
    CLK_SysTickDelay(349525);
       break;
       case 1:
    PA->DOUT &= ~((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3)|(1 << 4)|(1 << 5)|(1<<6));
    PA->DOUT |= (1 <<1)|(1 << 2);//1
    CLK_SysTickDelay(349525);
       break;
       case 2:
    PA->DOUT &= ~((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3)|(1 << 4)|(1 << 5)|(1<<6));
    PA->DOUT |= (1 << 0)|(1 <<1)|(1 << 3)|(1 << 4)|(1<<6);//2
    CLK_SysTickDelay(349525);
       break;
       case 3:
    PA->DOUT &= ~((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3)|(1 << 4)|(1 << 5)|(1<<6));
    PA->DOUT  |= (1 << 0)|(1 <<1)|(1 << 2)|(1 << 3)| (1<<6);//3
    CLK_SysTickDelay(349525);
       break;
       case 4:
    PA->DOUT &= ~((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3)|(1 << 4)|(1 << 5)|(1<<6));
    PA->DOUT |= (1 << 1)|(1 <<2)|(1 << 5)|(1<<6);//4
    CLK_SysTickDelay(349525);
       break;
     }
  }
 }
}
//-------------------------------------END-------------------------------//