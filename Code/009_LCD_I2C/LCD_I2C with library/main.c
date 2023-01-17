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
#include "i2c_wire.h"


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
   begin();
   CLK_SysTickDelay(500);
   Init_LCD();
   Lcd_SetPos(5,0);
   LCD_string("NUVOTON");
   Lcd_SetPos(0,1);
   LCD_string("MCU  NUC131SD2AE");
   CLK_SysTickDelay(20000);//delay(2000);
   Shift_the_entire_display_to_the_left(5);
   CLK_SysTickDelay(10000);//delay(1000);
   Shift_the_entire_display_to_the_right(5);
   CLK_SysTickDelay(10000);//delay(1000);
   Turn_display_off();
   Turn_on_the_screen_LCD_cursor_off();
   CLK_SysTickDelay(10000);//delay(1000);
   Cleaning_LCD();
   LCD_string_output_with_coordinates("MCU M031FC1AE",2,0);
   LCD_string_output_with_coordinates("LCD OUT Nuvoton",2,1);
   LCD_string_output_with_coordinates("I did it all right",1,2);
   I2C0_Close();

    while(1);

}

/******************************************************************************
*                  Функция инициализации LCD                                  *
*******************************************************************************/
void Init_LCD(void) {
 // 8-bit mode
 LCD_command_8bit(0x30);
 CLK_SysTickDelay(100);//delay(10);
 LCD_command_8bit(0x30);
 CLK_SysTickDelay(60);//delay(6);
 LCD_command_8bit(0x30);
 LCD_command_8bit(0x20); // changes to 4-bit mode
 // 4-bit mode
 LCD_command_nibble(0x28); // function set(4-bit, 2 line, 5x7 dot)
 LCD_command_nibble(0x0C); // display control(display ON, cursor OFF)
 LCD_command_nibble(0x06); // entry mode set(increment, not shift)
 LCD_command_nibble(0x01); // clear display
 CLK_SysTickDelay(40);//delay(4);
}
/******************************************************************************
*                  Функция записи команды в LCD                               *
*******************************************************************************/
void LCD_command_8bit(u8 val)
{
  beginTransmission( LCD_ADDRESS );
  val = (val & 0xF0) | 0x0C;
  write(val);
  val = val & E0;// E = 0;
  write(val);
  endTransmission();
}

void LCD_command_nibble(u8 val)
{
 u8 alfa = val;
 beginTransmission( LCD_ADDRESS );
 val = (val & 0xF0 ) | 0x0C;
 write(val);
 val = val & E0;// E = 0;
 write(val);
 val = (alfa << 4) | 0x0C;
 write(val);
 val = val & E0;// E = 0;
 write(val);
 endTransmission();
}

void LCD_send_byte(u8 val)
{
 u8 zero = val;
 beginTransmission( LCD_ADDRESS );
 val = (val & 0xF0) | Back_Light_ON;  // high 4 bit, E = 1, RW = 0, RS = 1
 write(val);
 val = val & E0;  // E = 0;
 write(val);
 val = (zero << 4 ) | Back_Light_ON;  // high  4 bit, E = 1, RW = 0, RS = 1
 write(val);
 val = val & E0;  // E = 0;
 write(val);
 endTransmission();
}
/******************************************************************************
*                   Функция вывода строки на экран LCD                        *
*******************************************************************************/
void LCD_string(const char *s){
 while(*s){
  LCD_send_byte(*s++);
 }
}
/******************************************************************************
*                   Функция установки позиции курсора                         *
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
*******************************************************************************/
void Shift_the_entire_display_to_the_left(u8 n)
{
  for (uint8_t i=0;i<n;i++)
  {
    LCD_command_nibble(0x18);
    CLK_SysTickDelay(10000);//delay(1000);
  }
}
/******************************************************************************
*                  Функция смещения экрана LCD вправо на n символов           *
*******************************************************************************/
void Shift_the_entire_display_to_the_right(u8 n)
{
  for (uint8_t i=0;i<n;i++)
  {
    LCD_command_nibble(0x1C);
    CLK_SysTickDelay(10000);//delay(1000);
  }
}
/******************************************************************************
*                  Функция выключения экрана LCD                              *
*******************************************************************************/
void Turn_display_off(void)
{
  LCD_command_nibble(0x8);
  CLK_SysTickDelay(10000);//delay(1000);
}
/******************************************************************************
*                  Функция включения экрана LCD, выключение курсора           *
*******************************************************************************/
void Turn_on_the_screen_LCD_cursor_off(void)
{
  LCD_command_nibble(0xE);
  CLK_SysTickDelay(10000);//delay(1000);
}
/******************************************************************************
*                  Функция очистки экрана LCD                                 *
*******************************************************************************/
void Cleaning_LCD(void){
  LCD_command_nibble(0x01);
  CLK_SysTickDelay(40);//delay(4);
}
/******************************************************************************
*         Функция вывода строка на экран LCD с заданными координатами         *
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
