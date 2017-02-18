/****************************************Copyright (c)******************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-----------------------------------------------------------------------
 ** @Filename:			FRAM.C
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 **********************************************************************************************************************/
#include "..\DRIVERS\I2C.h"
#include "API.H"
/************************************************************************************************************************
** @API Function name:   API_FRAM_Init
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
************************************************************************************************************************/
extern void API_FRAM_Init(void)
{
	I2C0Init();
}
/************************************************************************************************************************
** @API Function name:   API_RTC_RtcInit
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
************************************************************************************************************************/
extern unsigned char API_FRAM_ReadFram (unsigned char sa,unsigned int suba,unsigned char *s,unsigned int num)
{
	return I2C0RdNByte(sa,2,suba,s,num);
}
/************************************************************************************************************************
** @API Function name:   API_RTC_Read
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
************************************************************************************************************************/
extern unsigned char API_FRAM_WriteFram(unsigned char sa,unsigned int suba,unsigned char *s,unsigned int num)
{
	return I2C0WrNByte(sa,2,suba,s,num);
}
/**************************************End Of File**********************************************************************/
