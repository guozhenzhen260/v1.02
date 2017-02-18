/****************************************Copyright (c)***************************************************
 **                      JUNPENG Easivend Co.,Ltd.
 **                               http://www.easivend.com
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			uart0.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\config.h"	
#define	UART3BUFFERLEN		128
volatile unsigned char Uart3RevBuff[UART3BUFFERLEN];
volatile unsigned char Uart3RxdHead;
volatile unsigned char Uart3RxdTail;
/********************************************************************************************************
** @DRIVER Function name:   InitUart3
** @DRIVER Input para:     	None
** @DRIVER retrun para:     None
********************************************************************************************************/
void InitUart3 (unsigned int baud)
{
    unsigned short int ulFdiv;
	LPC_SC->PCONP |= (1ul << 25);
    LPC_UART3->LCR  = 0x83;                   				//允许设置波特率
    ulFdiv = (24000000 / 16) / baud;						//设置波特率
    LPC_UART3->DLM  = ulFdiv / 256;
    LPC_UART3->DLL  = ulFdiv % 256; 
    LPC_UART3->LCR  = 0x03;									//锁定波特率
    LPC_UART3->FCR  = 0x87;									//使能FIFO，设置8个字节触发点
    LPC_UART3->IER  = 0x01;									//使能接收中断
	Uart3RxdHead  = 0;
	Uart3RxdTail  = 0;
	memset((void *)Uart3RevBuff,0x00,UART3BUFFERLEN);		//初始化缓冲区
	NVIC_EnableIRQ(UART3_IRQn);
	NVIC_SetPriority(UART3_IRQn,11);
}
/********************************************************************************************************
** @DRIVER Function name:   UART0_IRQHandler
** @DRIVER Input para:     	ISR
** @DRIVER retrun para:     None
********************************************************************************************************/
void UART3_IRQHandler (void)
{
	unsigned char Num;
	unsigned char rxd_head;
	unsigned char rxd_data;
    while((LPC_UART3->IIR & 0x01) == 0)
	{
        switch(LPC_UART3->IIR & 0x0E)
		{                                         
            case 0x04 : for (Num = 0; Num < 8; Num++)
						{
		                	rxd_data = LPC_UART3->RBR;
							rxd_head = (Uart3RxdHead + 1);
					        if( rxd_head >= UART3BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart3RxdTail) 
					        {
					           	Uart3RevBuff[Uart3RxdHead] = rxd_data;
					           	Uart3RxdHead = rxd_head;
					        }
		                }
		                break;
            case 0x0C : while((LPC_UART3->LSR & 0x01) == 0x01)
						{                         
		                	rxd_data = LPC_UART3->RBR;
							rxd_head = (Uart3RxdHead + 1);
					        if( rxd_head >= UART3BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart3RxdTail) 
					        {
					           	Uart3RevBuff[Uart3RxdHead] = rxd_data;
					           	Uart3RxdHead = rxd_head;
					        }
		                }
		                break;
            default	: break;
        }
    }
}
/********************************************************************************************************
** @DRIVER Function name:   Uart3PutChar
** @DRIVER Input para:     	ch:data
** @DRIVER retrun para:     None
********************************************************************************************************/
void Uart3PutChar(unsigned char ch)
{
    LPC_UART3->THR = ch;
    while((LPC_UART3->LSR & 0x20) == 0);
}
/********************************************************************************************************
** @DRIVER Function name:   Uart3PutStr
** @DRIVER Input para:     	str:data;Len:data len
** @DRIVER retrun para:     None
********************************************************************************************************/
void Uart3PutStr(unsigned char *Str, unsigned int Len)
{
    unsigned int i;  
    for (i=0;i<Len;i++) 
	{
        Uart3PutChar(*Str++);
    }
}
/********************************************************************************************************
** @DRIVER Function name:   Uart3BuffIsNotEmpty
** @DRIVER Input para:     	None
** @DRIVER retrun para:     Empty status
********************************************************************************************************/
unsigned char Uart3BuffIsNotEmpty(void)
{
	if(Uart3RxdHead==Uart3RxdTail) 
		return 0;
	else
		return 1;
}
/********************************************************************************************************
** @DRIVER Function name:   Uart3GetCh
** @DRIVER Input para:     	None
** @DRIVER retrun para:     Get byte
********************************************************************************************************/
unsigned char Uart3GetCh(void)
{
    unsigned char ch;
    ch = Uart3RevBuff[Uart3RxdTail];
    Uart3RevBuff[Uart3RxdTail] = 0;
    Uart3RxdTail++;
    if(Uart3RxdTail >= UART3BUFFERLEN)
		Uart3RxdTail = 0;
	return ch;
}
/**************************************End Of File*******************************************************/
