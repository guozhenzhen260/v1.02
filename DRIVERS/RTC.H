/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			RTC.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _RTC_H
#define _RTC_H

void RTCInit(void);
void RTCStartOrStop(unsigned char ss);
void RTCReadTime(unsigned char *date);
void RTCSetTime(unsigned char *date);

#endif
/**************************************End Of File*******************************************************/
