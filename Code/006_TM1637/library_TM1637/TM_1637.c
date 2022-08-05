#include "stdio.h"
#include "stdbool.h"
#include "NuMicro.h"
#include "TM_1637.h"
#define ADDR_FIXED 0x44
#define ADDR_AUTO  0x40
const uint8_t digitHEX[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x40}; //0~9
uint8_t lastData[4];
uint8_t PointData;
uint8_t Cmd_DispCtrl;
uint8_t Cmd_SetData;
uint8_t Cmd_SetAddr;
//*************************************************************************************************************
void clear(void)
{
  display(0x00, 0x7f);
  display(0x01, 0x7f);
  display(0x02, 0x7f);
  display(0x03, 0x7f);
  lastData[0] = 0x00;
  lastData[1] = 0x00;
  lastData[2] = 0x00;
  lastData[3] = 0x00;
}
//*************************************************************************************************************
void display(uint8_t BitAddr, uint8_t DispData)
{
  uint8_t SegData;
  lastData[BitAddr] = digitHEX[DispData];
  SegData = digitHEX[DispData] + PointData;
  sendByte(BitAddr, SegData);
}
//*************************************************************************************************************
void sendByte(uint8_t BitAddr, int8_t sendData) {
  start();
  writeByte(ADDR_FIXED);
  stop();
  start();
  writeByte(BitAddr | 0xc0);
  writeByte(sendData);
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}
//*************************************************************************************************************
void brightness_(uint8_t brightness, uint8_t SetData, uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;
  update();
}
//*************************************************************************************************************
int writeByte(int8_t wr_data)
{
  uint8_t i, count1;
  for (i = 0; i < 8; i++)
  {
    PC0 = 0;
    if (wr_data & 0x01) {
    PC1 = 1;
    }
    else PC1 = 0;
    wr_data >>= 1;
    PC0 = 1;
  }
  PC0 = 0;//CLK
  PC1 = 1;
  PC0 = 1;
  uint8_t ack = 1;
  GPIO_SetMode(PC, BIT1, GPIO_MODE_INPUT);
  TIMER_Delay(TIMER0, 0x32);//delay 50 microseconds
  if (PC1 == 0)
  {
    GPIO_SetMode(PC, BIT1,  GPIO_MODE_OUTPUT);
    PC1 = 0;
    ack = 0;
  }
  TIMER_Delay(TIMER0, 0x32);//delay 50 microseconds
  GPIO_SetMode(PC, BIT1,  GPIO_MODE_OUTPUT);
  TIMER_Delay(TIMER0, 0x32);//delay 50 microseconds
  return ack;
}
//*************************************************************************************************************
void update(void)
{
  displayByte(lastData);
}
//*************************************************************************************************************
void displayByte(uint8_t DispData[])
{
  uint8_t SegData[4];
  for (uint8_t i = 0; i < 4; i ++) {
    lastData[i] = DispData[i];
    SegData[i] = DispData[i] + PointData;
  }
  sendArray(SegData);
}
//*************************************************************************************************************
void sendArray(uint8_t sendData[])
{
  start();
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for (uint8_t i = 0; i < 4; i ++) {
    writeByte(sendData[i]);
  }
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}
//*************************************************************************************************************
void display_Byte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3)
{
  uint8_t dispArray[] = {bit0, bit1, bit2, bit3};
  displayByte(dispArray);
}
//*************************************************************************************************************
void start(void)
{
  PC0 = 1;
  PC1 = 1;
  PC1 = 0;
  PC0 = 0;
}
//*************************************************************************************************************
void stop(void)
{
  PC0 = 0;
  PC1 = 0;
  PC0 = 1;
  PC1 = 1;
}
//*************************************************************************************************************
void runningString(uint8_t DispData[], uint8_t amount, int delayMs) {
  uint8_t segm_data[amount + 8];
  for (uint8_t i = 0; i < 4; i++) {
  segm_data[i] = 0x00;
  }
  for (uint8_t i = 0; i < amount; i++) {
  segm_data[i + 4] = DispData[i];
  }
  for (uint8_t i = amount + 4; i < amount + 8; i++) {
  segm_data[i] = 0x00;
  }
  for (uint8_t i = 0; i <= amount + 4; i++) {
  display_Byte(segm_data[i], segm_data[i + 1], segm_data[i + 2], segm_data[i + 3]);
  TIMER_Delay(TIMER0, delayMs);
  }
}
//*************************************************************************************************************
//*********************************************scroll**********************************************************
void scroll_set_all(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms) {
	uint8_t DispData[] = {digitHEX[bit0], digitHEX[bit1], digitHEX[bit2], digitHEX[bit3]};
    scrollByte(DispData, delayms);
}


void scroll(uint8_t DispData[], int delayms) {
	uint8_t DispDataByte[4];
    for (uint8_t i = 0; i < 4; i++) {
        DispDataByte[i] = digitHEX[DispData[i]];
    }
    scrollByte(DispDataByte, delayms);
}
void scrollByte(uint8_t DispData[], int delayms) {
	int8_t lastBytes[4];
	int8_t step;
	int8_t stepArray[4];
    bool changeByte[4] = {0, 0, 0, 0};

    for (int8_t i = 0; i < 4; i++) {
        if (DispData[i] != lastData[i]) changeByte[i] = 1;
        lastBytes[i] = 	lastData[i];
    }

    for (int8_t i = 0; i < 4; i++) {
    	int8_t lastByte = lastData[i];
        stepArray[i] = lastByte;

        if (changeByte[i]) {
            step = 0;
            swapBytes(&step, lastByte, 6, 0);
            swapBytes(&step, lastByte, 2, 1);
            swapBytes(&step, lastByte, 4, 5);
            swapBytes(&step, lastByte, 3, 6);
            stepArray[i] = step;
        }
    }
    displayByte(stepArray);
    TIMER_Delay(TIMER0, delayms);

    for (int8_t i = 0; i < 4; i++) {
    	int8_t lastByte = lastBytes[i];
        stepArray[i] = lastByte;

        if (changeByte[i]) {
            step = 0;
            swapBytes(&step, lastByte, 3, 0);
            stepArray[i] = step;
        }
    }
    displayByte(stepArray);
    TIMER_Delay(TIMER0, delayms);

    for (int8_t i = 0; i < 4; i++) {
        if (changeByte[i]) {
            stepArray[i] = 0;
        }
    }
    displayByte(stepArray);
    TIMER_Delay(TIMER0, delayms);

    for (int8_t i = 0; i < 4; i++) {
    	int8_t lastByte = lastBytes[i];
    	int8_t newByte = DispData[i];
        stepArray[i] = lastByte;

        if (changeByte[i]) {
            step = 0;
            swapBytes(&step, newByte, 0, 3);
            stepArray[i] = step;
        }
    }
    displayByte(stepArray);
    TIMER_Delay(TIMER0, delayms);

    for (int8_t i = 0; i < 4; i++) {
    	int8_t newByte = DispData[i];
        stepArray[i] = lastBytes[i];

        if (changeByte[i]) {
            step = 0;
            swapBytes(&step, newByte, 0, 6);
            swapBytes(&step, newByte, 1, 2);
            swapBytes(&step, newByte, 5, 4);
            swapBytes(&step, newByte, 6, 3);
            stepArray[i] = step;
        }
    }
    displayByte(stepArray);
    TIMER_Delay(TIMER0, delayms);

    for (int8_t i = 0; i < 4; i++) {
        displayByte(DispData);
    }
}

void swapBytes(int8_t* newByte, int8_t oldByte, int8_t newP, int8_t oldP) {
	int8_t newBit = 0;
    if (oldByte & (1 << oldP)) newBit = 1;
    else newBit = 0;
    *newByte = *newByte | (newBit << newP);
}
//*********************************************scroll**********************************************************
//*****************************************scroll_segment******************************************************
void scroll_segment(uint8_t BitAddr, uint8_t DispData, int delayms) {
	uint8_t DispDataByte = digitHEX[DispData];
    scroll_Byte_segment(BitAddr, DispDataByte, delayms);
}

void scroll_Byte_segment(uint8_t BitAddr, uint8_t DispData, int delayms) {
	uint8_t oldByte = lastData[BitAddr];
	uint8_t newByte = DispData;
	uint8_t step;

    step = 0;
    swapBytes(&step, oldByte, 6, 0);
    swapBytes(&step, oldByte, 2, 1);
    swapBytes(&step, oldByte, 4, 5);
    swapBytes(&step, oldByte, 3, 6);
    displayByte_segment(BitAddr, step);
    TIMER_Delay(TIMER0, delayms);

    step = 0;
    swapBytes(&step, oldByte, 3, 0);
    swapBytes(&step, newByte, 0, 3);
    displayByte_segment(BitAddr, step);
    TIMER_Delay(TIMER0, delayms);

    step = 0;
    swapBytes(&step, newByte, 0, 6);
    swapBytes(&step, newByte, 1, 2);
    swapBytes(&step, newByte, 5, 4);
    swapBytes(&step, newByte, 6, 3);
    displayByte_segment(BitAddr, step);
    TIMER_Delay(TIMER0, delayms);

    displayByte_segment(BitAddr, newByte);
}
void displayByte_segment(uint8_t BitAddr, uint8_t DispData)
{
    uint8_t SegData;
    lastData[BitAddr] = DispData;
    SegData = DispData + PointData;
    sendByte(BitAddr, SegData);
}
//*****************************************scroll_segment******************************************************
//********************************************twist************************************************************

void twist(uint8_t DispData[], int delayms) {
	uint8_t newData[4];
    for (uint8_t i = 0; i < 4; i++) {
        newData[i] = digitHEX[DispData[i]];
    }
    twistByte(newData, delayms);
}

void twistByte(uint8_t DispData[], int delayms) {
	uint8_t step;
	uint8_t stepArray[4];
    bool changeByte[4] = {0, 0, 0, 0};

    for (uint8_t i = 0; i < 4; i++) {
        if (DispData[i] != lastData[i]) changeByte[i] = 1;
        stepArray[i] = DispData[i];
    }

    step = step & 0b00111111;
    for (uint8_t i = 0; i < 5; i++) {
        step |= (1 << i);
        for (uint8_t k = 0; k < 4; k++) {
            if (changeByte[k])
            stepArray[k] = step;
        }
        displayByte(stepArray);
        TIMER_Delay(TIMER0, delayms);
    }
    for (uint8_t i = 0; i < 6; i++) {
        step = 0b11000000;
        step = ~(step | (1 << i) | (1 << i + 1));
        for (uint8_t k = 0; k < 4; k++) {
            if (changeByte[k]) stepArray[k] = step;
        }
        displayByte(stepArray);
        TIMER_Delay(TIMER0, delayms);
    }
    step = 0b11000000;
    for (uint8_t i = 0; i < 6; i++) {
        step |= (1 << i);
        for (uint8_t k = 0; k < 4; k++) {
            if (changeByte[k])
            stepArray[k] = ~step;
        }
        displayByte(stepArray);
        TIMER_Delay(TIMER0, delayms);
    }
    for (uint8_t k = 0; k < 4; k++) {
        if (changeByte[k])
        stepArray[k] = 0b0000000;
    }
    for (uint8_t i = 0; i < 7; i++) {
        for (uint8_t k = 0; k < 4; k++) {
            if (changeByte[k]) {
            	uint8_t newBit = 0;
                if (DispData[k] & (1 << i)) newBit = 1;
                else newBit = 0;
                stepArray[k] |= (newBit << i);
            }
        }
        displayByte(stepArray);
        TIMER_Delay(TIMER0, delayms);
    }
}
void twistByte_segment(uint8_t BitAddr, uint8_t DispData, int delayms) {
	uint8_t oldByte = lastData[BitAddr];
	uint8_t newByte = DispData;
	uint8_t step = oldByte;

    //step = step & 0b0111111;
	step = step & 0b00111111;
    for (uint8_t i = 0; i < 5; i++) {
        step |= (1 << i);
        displayByte_twist(BitAddr, step);
        TIMER_Delay(TIMER0, delayms);
    }
    for (uint8_t i = 0; i < 6; i++) {
        //step = 0b1000000;
    	step = 0b11000000;
        step = ~(step | (1 << i) | (1 << i + 1));

        displayByte_twist(BitAddr, step);
        TIMER_Delay(TIMER0, delayms);
    }
    //step = 0b1000000;
    step = 0b11000000;
    for (uint8_t i = 0; i < 6; i++) {
        step |= (1 << i);
        displayByte_twist(BitAddr, ~step);
        TIMER_Delay(TIMER0, delayms);
    }
    step = 0;
    for (uint8_t i = 0; i < 7; i++) {
    	uint8_t newBit = 0;
        if (newByte & (1 << i)) newBit = 1;
        else newBit = 0;
        step |= (newBit << i);
        displayByte_twist(BitAddr, step);
        TIMER_Delay(TIMER0, delayms);
    }
    displayByte_twist(BitAddr, newByte);
}
void displayByte_twist(uint8_t BitAddr, uint8_t DispData)
{
    uint8_t SegData;
    lastData[BitAddr] = DispData;
    SegData = DispData + PointData;
    sendByte(BitAddr, SegData);
}

void twist_number(uint8_t BitAddr, uint8_t DispData, int delayms) {
    twistByte_segment(BitAddr, digitHEX[DispData], delayms);
}
//********************************************twist************************************************************
