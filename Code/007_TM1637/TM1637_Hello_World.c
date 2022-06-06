/**-----------------------------------------------------------------------------------------------
*\date  06.06.2022
*\brief
*\authors ScuratovaAnna + PivnevNikolay
* Telegram "Nuvoton Programming":    https://t.me/nuvoton_programming
* M251EC2AE 32 KB Flash 8 KB SRAM
*      https://www.nuvoton.com/products/microcontrollers/arm-cortex-m23-mcus/m251-m252-series/m251ec2ae
*      https://alexgyver.ru/tm1637_display/
*/
#include "stdio.h"
#include "NuMicro.h"
#define ADDR_FIXED 0x44
#define ADDR_AUTO  0x40
const uint8_t digitHEX[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x40}; //0~9
uint8_t lastData[4];
uint8_t PointData;
uint8_t Cmd_DispCtrl;
uint8_t Cmd_SetData;
uint8_t Cmd_SetAddr;
//*************************************************************************************************************
void clear(void);
void display(uint8_t BitAddr, uint8_t DispData);
void sendByte(uint8_t BitAddr, int8_t sendData);
void brightness_(uint8_t brightness, uint8_t SetData, uint8_t SetAddr);
int writeByte(int8_t wr_data);
void update(void);
void displayByte(uint8_t DispData[]);
void sendArray(uint8_t sendData[]);
void display_Byte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);
void start(void);
void stop(void);

void SYS_Init(void)
{
/* Unlock protected registers */
SYS_UnlockReg();
/* Enable Internal RC 12MHz clock */
CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
/* Waiting for Internal RC clock ready */
CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
/* Switch HCLK clock source to Internal RC and HCLK source divide 5 + 1 */
CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(6));
/* Enable UART clock */
CLK_EnableModuleClock(UART0_MODULE);
CLK_EnableModuleClock(TMR0_MODULE);
/* Select UART clock source from HIRC */
CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK0, 0);
/* Update System Core Clock */
/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
SystemCoreClockUpdate();
/* Set GPB multi-function pins to UART0 RXD and TXD */
SYS->GPA_MFPH = (SYS->GPA_MFPH & ~SYS_GPA_MFPH_PA15MFP_Msk) | SYS_GPA_MFPH_PA15MFP_UART0_RXD;//PA.15
SYS->GPA_MFPH = (SYS->GPA_MFPH & ~SYS_GPA_MFPH_PA14MFP_Msk) | SYS_GPA_MFPH_PA14MFP_UART0_TXD ;//PA.14
/* Set PC.0 to GPIO */
SYS->GPC_MFPL = (SYS->GPC_MFPL & ~SYS_GPC_MFPL_PC0MFP_Msk)  | SYS_GPC_MFPL_PC0MFP_GPIO;
/* Set PC.1 to GPIO */
SYS->GPC_MFPL = (SYS->GPC_MFPL & ~SYS_GPC_MFPL_PC1MFP_Msk)  | SYS_GPC_MFPL_PC1MFP_GPIO;
/* Lock protected registers */
SYS_LockReg();
}
void UART0_Init()
{
/* Reset UART module */
SYS_ResetModule(UART0_RST);
/* Configure UART0 and set UART0 baud rate */
UART_Open(UART0, 115200);
}
int main()
{
SYS_Init();
UART0_Init();
GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);//CLK
GPIO_SetMode(PC, BIT1, GPIO_MODE_OUTPUT);//DIO
//printf("\n Start test: \n");
clear();
brightness_(7, 0x40, 0xc0); // brightness, 0 - 7 (min - max)
while(1){
  clear();
  display_Byte(0x3f, 0x06, 0x5b, 0x4f);
  TIMER_Delay(TIMER0, 0x1E8480);//delay 2 seconds
  clear();
  display_Byte(0x66, 0x6d, 0x7d, 0x07);
  TIMER_Delay(TIMER0, 0x1E8480);//delay 2 seconds
 }
}
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

