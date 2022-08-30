#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include "NuMicro.h"
#include "system_init.h"

void SYS_Init(void)
{

    SYS_UnlockReg();
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1);
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_PCLK0, CLK_CLKDIV0_UART0(1));
    SystemCoreClockUpdate();
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    SYS_LockReg();
}

void UART0_Init(void)
{
    // Reset UART0
    SYS_ResetModule(UART0_RST);
    // Configure UART0 and set UART0 baud rate
    UART_Open(UART0, 115200);
}

void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number) {
  uint8_t i;
  uint32_t rounding_2 = 1;
  float rounding = 0.5f;
  *sign_number = 0;
  if (*((uint32_t*) &value) & 0x80000000) {
  *((uint32_t*) &value) &= 0x7FFFFFFF;
  *sign_number = 1;
  }
    for (i = 0; i < width; i++) {
    rounding_2 = rounding_2 * 10;
  }
  rounding = rounding / rounding_2;
  *integer_number = (int) ((float) value + rounding);
  *fractional_number = (((float) value + rounding) * rounding_2) - (*integer_number * rounding_2);
}
