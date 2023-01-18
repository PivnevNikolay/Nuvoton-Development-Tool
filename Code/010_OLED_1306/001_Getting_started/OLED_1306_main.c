/**-----------------------------------------------------------------------------------------------
*\date  19.01.2023
*\brief
*\authors ScuratovaAnna + PivnevNikolay
*\M031FC1Ae 32 KB Flash 4 KB SRAM
*/
//------------------------DrawPixel----------------------------------------------------------//
#include <stdio.h>
#include "NuMicro.h"
#include "stdbool.h"
#include "system_init.h"
#include "string.h"
//-------------------------------------------------------------------------------------------//
#define DISPLAY_ADDRESS 0x3C
//-------------------------------------------------------------------------------------------//
#define BLACK 0
#define WHITE 1
#define INVERSE 2
//-------------------------------------------------------------------------------------------//
#define DISPLAYWIDTH 128
#define DISPLAYHEIGHT 64
//-------------------------------------------------------------------------------------------//
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
//-------------------------------------------------------------------------------------------//
static u8 _i2caddr;
//-------------------------------------------------------------------------------------------//
static u8 SSD1306_buffer[DISPLAYHEIGHT * DISPLAYWIDTH / 8 + 1] = { 0x40 };
static u8* _displaybuf = SSD1306_buffer+1;
static u16 _displaybuf_size = sizeof(SSD1306_buffer) - 1;
//-------------------------------------------------------------------------------------------//
#define GDDRAM_ADDRESS(X,Y) ((_displaybuf)+((Y)/8)*(DISPLAYWIDTH)+(X))
#define GDDRAM_PIXMASK(Y) (1 << ((Y)&0x07))
#define PIXEL_ON(X,Y) (*GDDRAM_ADDRESS(x,y) |= GDDRAM_PIXMASK(y))
#define PIXEL_OFF(X,Y) (*GDDRAM_ADDRESS(x,y) &= ~GDDRAM_PIXMASK(y))
#define PIXEL_TOGGLE(X,Y) (*GDDRAM_ADDRESS(x,y) ^= GDDRAM_PIXMASK(y))
//-------------------------------------------------------------------------------------------//
int16_t WIDTH, HEIGHT;
static int16_t _width, _height;
static int16_t cursor_x, cursor_y;
static uint16_t textcolor, textbgcolor;
static uint8_t textsize;
u8 rotation;
u8 wrap;
//-------------------------------------------------------------------------------------------//
void display_write_buf( u8* buf, uint16_t size );
void display_init( u8 i2caddr );
void gfx_init( int16_t width, int16_t height );
void display_update(void);
void display_setPixel( int16_t x, int16_t y, uint16_t color );
void gfx_drawPixel( int16_t x, int16_t y, uint16_t color );
void gfx_rotation_adjust( int16_t* px, int16_t* py );
void display_clear(void);
//-------------------------------------------------------------------------------------------//
int main()
{
   SYS_Init();
   UART_Open(UART0, 115200);
   I2C_Open(I2C0, 100000);
   display_init(DISPLAY_ADDRESS);
   display_clear();
   display_update();
//------------------------------drawing a pixel----------------------------------------------//  
   gfx_drawPixel(0, 0, WHITE);
   gfx_drawPixel(1, 1, WHITE);
   gfx_drawPixel(2, 2, WHITE);
   gfx_drawPixel(3, 3, WHITE);
   gfx_drawPixel(4, 4, WHITE);
   gfx_drawPixel(5, 5, WHITE);
   gfx_drawPixel(6, 6, WHITE);
   gfx_drawPixel(7, 7, WHITE);
   gfx_drawPixel(8, 8, WHITE);
   gfx_drawPixel(9, 9, WHITE);
   gfx_drawPixel(10, 10, WHITE);
//------------------------------drawing a pixel----------------------------------------------//    
   display_update();
   while(1){
   }
 }
//-------------------------------------------------------------------------------------------//
void gfx_init( int16_t width, int16_t height ){
    WIDTH = width;
    HEIGHT = height;
    _width = WIDTH;
    _height = HEIGHT;
    rotation = 0;
    cursor_y = cursor_x = 0;
    textsize = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap = 1;
}
//-------------------------------------------------------------------------------------------//
void display_init( u8 i2caddr ){
 _i2caddr = i2caddr;
 gfx_init( DISPLAYWIDTH, DISPLAYHEIGHT );
 u8 cmdbuf[] = {
   0x00,
   SSD1306_DISPLAYOFF,
   SSD1306_SETDISPLAYCLOCKDIV,
   0x80,
   SSD1306_SETMULTIPLEX,
   0x3f,
   SSD1306_SETDISPLAYOFFSET,
   0x00,
   SSD1306_SETSTARTLINE | 0x0,
   SSD1306_CHARGEPUMP,
   0x14,
   SSD1306_MEMORYMODE,
   0x00,
   SSD1306_SEGREMAP | 0x1,
   SSD1306_COMSCANDEC,
   SSD1306_SETCOMPINS,
   0x12,
   SSD1306_SETCONTRAST,
   0xcf,
   SSD1306_SETPRECHARGE,
   0xf1,
   SSD1306_SETVCOMDETECT,
   0x40,
   SSD1306_DISPLAYALLON_RESUME,
   SSD1306_NORMALDISPLAY,
   SSD1306_DISPLAYON
 };
  I2C_WriteMultiBytes(I2C0, DISPLAY_ADDRESS, cmdbuf, sizeof(cmdbuf));
}
//-------------------------------------------------------------------------------------------//
void display_write_buf( u8* buf, uint16_t size ){
  I2C_WriteMultiBytes(I2C0, DISPLAY_ADDRESS, buf, size);
}
//-------------------------------------------------------------------------------------------//
void display_update(void) {
  u8 cmdbuf[] = {
    0x00,
    SSD1306_COLUMNADDR,
    0,
    DISPLAYWIDTH-1,
    SSD1306_PAGEADDR,
    0,
    7
  };
  display_write_buf( cmdbuf, sizeof(cmdbuf) );
  display_write_buf( SSD1306_buffer, sizeof(SSD1306_buffer) );
 }
//-------------------------------------------------------------------------------------------//
void display_setPixel( int16_t x, int16_t y, uint16_t color ){

    if( (x < 0) || (x >= DISPLAYWIDTH) || (y < 0) || (y >= DISPLAYHEIGHT) )
        return;
    switch( color ){
        case WHITE:
            PIXEL_ON(x,y);
            break;
        case BLACK:
            PIXEL_OFF(x,y);
            break;
        case INVERSE:
            PIXEL_TOGGLE(x,y);
            break;
    }
}
//-------------------------------------------------------------------------------------------//
void gfx_drawPixel( int16_t x, int16_t y, uint16_t color ){
  if( (x < 0) || (x >= _width) || (y < 0) || (y >= _height) )
         return;
  gfx_rotation_adjust( &x, &y );
  display_setPixel(x,y,color);
 }
//-------------------------------------------------------------------------------------------//
void gfx_rotation_adjust( int16_t* px, int16_t* py ){
    int16_t y0 = *py;
    switch( rotation ){
        case 1:
            *py = *px;
            *px = WIDTH - y0 - 1;
            break;
        case 2:
            *px = WIDTH - *px - 1;
            *py = HEIGHT - *py - 1;
            break;
        case 3:
            *py = HEIGHT - *px - 1;
            *px = y0;
            break;
    }
}
//-------------------------------------------------------------------------------------------//
void display_clear(void){
  memset( _displaybuf, 0, _displaybuf_size );
  SSD1306_buffer[0] = 0x40;
 }
//-------------------------------------------------------------------------------------------//