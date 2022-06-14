#include "stdio.h"
#include "NuMicro.h"
#include "TM_1637.h"
#include "stdbool.h"

//*************************************************************************************************************
//                        H     E     L     L     O                 1     6     3     7           D     i     S     P
uint8_t Hello_world[] = {0x76, 0x79, 0x38, 0x38, 0x3f, 0x00, 0x00, 0x06, 0x7d, 0x4f, 0x07, 0x00, 0x3f, 0x10, 0x6d, 0x73,
//                        l     A     Y                           0    1    2    3    4    5    6    7    8    9
                        0x06, 0x77, 0x6e, 0x00, 0x00,0x00, 0x00,0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//*************************************************************************************************************

void scrolls();

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
//*************************************************************************************************************
void UART0_Init()
{
/* Reset UART module */
SYS_ResetModule(UART0_RST);
/* Configure UART0 and set UART0 baud rate */
UART_Open(UART0, 115200);
}
//***********************************************main**********************************************************
int main()
{
SYS_Init();
UART0_Init();
GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);//CLK
GPIO_SetMode(PC, BIT1, GPIO_MODE_OUTPUT);//DIO
//printf("\n Start test: \n");
clear();
brightness_(7, 0x40, 0xc0); // brightness, 0 - 7 (min - max)
clear();
display_Byte(0x3f, 0x06, 0x5b, 0x4f);
TIMER_Delay(TIMER0, 0x1E8480);//delay 2 seconds
clear();
display_Byte(0x66, 0x6d, 0x7d, 0x07);
TIMER_Delay(TIMER0, 0x1E8480);//delay 2 seconds
//**************************************************while(1)***************************************************
while(1){
  clear();
  TIMER_Delay(TIMER0, 0xF4240);
  scrolls();
  runningString(Hello_world, sizeof(Hello_world), 350000);
 }
}
//*************************************************************************************************************
//*******************************************functions*********************************************************
void scrolls() {
  // set an array of numbers
  int8_t digs[4] = {4, 5, 6, 7};
  scroll(digs, 190000);     // scroll all segments
  TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
  scroll_segment(0, 8, 120000);//set 8 and rotate the first segment
  TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
  scroll_segment(1, 9, 120000);//set 9 and twist the second segment
  TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
  scroll_segment(2, 0, 120000);//set to 0 and twist the third segment
  TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
  scroll_segment(3, 1, 120000);//set 1 and twist the fourth segment
  TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
  clear();
  for (uint8_t i = 0; i < 10; i++) {
      scroll_segment(3, i, 100000);
      TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
  }
  scroll_set_all(2, 3, 0, 1, 150000);//set values to all 4 segments and scroll
  TIMER_Delay(TIMER0, 0xF4240);//delay 1 seconds
}
//***********************************************the end*******************************************************