/**-----------------------------------------------------------------------------------------------
*\date  04.05.2022
*\brief 
*\authors ScuratovaAnna + PivnevNikolay
* подсмотренно
* https://www.youtube.com/watch?v=RuUFxePFrmo
* https://vk.com/solderingiron.stm32
* Ссылка на Telegram канал: https://t.me/nuvoton_programming
*/
#include <stdio.h>
#include "NuMicro.h"
#define PLL_CLOCK           96000000
uint8_t sign_number;
int integer_number;
uint32_t  fractional_number;
char Float_buffer[20] = {0,};
float Val = 3.10010202;
int8_t item;

void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number);

void SYS_Init(void)
{
  SYS_UnlockReg();
  CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
  CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
  CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
  CLK_SetCoreClock(PLL_CLOCK);
  CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1;
  SystemCoreClockUpdate();
  SYS_LockReg();
}


int32_t main()
{
  SYS_Init();
  Float_transform(Val, 6, &sign_number, &integer_number, &fractional_number);//шесть знаков после запятой
  printf("Val = %d.%.06ld\r\n", integer_number, fractional_number);//выводим шесть знаков после запятой
  Float_transform(Val, 4, &sign_number, &integer_number, &fractional_number);//четыре знака после запятой
  printf("Val = %d.%.04ld\r\n", integer_number, fractional_number);//выводим четыре знака после запятой
  Float_transform(Val, 1, &sign_number, &integer_number, &fractional_number);//один знак после запятой
  printf("Val = %d.%.01ld\r\n", integer_number, fractional_number);//выводим один знак после запятой 
  printf("\nStart SEMIHOST test: \n");
  while(1)
  {     
  item = getchar();
  printf("%c\n",item);     
  }
}

void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number) {
  uint8_t i;
  uint32_t rounding_2 = 1;
  float rounding = 0.5f;
  *sign_number = 0;
  if (*((uint32_t*) &value) & 0x80000000) {
  *((uint32_t*) &value) &= 0x7FFFFFFF;
  *sign_number = 1;
  }
    for (i = 0; i < width; i++) {
    rounding_2 = rounding_2 * 10;
  }
  rounding = rounding / rounding_2;
  *integer_number = (int) ((float) value + rounding);
  *fractional_number = (((float) value + rounding) * rounding_2) - (*integer_number * rounding_2);
}
