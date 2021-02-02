/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Demonstrate how to use GPIO pins to control stepper motor forward and reverse
 *
 * @copyright (C) 2013~2015 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include "stdio.h"
#include "NuMicro.h"


#define PLL_CLOCK       192000000

#define d360 (4096)
#define d180 (4096/2)
#define d90 (4096/4)
#define d45 (4096/8)
#define Motor_Step 8

unsigned char CW[8] = {0x08,0x0C,0x04,0x06,0x02,0x03,0x01,0x09}; 
unsigned char CCW[8] = {0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};
unsigned char Dir_flag=0;
volatile unsigned int  Motor_Count=0;

typedef enum
{
    clockwise=1, counterclockwise=!clockwise
}Motor_direction;

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

void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Unlock protected registers */
    SYS_UnlockReg();
    
    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Enable TIMER module clock */
    CLK_EnableModuleClock(TMR0_MODULE);
    
    /* Select TIMER module clock source as HXT */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0);
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();
    
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PD multi-function pins for UART0 RXD(PD.2) and TXD(PD.3) */
    SYS->GPD_MFPL &= ~(SYS_GPD_MFPL_PD2MFP_Msk | SYS_GPD_MFPL_PD3MFP_Msk);
    SYS->GPD_MFPL |= (SYS_GPD_MFPL_PD2MFP_UART0_RXD | SYS_GPD_MFPL_PD3MFP_UART0_TXD);
    
    /* Lock protected registers */
    SYS_LockReg();
}

void Rotation_MOTOR(unsigned int deg, unsigned char motor_dir)
{
    while(Motor_Count);
    Motor_Count = deg;
    Dir_flag = motor_dir;
    // Start Timer 0
    TIMER_Start(TIMER0);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */
    UART_Open(UART0, 115200);

    /* Open Timer0 in periodic mode, set timer frequency to 200HZ */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 200);
    
    /* Enable timer interrupt */
    TIMER_EnableInt(TIMER0);
    
    /* Enable Timer0 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);
    
    /* Configure PA.0 PA.1 PA.2 PA.3 set open drain mode, motor control IO */
    GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3), GPIO_MODE_OPEN_DRAIN);
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
