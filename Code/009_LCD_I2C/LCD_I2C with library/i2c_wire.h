#ifndef _I2c_wire_H
#define _I2c_wire_H

#define BUFFER_LENGTH 32

void begin(void);
void begin_address(u8  address);
void beginTransmission(u8  address);
size_t write(u8  data);
u8 EndTransmission(bool sendStop);
void endTransmission(void);
void I2C0_Close(void);

#endif
