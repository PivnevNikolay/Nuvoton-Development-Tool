#ifndef _TM_1637_H
#define _TM_1637_H

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
void runningString(uint8_t DispData[], uint8_t amount, int delayMs);
//*********************************************scroll**********************************************************
void scrollByte(uint8_t DispData[], int delayms);
void swapBytes(int8_t* newByte, int8_t oldByte, int8_t newP, int8_t oldP);
void scroll(uint8_t DispData[], int delayms);
void scroll_set_all(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);
//*********************************************scroll**********************************************************
//*****************************************scroll_segment******************************************************
void scroll_segment(uint8_t BitAddr, uint8_t DispData, int delayms);
void scroll_Byte_segment(uint8_t BitAddr, uint8_t DispData, int delayms);
void displayByte_segment(uint8_t BitAddr, uint8_t DispData);
//*****************************************scroll_segment******************************************************
#endif