/****************************************Copyright (c)***************************************************
 **                      JUNPENG Easivend Co.,Ltd.
 **                               http://www.easivend.com
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			uart.h
 ** @brief:				head file
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef __UART3_H 
#define __UART3_H

void InitUart3(unsigned int baud);
void UART3_IRQHandler(void);
void Uart3PutChar(unsigned char ch);
void Uart3PutStr(unsigned char *Str, unsigned int Len);
unsigned char Uart3BuffIsNotEmpty(void);
unsigned char Uart3GetCh(void);

#endif
/**************************************End Of File*******************************************************/
