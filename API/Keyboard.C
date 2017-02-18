/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			Keyboard.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\DRIVERS\Key.h"
#include "..\DRIVERS\Buzzer.h"
#include "API.H"
/*******************************************************************************************************
** @API Function name:   API_KEY_KeyboardInit
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*******************************************************************************************************/
extern void API_KEY_KeyboardInit(void)
{
	KeyboardInit();
}
/*******************************************************************************************************
** @API Function name:   API_KEY_KeyboardCtrl
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*******************************************************************************************************/
extern void API_KEY_KeyboardCtrl(unsigned char OpenOrClose)
{
	if(OpenOrClose)
		KeyboardCtrl(0x01);//Open
	else
		KeyboardCtrl(0x00);//Close
}
/*******************************************************************************************************
** @API Function name:   API_KEY_ReadKey
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*******************************************************************************************************/
extern unsigned char API_KEY_ReadKey(void)
{
	return ReadKeyValue();
}
/*******************************************************************************************************
** @API Function name:   API_BUZZER_Buzzer
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
*******************************************************************************************************/
extern void API_BUZZER_Buzzer(void)
{
	Buzzer();
}
/**************************************End Of File*****************************************************/
