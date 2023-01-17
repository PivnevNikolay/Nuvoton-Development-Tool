/**-----------------------------------------------------------------------------------------------
*\date  18.01.2023
*\brief
*\authors ScuratovaAnna + PivnevNikolay
* M031FC1Ae 32 KB Flash 4 KB SRAM
*/

#include <stdio.h>
#include "NuMicro.h"
#include "stdbool.h"
#include "system_init.h"

#define LCD_ADDRESS     0x27
#define E0              0xFB
#define Back_Light_OFF  0x5 // low  4 bit, E = 1, RW = 0, RS = 1
#define Back_Light_ON   0xD // high 4 bit, E = 1, RW = 0, RS = 1

void Init_LCD(void);
void LCD_command_8bit(u8 val);
void LCD_command_nibble(u8 val);
void LCD_send_byte(u8 val);
void LCD_string(const char *s);
void Lcd_SetPos(u8 x, u8 y);
void Shift_the_entire_display_to_the_left(u8 n);
void Shift_the_entire_display_to_the_right(u8 n);
void Turn_display_off(void);
void Turn_on_the_screen_LCD_cursor_off(void);
void Cleaning_LCD(void);
void LCD_string_output_with_coordinates(char *p,u8 x,u8 y);

int main()
{
   SYS_Init();
   UART_Open(UART0, 115200);
   I2C_Open(I2C0, 100000);
   Init_LCD();

while(1){
   Lcd_SetPos(5,0);
   LCD_string("NUVOTON");
   Lcd_SetPos(5,1);
   LCD_string("NuEclipse IDE");
   TIMER_Delay(TIMER0, 1000000);
   Shift_the_entire_display_to_the_left(3);
   Shift_the_entire_display_to_the_right(3);
   Cleaning_LCD();
   TIMER_Delay(TIMER0, 1000000);
   LCD_string_output_with_coordinates("MCU M031FC1AE",2,0);
   LCD_string_output_with_coordinates("Arm Cortex-M0",2,1);
   LCD_string_output_with_coordinates("LCD OUT Nuvoton",2,2);
   LCD_string_output_with_coordinates("I did it all right",2,3);
   TIMER_Delay(TIMER0, 1000000);
   Shift_the_entire_display_to_the_left(2);
   Shift_the_entire_display_to_the_right(2);
   TIMER_Delay(TIMER0, 1000000);
   Cleaning_LCD();
  }
}

/******************************************************************************
*                  Функция инициализации LCD                                  *
*                  LCD initialization function                                *
*******************************************************************************/
void Init_LCD(void) {
 // 8-bit mode
 LCD_command_8bit(0x30);
 TIMER_Delay(TIMER0, 4500);//delay
 LCD_command_8bit(0x30);
 TIMER_Delay(TIMER0, 4500);//delay
 LCD_command_8bit(0x30);
 TIMER_Delay(TIMER0, 4500);//delay
 LCD_command_8bit(0x20); // changes to 4-bit mode
 TIMER_Delay(TIMER0, 150);//delay
 // 4-bit mode
 LCD_command_nibble(0x28); // function set(4-bit, 2 line, 5x7 dot)
 LCD_command_nibble(0x0C); // display control(display ON, cursor OFF)
 LCD_command_nibble(0x06); // entry mode set(increment, not shift)
 LCD_command_nibble(0x01); // clear display
 TIMER_Delay(TIMER0, 150);//delay//delay(4);
}
/******************************************************************************
*                  Функция записи команды в LCD                               *
*                  Command recording function in LCD                          *
*******************************************************************************/
void LCD_command_8bit(u8 val)
{
  val = (val & 0xF0) | 0x0C;
  I2C_WriteByte(I2C0, LCD_ADDRESS, val);
  val = val & E0;// E = 0;
  I2C_WriteByte(I2C0, LCD_ADDRESS, val);
}

void LCD_command_nibble(u8 val)
{
 u8 alfa = val;
 val = (val & 0xF0 ) | 0x0C;
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
 val = val & E0;// E = 0;
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
 val = (alfa << 4) | 0x0C;
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
 val = val & E0;// E = 0;
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
}

void LCD_send_byte(u8 val)
{
 u8 zero = val;
 val = (val & 0xF0) | Back_Light_ON;  // high 4 bit, E = 1, RW = 0, RS = 1
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
 val = val & E0;  // E = 0;
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
 val = (zero << 4 ) | Back_Light_ON;  // high  4 bit, E = 1, RW = 0, RS = 1
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
 val = val & E0;  // E = 0;
 I2C_WriteByte(I2C0, LCD_ADDRESS, val);
}
/******************************************************************************
*                   Функция вывода строки на экран LCD                        *
*                   Line output function on the LCD screen                    *
*******************************************************************************/
void LCD_string(const char *s){
 while(*s){
  LCD_send_byte(*s++);
 }
}
/******************************************************************************
*                   Функция установки позиции курсора                         *
*                   Cursor position setting function                          *
*******************************************************************************/
void Lcd_SetPos(u8 x, u8 y)
 {
 switch(y)
  {
  case 0:
   LCD_command_nibble(x|0x80);
  break;
  case 1:
   LCD_command_nibble((0x40+x)|0x80);
  break;
  case 2:
   LCD_command_nibble((0x14+x)|0x80);
  break;
  case 3:
   LCD_command_nibble((0x54+x)|0x80);
  break;
 }
}
/******************************************************************************
*                   Функция смещения экрана LCD влево на n символов           *
*  The function of shifting the LCD screen to the left by n characters        *
*******************************************************************************/
void Shift_the_entire_display_to_the_left(u8 n)
{
  for (uint8_t i=0;i<n;i++)
  {
    LCD_command_nibble(0x18);
    TIMER_Delay(TIMER0,1000000);
  }
}
/******************************************************************************
*                  Функция смещения экрана LCD вправо на n символов           *
*     The function of shifting the LCD screen to the right by n characters    *
*******************************************************************************/
void Shift_the_entire_display_to_the_right(u8 n)
{
  for (uint8_t i=0;i<n;i++)
  {
    LCD_command_nibble(0x1C);
    TIMER_Delay(TIMER0,1000000);
  }
}
/******************************************************************************
*                  Функция выключения экрана LCD                              *
*                  LCD screen off function                                    *
*******************************************************************************/
void Turn_display_off(void)
{
  LCD_command_nibble(0x8);
  TIMER_Delay(TIMER0, 1000000);
}
/******************************************************************************
*                  Функция включения экрана LCD, выключение курсора           *
*    The function of turning on the LCD screen, turning off the cursor        *
*******************************************************************************/
void Turn_on_the_screen_LCD_cursor_off(void)
{
  LCD_command_nibble(0xE);
  TIMER_Delay(TIMER0, 1000000);
}
/******************************************************************************
*                  Функция очистки экрана LCD                                 *
*                  LCD screen cleaning function                               *
*******************************************************************************/
void Cleaning_LCD(void){
  LCD_command_nibble(0x01);
  TIMER_Delay(TIMER0, 100);
}
/******************************************************************************
*         Функция вывода строка на экран LCD с заданными координатами         *
*         The function of displaying a string on the LCD screen with          *
*         the specified coordinates                                           *
*******************************************************************************/
//Выводим строку на экран LCD с заданными координатами
void LCD_string_output_with_coordinates(char *p,u8 x,u8 y) //Вывести строку на дисплей X,Y
///lcd_string("HI BABY",4,1) ; выведется сторка по конкретным координатам
{
 Lcd_SetPos(x, y);
  while( *p )
  {
    LCD_send_byte(*p++);
  }
}
//************************************END**************************************/
