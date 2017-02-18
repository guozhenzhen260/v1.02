/****************************************Copyright (c)*********************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.LinkMac.com.cn
 **-------------------------------------File Info--------------------------------------------------------------------------------------
 ** @Filename:			API.H
 ** @brief:				API head files 
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 **************************************************************************************************************************************/
#ifndef __API_H
#define __API_H

//#define DEBUG_MDB						//Trace MDB conversation information

/**************************************@System & @Timer API****************************************************************************/
extern void API_SYSTEM_TimerInit(unsigned char number);
extern void API_SYSTEM_TimerChannelSet(unsigned char channel,unsigned int value);
extern unsigned int API_SYSTEM_TimerReadChannelValue(unsigned char channel);
extern void API_SYSTEM_PinFuctionSelect(void);

/**************************************@COM API****************************************************************************************/
extern void API_COM_Init(unsigned char COMNumb,unsigned int baud);
extern unsigned char API_COM_RecBufferIsNotEmpty(unsigned char COMNumb);
extern unsigned char API_COM_ReadCharFromBuffer(unsigned char COMNumb);
extern void API_COM_SendStr(unsigned char COMNumb,unsigned char *data,unsigned char len);
extern void Trace(const char *format,...);
extern void Trace3(const char *format,...);

/**************************************@Keyboard & @Buzzer API*************************************************************************/
extern void API_KEY_KeyboardInit(void);
extern void API_KEY_KeyboardCtrl(unsigned char OpenOrClose);
extern unsigned char API_KEY_ReadKey(void);
extern void API_BUZZER_Buzzer(void);

/**************************************@Display API************************************************************************************/
extern void API_LCM_Init(void);
extern void API_LCM_ClearScreen(void);
extern void API_LCM_ClearArea(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
extern void API_LCM_DrawLine(unsigned char x,unsigned char y);
extern void API_LCM_SetXY(unsigned char x,unsigned char y);
extern unsigned char API_LCM_GetX(void);
extern unsigned char API_LCM_GetY(void);
extern void API_LCM_PutRMBSymbol(unsigned char x,unsigned char y);
extern void API_LCM_PutBMP(unsigned char x,unsigned char y,unsigned char W,unsigned char H,unsigned char const *data);
extern void API_LCM_Printf(unsigned char x,unsigned char y,unsigned char font,unsigned char Inv,char *format,...);

/**************************************@MDB Device API*********************************************************************************/
extern void API_MDB_ResetBus(void);
extern void API_MDB_UartInit(void);
/**************************************@MDB Coin Device *******************************************************************************/
extern unsigned char API_MDB_Reset_CoinDevice(void);
extern unsigned char API_MDB_Setup_CoinDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_TubeStatus_CoinDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_Poll_CoinDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_CoinType_CoinDevice(unsigned char CoinAcceptorCtrl,unsigned char CoinDispenseCtrl);
extern unsigned char API_MDB_ExpanIdentification_CoinDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_ExpanFeatureEnable_CoinDevice(unsigned char *Feature);
extern unsigned char API_MDB_ExpanPayout_CoinDevice(unsigned char value);
extern unsigned char API_MDB_ExpanPayoutStatus_CoinDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_ExpanPayoutValuePoll_CoinDevice(unsigned char *value);
extern unsigned char API_MDB_ExpanDiagStatus_CoinDevice(unsigned char *MainCode,unsigned char *SubCode);
/**************************************@MDB Bill Device ********************************************************************************/
extern unsigned char API_MDB_Reset_BillDevice(void);
extern unsigned char API_MDB_Setup_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_Security_BillDevice(unsigned char Y1,unsigned char Y2);
extern unsigned char API_MDB_Poll_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_BillType_BillDevice(unsigned int Y1Y2,unsigned int Y3Y4);
extern unsigned char API_MDB_Escrow_BillDevice(unsigned char Y1);
extern unsigned char API_MDB_Stacker_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_L1IdentificationWithoutOptBit_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_L2IdentificationWithOptBit_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_L2FeatureEnable_BillDevice(unsigned char *VmcData);
extern unsigned char API_MDB_Diagnostics_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_RecyclerSetup_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_RecyclerEnable_BillDevice(unsigned char *cmd,unsigned char *cmdlen);
extern unsigned char API_MDB_BillDispenseStatus_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_DispenseBill_BillDevice(unsigned char Y1,unsigned char Y2,unsigned char Y3);
extern unsigned char API_MDB_DispenseValue_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_PayoutStatus_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_PayoutValuePoll_BillDevice(unsigned char *ack,unsigned char *acklen);
extern unsigned char API_MDB_PayoutCancel_BillDevice(unsigned char *ack,unsigned char *acklen);
/**************************************@Cashless Device API****************************************************************************/

/**************************************@Vending Device API*****************************************************************************/
extern void API_VENDING_Init(void);
extern unsigned char API_VENDING_Vend(char Layer,char Channel);
/**************************************@FRAM Device API********************************************************************************/
extern void API_FRAM_Init(void);
extern unsigned char API_FRAM_ReadFram (unsigned char sa,unsigned int suba,unsigned char *s,unsigned int num);
extern unsigned char API_FRAM_WriteFram(unsigned char sa,unsigned int suba,unsigned char *s,unsigned int num);

/**************************************@RTC Device API*********************************************************************************/
extern void API_RTC_RtcInit(void);
extern void API_RTC_Read(unsigned char *date);
extern void API_RTC_Write(unsigned char *date);

#endif
/**************************************End Of File*************************************************************************************/
