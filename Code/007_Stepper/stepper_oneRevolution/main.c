#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include <math.h>
#include "NuMicro.h"
#include "system_init.h"
uint32_t HCLK;
//---------------------------------------------------------------//
const int stepsPerRevolution = 2048;
 int number_of_steps;
int step_number;
unsigned long last_step_time;//отметка времени, когда был сделан последний шаг
unsigned long step_delay;
unsigned long now;
unsigned long last_step_time = 0; // отметка времени, когда был сделан последний шаг
//---------------------------------------------------------------//
void stepMotor(uint8_t thisStep);
void setSpeed(long whatSpeed);
void step(int steps_to_move);
void micros();

int main()
{
    SYS_Init();
    UART0_Init();
    HCLK = CLK_GetHCLKFreq();
    printf("HCLK %u Hz\n", HCLK);
//---------------------------------------------------------------//
    GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3), GPIO_MODE_OUTPUT);
    setSpeed(160);//max180 norm170-160
    while(1){
      step(300);
      CLK_SysTickDelay(349525);
      step(-300);
      CLK_SysTickDelay(349525);
    }
}

void stepMotor(uint8_t thisStep){
    switch (thisStep) {
      case 0:  // 1010
      PA0=1;PA1=0;PA2=1;PA3=0;
      break;
      case 1:  // 0110
      PA0=0;PA1=1;PA2=1;PA3=0;
      break;
      case 2:  //0101
      PA0=0;PA1=1;PA2=0;PA3=1;
      break;
      case 3:  //1001
      PA0=1;PA1=0;PA2=0;PA3=1;
      break;
    }
}
void setSpeed(long whatSpeed)
{
 number_of_steps=stepsPerRevolution;
 step_delay = 60L * 1000L * 1000L / number_of_steps / whatSpeed;
 //step_delay = 60*1000*1000/2048/11 =2663
}


void step(int steps_to_move){
    int steps_left = abs(steps_to_move);  // how many steps to take
    // определить направление на основе того, является ли steps_to_mode + или -
    bool direction;
    if (steps_to_move > 0) { direction = true; }
    if (steps_to_move < 0) { direction = false; }
    printf(" %u \n", direction);
     // уменьшать количество шагов, каждый раз перемещаясь на один шаг:
    while (steps_left > 0)
    {
    micros();
    // двигаться, только если прошла соответствующая задержка:
    if (now - last_step_time >= step_delay)
    {
     // get the timeStamp of when you stepped:
     last_step_time = now;
     // increment or decrement the step number,
     // depending on direction:
     if (direction)
    {
      //step_number++;
      if (step_number == number_of_steps) {
      step_number = 0;
    }
      step_number++;
    }
     else
    {
     //step_number--;
     if (step_number == 0) {
     step_number = number_of_steps;
    }
    step_number--;
    }
     // decrement the steps left:
     steps_left--;
     // step the motor to step number 0, 1, ..., {3 or 10}
     //stepMotor(step_number % 4);//& 0b11
     stepMotor(step_number & 0b11);
     printf(" %d \n", step_number);
    }
}
}
void micros(){
  TIMER_Delay(TIMER0, 1000000);
  now ++;
}
