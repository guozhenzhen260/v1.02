/****************************************Copyright (c)******************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-----------------------------------------------------------------------
 ** @Filename:			VENDING.C
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 **********************************************************************************************************************/
#include "..\DRIVERS\Motor.h"
#include "API.H"
/************************************************************************************************************************
** @API Function name:   API_FRAM_Init
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
************************************************************************************************************************/
extern void API_VENDING_Init(void)
{
	MOTORInit();
}
/************************************************************************************************************************
** @API Function name:   API_FRAM_Init
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
************************************************************************************************************************/
extern unsigned char API_VENDING_Vend(char Layer,char Channel)
{
	return MotorVending(Layer,Channel);
}
/**************************************End Of File**********************************************************************/
