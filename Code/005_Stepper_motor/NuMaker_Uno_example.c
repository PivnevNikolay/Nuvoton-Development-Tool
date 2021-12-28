/**************************************************************************//**
* что надо почитать, желательно...
* https://wiki.iarduino.ru/page/shagovye-dvigateli/
* Модель: 28BYJ-48
* Тип двигателя: 4-х фазный униполярный
* Режим управления: полношаговый и полушаговый
* Напряжение питания: 5 В в униполярном и 10 В в биполярном режиме
* Номинальный ток фазы: 160 мА
* Номинальное сопротивление обмоток: 50 Ом
* Количество шагов внутреннего вала на оборот: 32
* Передача встроенного редуктора: 1:64
* Количество шагов внешнего вала на оборот: 2048
* Шаг в градусах: 0,18°
* Максимальное количество шагов в секунду: 500
* Время полного оборота: 4,096 c
* Крутящий момент: 0,3 кгx см
* Диаметр вала: 5 мм
* Длина вала: 8 мм
* Габариты корпуса: 25 x 18 мм
* Вес: 40 г
* https://amperka.ru/product/stepper-motor-28byj-48-5v
* Полушаговый режим: 8-шаговая управляющая сигнальная последовательность (рекомендовано). 5.625 градусов на шаг, 64 шага на оборот внутреннего вала мотора. 
* Режим полного шага: 4-шаговая управляющая сигнальная последовательность. 11.25 градус/шаг, 32 шага на оборот внутреннего вала двигателя
 ******************************************************************************/
 
#include <stdio.h>
#include "NUC131.h"

#define PLL_CLOCK           50000000
//----------------------------------------------------------------//
#define d360 (4096)
// откуда берется эта цифра минут 10 тупил --> всё просто двигатель имеет редуктор 1/64
// делее используем полушаговый режим ...  
// 32*2*64=4096 это полный оборот двигателя 360 градусов
#define d180 (4096/2)// 180 градусов
#define d90 (4096/4)// 90 градусов
#define d45 (4096/8)// 45 градусов
#define Motor_Step 8// количество шагов
//----------------------------------------------------------------//
unsigned char CW[8] = {0x08,0x0C,0x04,0x06,0x02,0x03,0x01,0x09};// массив для вращения по часовой стрелки
unsigned char CCW[8] = {0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};// массив для вращения против часовой стрелки
unsigned char Dir_flag=0;// флаг для обработчика прерывания 
volatile unsigned int  Motor_Count=0;// счетчик для прерывания
//----------------------------------------------------------------//
void Rotation_MOTOR(unsigned int deg, unsigned char motor_dir);
//----------------------------------------------------------------//
typedef enum
{
    clockwise=1, counterclockwise=!clockwise
}Motor_direction;
//----------------------------------------------------------------//
void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1) {
        /* The motor is turning a circle of clockwise! */
        if(Dir_flag)   
        {
            /* calculation the step of motor */
            if(Motor_Count%Motor_Step==0)
            PA->DOUT=CW[0];
            else
            PA->DOUT=CW[Motor_Step-(Motor_Count%Motor_Step)];
        }
        else
        /* The motor is turning a circle of counterclockwise! */
        {
            /* calculation the step of motor */
            if(Motor_Count%Motor_Step==0)
            PA->DOUT=CCW[0];
            else
            PA->DOUT=CCW[Motor_Step-(Motor_Count%Motor_Step)];
        }
        Motor_Count--;
        if(Motor_Count==0) 
        {
            TIMER_Stop(TIMER0);
        }
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);
    }
}
//----------------------------------------------------------------//
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCON_IRC22M_EN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable HXT */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL and SysTick source to HCLK/2*/
    CLK_SetCoreClock(PLL_CLOCK);
    CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2);

    /* Enable peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(TMR0_MODULE);


    /* Peripheral clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_PLL, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, 0);


    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD, TXD */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);
}

void UART0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset IP */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{


    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    printf("CPU @ %d Hz\n", SystemCoreClock);


    /* Open Timer0 in periodic mode, enable interrupt  */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 200);
    TIMER_EnableInt(TIMER0);

   

    /* Enable Timer0 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);

    /* Configure PA.0 PA.1 PA.2 PA.3 set Push-pull mode, motor control IO */
    GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3), GPIO_PMD_OUTPUT);
    printf("\n===============================================\n");
    printf("The motor is turning a circle of counterclockwise!\n");
    Rotation_MOTOR(d360, counterclockwise);
    printf("done!\n");
    printf("\n===============================================\n");
    printf("The motor is turning a circle of clockwise!\n");
    Rotation_MOTOR(d360, clockwise);
    printf("done!\n");

    while(1);
}

void Rotation_MOTOR(unsigned int deg, unsigned char motor_dir)
{
    Motor_Count = deg;
    Dir_flag = motor_dir;
    TIMER_Start(TIMER0);// Start Timer 0
	while(Motor_Count);
}
