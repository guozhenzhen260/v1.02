/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			MdbBillDeviceOperation.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _MDBBILLDEVICEOPERATION_H
#define _MDBBILLDEVICEOPERATION_H

void MdbBillResetAndSetup(void);
unsigned char BillDevProcess(unsigned int *RecvMoney,unsigned char *BillType,unsigned char billOpt,unsigned char *billOptBack);
void MdbBillTypeEanbleOrDisable(unsigned char AcceptCtrl,unsigned char DispenCtrl);
void MdbBillGetStackerStatus(void);
void MdbBillStackBillOrReturn(unsigned char ctrl);
void MdbBillTypeEscrowEanbleOrDisable(unsigned short EscrowCtrl);
unsigned char MdbRecyclerPayout(unsigned int PayoutValue,unsigned int *AcuPayout);

#endif
/**************************************End Of File*******************************************************/
