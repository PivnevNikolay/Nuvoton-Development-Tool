//-----------------------------------------------------------------------------/
// 30.03.2021
// GPIO pin mode and use pin data input/output control.
//----------------------------------------------------------------------------/
#include "stdio.h"
#include "NuMicro.h"
//                0    1   2    3     4   5    6    7   8    9
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
uint8_t z;
void SYS_Init(void)
{
    //---------------------------------------------------------------------------------------------------------/
    // Init System Clock                                                                                       
    //---------------------------------------------------------------------------------------------------------/
    // Enable HIRC clock (Internal RC 48 MHz)
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    // Wait for HIRC clock ready 
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    // Select HCLK clock source as HIRC and and HCLK clock divider as 1 
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    // Enable UART module clock 
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
    CLK_EnableModuleClock(GPC_MODULE);
    // Select UART module clock source as HIRC and UART module clock divider as 1 
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));    
    // Init I/O Multi-function     
    Uart0DefaultMPF();
}

void UART0_Init()
{
	  // Reset UART module 
    SYS_ResetModule(UART0_RST);
    // Configure UART0 and set UART0 baud rate 
    UART_Open(UART0, 115200);
}

int main(void)
	{
		// Unlock protected registers
    SYS_UnlockReg();
    // Init System, peripheral clock and multi-function I/O 
    SYS_Init();
    // Lock protected registers 
    SYS_LockReg();
    // Init UART0 for printf 
    UART0_Init();
    printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
    GPIO_SetMode(PB, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6), GPIO_MODE_OUTPUT);
    while (1){			
		for ( z =0;z<10;z++){
			PB->DOUT = led[z];
			CLK_SysTickDelay(349525);
		  }
	  }
  }
