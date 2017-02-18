/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			RTC.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\DRIVERS\RTC.h"
#include "API.H"
/*********************************************************************************************************
** @API Function name:   API_RTC_RtcInit
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
extern void API_RTC_RtcInit(void)
{
	RTCInit();
}
/*********************************************************************************************************
** @API Function name:   API_RTC_Read
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
extern void API_RTC_Read(unsigned char *date)
{
	RTCReadTime(date);
}
/*********************************************************************************************************
** @API Function name:   API_RTC_Write
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*********************************************************************************************************/
extern void API_RTC_Write(unsigned char *date)
{
	RTCSetTime(date);
}
/**************************************End Of File*******************************************************/
