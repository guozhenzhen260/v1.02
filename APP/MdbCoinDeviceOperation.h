/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			ParaDef.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _MDBCOINDEVICEOPERATION_H
#define _MDBCOINDEVICEOPERATION_H

void MdbCoinResetAndSetup(void);
unsigned char CoinDevProcess(unsigned int *RecvMoney,unsigned char *CoinType,unsigned char *coinOptBack);
void MdbCoinGetTubeStatus(void);
void MdbCoinTypeEanbleOrDisable(unsigned char AcceptCtrl,unsigned char DispenCtrl);
unsigned int MdbGetCoinValue(unsigned char CoinTypeCredit);
unsigned char MdbCoinPayout(unsigned int PayoutValue,unsigned int *AcuPayout);
unsigned char ChangePayoutProcessLevel3(unsigned int PayMoney,unsigned int *PayoutMoney);

#endif
/**************************************End Of File*******************************************************/
