#ifndef _System_init_H
#define _System_init_H

#define PLL_CLOCK           192000000

typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

//#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define max(a,b) ((a)>(b)?(a):(b))
//#define min(a,b) ((a)<(b)?(a):(b))
//https://robocraft.ru/arduino/38

void SYS_Init(void);
//void UART0_Init(void);

#endif
