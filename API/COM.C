/****************************************Copyright (c)****************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info---------------------------------------------------------
 ** @Filename:			com.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 ********************************************************************************************************/
#include "..\DRIVERS\UART0.h"
#include "..\DRIVERS\UART3.h"
#include "API.H"
/*********************************************************************************************************
** @API Function name:   API_COM_Init
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
extern void API_COM_Init(unsigned char COMNumb,unsigned int baud)
{
	switch(COMNumb)
	{
		case 0x00:
			InitUart0(baud);
			break;
		case 0x01:
			InitUart3(baud);
			break;
		case 0x02:
			//no used
			break;
		default:break;
	}
}
/*********************************************************************************************************
** @API Function name:   API_COM_RecBufferIsNotEmpty
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
unsigned char API_COM_RecBufferIsNotEmpty(unsigned char COMNumb)
{
	unsigned char empty = 0x00;
	switch(COMNumb)
	{
		case 0x00:
			empty = Uart0BuffIsNotEmpty();
			break;
		case 0x01:
			empty = Uart3BuffIsNotEmpty();
			break;
		case 0x02:
			//no used
			break;
		default:break;
	}
	return empty;
}
/*********************************************************************************************************
** @API Function name:   API_COM_ReadCharFromBuffer
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
unsigned char API_COM_ReadCharFromBuffer(unsigned char COMNumb)
{
	unsigned char ch = 0x00;
	switch(COMNumb)
	{
		case 0x00:
			ch = Uart0GetCh();
			break;
		case 0x01:
			ch = Uart3GetCh();
			break;
		case 0x02:
			//no used
			break;
		default:break;
	}
	return ch;
}
/*********************************************************************************************************
** @API Function name:   API_COM_SendStr
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
void API_COM_SendStr(unsigned char COMNumb,unsigned char *data,unsigned char len)
{
	switch(COMNumb)
	{
		case 0x00:
			Uart0PutStr(data,len);
			break;
		case 0x01:
			Uart3PutStr(data,len);
			break;
		case 0x02:
			//no used
			break;
		default:break;
	}
}
/**************************************End Of File*******************************************************/
