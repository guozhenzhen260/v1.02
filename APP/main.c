/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			main.c
 ** @brief:				main  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\CONFIG.H"
/********************************************************************************************************
 ** @FunctionName	main
 ** @brief 			Config CPU Clock and PIN/GPIO,Create Tasks and start FreeRTOS
 ** @param  		none
 ** @return 		never
 *******************************************************************************************************/
int main(void)
{
    xTaskCreate(VendService,"VendService",2048,(void *)NULL,tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(Maintenance,"Maintenance",1048,(void *)NULL,tskIDLE_PRIORITY+2,NULL);
	xTaskCreate(AndroidService,"AndroidService",512,(void *)NULL,tskIDLE_PRIORITY+3,NULL);
	vTaskStartScheduler();
}
/**************************************End Of File*******************************************************/
