/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			Android.c
 ** @brief:				安卓主机接口
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\API\API.H"
#include "..\API\OS.H"
#include "Android.h"
/*********************************************************************************************************
** @APP Function name:  AndroidService
** @APP Description:   	安卓主机接口
** @APP Input para:		pvParameters:no used
** @APP retrun para:    None
*********************************************************************************************************/
void AndroidService(void *pvParameters)
{
	unsigned char cmd;
	pvParameters = pvParameters;
	API_COM_Init(1,9600);		//Andriod主机通讯口
	while(1)
	{
		if(API_COM_RecBufferIsNotEmpty(1))
		{
			cmd = API_COM_ReadCharFromBuffer(1);
			if(cmd == 'T')
			{
				API_COM_SendStr(1,"Rev\r\n",5);
			}
		}
		vTaskDelay(10);
	}
}
/**************************************End Of File*******************************************************/
