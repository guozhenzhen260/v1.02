/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			uart.h
 ** @brief:				head file
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef __UART0_H 
#define __UART0_H

void InitUart0(unsigned int BPS);
void UART0_IRQHandler(void);
void Uart0PutChar(unsigned char ch);
void Uart0PutStr(unsigned char const *Str, unsigned int Len);
unsigned char Uart0BuffIsNotEmpty(void);
unsigned char Uart0GetCh(void);
void Trace(const char *format,...);

#endif
/**************************************End Of File*******************************************************/
