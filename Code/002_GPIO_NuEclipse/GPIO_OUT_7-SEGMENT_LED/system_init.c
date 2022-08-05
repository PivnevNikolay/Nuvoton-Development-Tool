#include <stdio.h>
#include "NuMicro.h"
#include "system_init.h"

void SYS_Init(void)
{
  SYS_UnlockReg();
  // Enable HIRC clock (Internal RC 48MHz)
  CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
  // Wait for HIRC clock ready
  CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
  // Select HCLK clock source as HIRC and HCLK source divider as 1
  CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
  // Enable UART0 clock
  CLK_EnableModuleClock(UART0_MODULE);
  CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
  SystemCoreClockUpdate();
  // Set multi-function pins UART0 RXD = PD.2 and TXD = PA.7
  SYS->GPD_MFPL = (SYS->GPD_MFPL & ~SYS_GPD_MFPL_PD2MFP_Msk) | SYS_GPD_MFPL_PD2MFP_UART0_RXD;
  SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA7MFP_Msk) | SYS_GPA_MFPL_PA7MFP_UART0_TXD;
  SYS_LockReg();
}

void UART0_Init(void)
{
  // Reset UART0
  SYS_ResetModule(UART0_RST);
  // Configure UART0 and set UART0 baud rate
  UART_Open(UART0, 115200);
}
