/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			System.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\DRIVERS\TIMER.h"
#include "..\DRIVERS\PINSELECT.h"
#include "API.H"
extern volatile unsigned int TimerForAPP[8];
/*********************************************************************************************************
** @API Function name:   API_SYSTEM_PinFuctionSelect
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
void API_SYSTEM_TimerInit(unsigned char number)
{
	switch(number)
	{
		case 0x00:
			InitTimer(0,240000);	//For APP
			break;
		case 0x01:
			InitTimer(3,240000);	//For MDB uart conversation
			break;
		default:
			break;
	}
}
/*********************************************************************************************************
** @API Function name:   API_SYSTEM_PinFuctionSelect
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
void API_SYSTEM_TimerChannelSet(unsigned char channel,unsigned int value)
{
	if(channel < 8)
	{
		TimerForAPP[channel] = value;
		TimerCtrl(0,1);
	}
}
/*********************************************************************************************************
** @API Function name:   API_SYSTEM_PinFuctionSelect
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
unsigned int API_SYSTEM_TimerReadChannelValue(unsigned char channel)
{
	if(channel < 8)
		return TimerForAPP[channel];
	else
		return 0x00;
}
/*********************************************************************************************************
** @API Function name:   API_SYSTEM_PinFuctionSelect
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
void API_SYSTEM_PinFuctionSelect(void)
{
	PinFuctionSelect();
}
/**************************************End Of File*******************************************************/
