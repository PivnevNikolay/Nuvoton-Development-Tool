
#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include <math.h>
#include "NuMicro.h"
#include "system_init.h"

u8 sign_number;
int integer_number;
u32  fractional_number;
char Float_buffer[20] = {0,};

float _c = 242.0;
float _a = 2.0;
float result = 0.0;
float Val = 3.10010202;
u8 item;

int main()
{
    SYS_Init();
    UART0_Init();
    result = sqrt(81.0/9.0);
    Float_transform(result, 1, &sign_number, &integer_number, &fractional_number);
    printf("result = %d.%.01ld\r\n", integer_number, fractional_number);
    result = sqrt(_c * _a);
    Float_transform(result, 1, &sign_number, &integer_number, &fractional_number);
    printf("result = %d.%.01ld\r\n", integer_number, fractional_number);
    Float_transform(Val, 6, &sign_number, &integer_number, &fractional_number);//шесть знаков после запятой
    printf("Val = %d.%.06ld\r\n", integer_number, fractional_number);//выводим шесть знаков после запятой
    Float_transform(Val, 4, &sign_number, &integer_number, &fractional_number);//четыре знака после запятой
    printf("Val = %d.%.04ld\r\n", integer_number, fractional_number);//выводим четыре знака после запятой
    Float_transform(Val, 1, &sign_number, &integer_number, &fractional_number);//один знак после запятой
    printf("Val = %d.%.01ld\r\n", integer_number, fractional_number);//выводим один знак после запятой
    printf("\nStart SEMIHOST test: \n");
    while(1){
    item = getchar();
    printf("%c\n",item);
    }
}

/*
result = 3.0
result = 22.0
Val = 3.100102
Val = 3.1001
Val = 3.1

Start SEMIHOST test:
1
2
3
*/
