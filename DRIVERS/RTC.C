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
#include "..\config.h"
/*********************************************************************************************************
** @DRIVER Function name:   RTCInit
** @DRIVER Input para:     	None
** @DRIVER retrun para:     None
*********************************************************************************************************/
void RTCInit(void)
{
    LPC_SC->PCONP |= (1ul << 9);
	if(LPC_RTC->RTC_AUX & (1ul << 4))
		LPC_RTC->RTC_AUX |= (1ul << 4);	
	LPC_RTC->CCR   = 0;                                          
    LPC_RTC->ILR   = 0x03;                                         
    LPC_RTC->CIIR  = 0;                                          
    LPC_RTC->AMR   = 0;
	LPC_RTC->CCR |= 0x01;
	LPC_RTC->ILR = 0x01;
	//LPC_RTC->CCR &= ~0x01; //This line can stop RTC,but the system never stop RTC.
}
/*********************************************************************************************************
** @DRIVER Function name:   RTCReadTime
** @DRIVER Input para:     	RTC_DATA
** @DRIVER retrun para:    	None
*********************************************************************************************************/
void RTCReadTime(unsigned char *date)
{
	unsigned int year;
	date[0] = LPC_RTC->SEC;
	date[1] = LPC_RTC->MIN;
	date[2] = LPC_RTC->HOUR;
	date[3] = LPC_RTC->DOM;
	date[4] = LPC_RTC->MONTH;
	year = LPC_RTC->YEAR;
	date[5] = year >> 8;
	date[6] = year;
}
/*********************************************************************************************************
** @DRIVER Function name:   RTCSetTime
** @DRIVER Input para:     	RTC_DATA
** @DRIVER retrun para:     None
*********************************************************************************************************/
void RTCSetTime(unsigned char *date)
{
	unsigned int year;
	LPC_RTC->SEC   = date[0];
	LPC_RTC->MIN   = date[1];
	LPC_RTC->HOUR  = date[2];
	LPC_RTC->DOM   = date[3];
	LPC_RTC->MONTH = date[4];
	year = (((unsigned int)date[5] << 8) + date[6]);
	LPC_RTC->YEAR  = year;	
}
/**************************************End Of File*******************************************************/
