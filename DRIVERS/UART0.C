/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			uart0.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\config.h"	
#define	UART0BUFFERLEN		64
volatile unsigned char Uart0RevBuff[UART0BUFFERLEN];
volatile unsigned char Uart0RxdHead;
volatile unsigned char Uart0RxdTail;
/********************************************************************************************************
** @DRIVER Function name:   InitUart0
** @DRIVER Input para:     	None
** @DRIVER retrun para:     None
********************************************************************************************************/
void InitUart0(unsigned int BPS)
{
    unsigned int Freq;
	LPC_SC->PCONP |= 0x08;
    LPC_UART0->LCR  = 0x83;
    Freq = (24000000 / 16) / BPS;
    LPC_UART0->DLM  = Freq / 256;
    LPC_UART0->DLL  = Freq % 256;
    LPC_UART0->LCR  = 0x03;
    LPC_UART0->FCR  = 0x87;
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn,10);
	LPC_UART0->IER  = 0x01;
	Uart0RxdHead  = 0;
	Uart0RxdTail  = 0;
	memset((void *)Uart0RevBuff,0x00,UART0BUFFERLEN);
}
/********************************************************************************************************
** @IRQ Function name:  UART0_IRQHandler
** @IRQ Input para:     None
** @IRQ retrun para:    None
********************************************************************************************************/
void UART0_IRQHandler(void)
{
	unsigned char Num;
	unsigned char rxd_head;
	unsigned char rxd_data;
    while((LPC_UART0->IIR & 0x01) == 0)
	{
        switch(LPC_UART0->IIR & 0x0E)
		{                                         
            case 0x04: 
				for(Num=0;Num<8;Num++)
				{
					rxd_data = LPC_UART0->RBR;
					rxd_head = (Uart0RxdHead + 1);
					if(rxd_head >= UART0BUFFERLEN)
						rxd_head = 0;
					if(rxd_head != Uart0RxdTail)
					{
						Uart0RevBuff[Uart0RxdHead] = rxd_data;
						Uart0RxdHead = rxd_head;
					}
				}
				break;
            case 0x0C:
				while((LPC_UART0->LSR & 0x01) == 0x01)
				{                         
					rxd_data = LPC_UART0->RBR;
					rxd_head = (Uart0RxdHead + 1);
					if(rxd_head >= UART0BUFFERLEN)
						rxd_head = 0;
					if(rxd_head != Uart0RxdTail)
					{
						Uart0RevBuff[Uart0RxdHead] = rxd_data;
						Uart0RxdHead = rxd_head;
					}
				}
				break;
            default: 
				break;
        }
    }
}
/********************************************************************************************************
** @DRIVER Function name:   Uart0PutChar
** @DRIVER Input para:     	ch:data
** @DRIVER retrun para:     None
********************************************************************************************************/
void Uart0PutChar(unsigned char ch)
{
    LPC_UART0->THR = ch;
    while((LPC_UART0->LSR & 0x20) == 0);
}

/********************************************************************************************************
** @DRIVER Function name:   Uart0PutStr
** @DRIVER Input para:     	str:data;len:lenght
** @DRIVER retrun para:     None
********************************************************************************************************/
void Uart0PutStr(unsigned char const *str, unsigned int len)
{
    unsigned int i;
    for (i=0;i<len;i++)
	{
        Uart0PutChar(*str++);
    }
}
/********************************************************************************************************
** @DRIVER Function name:   Uart0BuffIsNotEmpty
** @DRIVER Input para:     	None
** @DRIVER retrun para:     0x00:empyt;0x01:has data
********************************************************************************************************/
unsigned char Uart0BuffIsNotEmpty(void)
{
	if(Uart0RxdHead==Uart0RxdTail)
		return 0;
	else
		return 1;
}
/********************************************************************************************************
** @DRIVER Function name:   Uart0GetCh
** @DRIVER Input para:     	None
** @DRIVER retrun para:     data
********************************************************************************************************/
unsigned char Uart0GetCh(void)
{
    unsigned char ch;
    ch = Uart0RevBuff[Uart0RxdTail];
    Uart0RevBuff[Uart0RxdTail] = 0;
    Uart0RxdTail++;
    if(Uart0RxdTail >= UART0BUFFERLEN)
		Uart0RxdTail = 0;
	return ch;
}
/********************************************************************************************************
** @DRIVER Function name:   Trace
** @DRIVER Input para:     	*format:const str;...:list
** @DRIVER retrun para:     None
********************************************************************************************************/
void Trace(const char *format,...)
{
	unsigned char StringTemp[168];
	unsigned int i;
	unsigned int len;
	va_list start_Point;
	va_start(start_Point,format);
	len = vsprintf((char *)StringTemp,(const char *)format,start_Point);
	va_end(start_Point);
	for(i=0;i<len;i++)
	{
		Uart0PutChar(StringTemp[i]);
	}
}
/**************************************End Of File******************************************************/
