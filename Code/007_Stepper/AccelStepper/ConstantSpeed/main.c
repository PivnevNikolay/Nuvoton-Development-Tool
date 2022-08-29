#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include "NuMicro.h"
#include "system_init.h"
#include "stepper.h"

uint32_t HCLK;

int main()
{
    SYS_Init();
    UART0_Init();
    HCLK = CLK_GetHCLKFreq();
    printf("HCLK %u Hz\n", HCLK);
    GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3), GPIO_MODE_OUTPUT);
    AccelStepper_stepper(FULL4WIRE);
    setMaxSpeed(6000);
    //setSpeed(5500);
    setSpeed(4000);
    //setSpeed(100);
    while(1){
    runSpeed();
    }
}
