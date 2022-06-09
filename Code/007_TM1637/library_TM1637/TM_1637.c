#include "stdio.h"
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
