/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			Key.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _LOG_H
#define _LOG_H
extern void LogGetMoneyAPI(uint32_t InValue,uint8_t MoneyType);
extern void LogTransactionAPI(int type);
extern void LogChangeAPI(uint32_t InValue);
extern void LogEndAPI(void);
extern void LogClearAPI(void);
#endif
/**************************************End Of File*******************************************************/
