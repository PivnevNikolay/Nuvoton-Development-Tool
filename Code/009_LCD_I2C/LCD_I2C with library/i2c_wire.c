#include <stdio.h>
#include "stdbool.h"
#include "NuMicro.h"
#include "system_init.h"
#include "i2c_wire.h"

static const u8 TIMEOUT = 100;
static const uint32_t I2C_CLOCK = 100000;

// TWI state
enum WireStatus {
	UNINITIALIZED,
	MASTER_IDLE,
	MASTER_SEND,
	MASTER_RECV,
	SLAVE_IDLE,
	SLAVE_RECV,
	SLAVE_SEND
};
enum WireStatus status;

//------------------------------------------------------//
// TX Buffer
u8 txAddress;
u8 txBuffer[BUFFER_LENGTH];
u8 txBufferLength;
//------------------------------------------------------
// Service buffer
uint8_t srvBuffer[BUFFER_LENGTH];
uint8_t srvBufferIndex;
uint8_t srvBufferLength;
//------------------------------------------------------//
//*******************************************************************************************//
//++++++++++++++++++++++++++++++++++++++functions++++++++++++++++++++++++++++++++++++++++++++//
//*******************************************************************************************//
void begin(void) {
    I2C_Open(I2C0, I2C_CLOCK);
    printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C0));
    status = MASTER_IDLE;
}
//*******************************************************************************************//

void begin_address(u8 address) {
	I2C_Open(I2C0,I2C_CLOCK);
	status = SLAVE_IDLE;
	I2C_SetSlaveAddr(I2C0, 0, address, 0);   /* Slave Address */
	I2C_EnableInt(I2C0);
	I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI_AA); /* I2C enter no address SLV mode */
}
//*******************************************************************************************//
void beginTransmission(u8  address) {
	status = MASTER_SEND;
	// save address of target and empty buffer
	txAddress = address<<1;
	txBufferLength = 0;
	printf("txAddress %d\n", txAddress);
}
//*******************************************************************************************//
size_t write(u8 data) {
	if (status == MASTER_SEND) {
		if (txBufferLength >= BUFFER_LENGTH)
			return 0;
		txBuffer[txBufferLength++] = data;
		return 1;
	} else {
		if (srvBufferLength >= BUFFER_LENGTH)
			return 0;
		srvBuffer[srvBufferLength++] = data;
		return 1;
	}
}
//*******************************************************************************************//
u8 EndTransmission(bool sendStop){
	int sent = 0;
	u8 timeout_= TIMEOUT;
	while(timeout_--){
	I2C_START(I2C0);
	I2C_WAIT_READY(I2C0);
	I2C_SET_DATA(I2C0, txAddress);
	I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
	I2C_WAIT_READY(I2C0);
	if(I2C_GET_STATUS(I2C0)!=0x18)
	 {
		I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO | I2C_CTL_SI);
		continue;
	}
	sent = 0;
	while (sent < txBufferLength) {
		I2C_SET_DATA(I2C0, txBuffer[sent++]);
		I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
		I2C_WAIT_READY(I2C0);
		if(I2C_GET_STATUS(I2C0)!=0x28) {
		  I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO | I2C_CTL_SI);
		  continue;
		}
	}
	if(sendStop==true)
	{
		/* Send stop */
		I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO | I2C_CTL_SI);
	}
	break;
	}
	// empty buffer
	txBufferLength = 0;
	status = MASTER_IDLE;
	return sent;
}
//*******************************************************************************************//

void endTransmission(void)
{
	EndTransmission(true);
}
//*******************************************************************************************//
void I2C0_Close(void)
{
    /* Disable I2C0 interrupt and clear corresponding NVIC bit */
    I2C_DisableInt(I2C0);
    NVIC_DisableIRQ(I2C0_IRQn);
    /* Disable I2C0 and close I2C0 clock */
    I2C_Close(I2C0);
    CLK_DisableModuleClock(I2C0_MODULE);
}
//*******************************************************************************************//
