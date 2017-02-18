/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			log.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\API\API.H"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "Log.h"
/*********************************************************************************************************
** @APP Function name:  LogGetMoneyAPI
** @APP Description:   	收币时更新日志
** @APP Input para:		InValue投币的金额分为单位,MoneyType=1硬币器,2纸币器,3读卡器
** @APP retrun para:    None
*********************************************************************************************************/
void LogGetMoneyAPI(uint32_t InValue,uint8_t MoneyType)
{
	uint16_t money=(InValue/10);	
	//更新明细交易记录
	if(MoneyType == 1)
		TotalTradeLog.TotalCoinPayIn += money;
	else if(MoneyType == 2)
		TotalTradeLog.TotalBillPayIn += money;	
}
/*********************************************************************************************************
** @APP Function name:  LogTransactionAPI
** @APP Description:   	交易出货时更新日志
** @APP Input para:		type=1出货成功,0出货失败
** @APP retrun para:    None
*********************************************************************************************************/
void LogTransactionAPI(int type)
{
	if(type==1)
		TotalTradeLog.TotalSuccesNumber++;	
	else
		TotalTradeLog.TotalErrorNumber++;		
}
/*********************************************************************************************************
** @APP Function name:  LogChangeAPI
** @APP Description:   	找零时更新日志
** @APP Input para:		InValue找零的金额
** @APP retrun para:    None
*********************************************************************************************************/
void LogChangeAPI(uint32_t InValue)
{	
	uint16_t money=(InValue/10);
	//更新区域交易记录
	TotalTradeLog.TotalCoinPayout += money;
}
/*********************************************************************************************************
** @APP Function name:  LogChangeAPI
** @APP Description:   	找零时更新日志
** @APP Input para:		InValue找零的金额
** @APP retrun para:    None
*********************************************************************************************************/
void LogEndAPI()
{	
	LoadNewTotalLog();
}
/*********************************************************************************************************
** @APP Function name:  LogClearAPI
** @APP Description:   	收币时更新日志
** @APP Input para:		删除区域日志记录
** @APP retrun para:    None
*********************************************************************************************************/
void LogClearAPI(void)
{
	TotalTradeLog.TotalCoinPayIn=0;
	TotalTradeLog.TotalBillPayIn=0;
	TotalTradeLog.TotalSuccesNumber=0;
	TotalTradeLog.TotalErrorNumber=0;	
	TotalTradeLog.TotalCoinPayout=0;
	LoadNewTotalLog();	
}
/**************************************End Of File*******************************************************/
