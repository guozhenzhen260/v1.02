/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			Menu.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _MENU_H
#define _MENU_H
#define MAXLAGUAGENUBER	0x02	//定义支持的语言种类,目前支持中英文
/*****************************************用户交易菜单**************************************************/
/********************************************************************************************************
** @Define name:       welcome
********************************************************************************************************/
typedef struct
{
	char *welcome[MAXLAGUAGENUBER];
	char *UserTip[MAXLAGUAGENUBER];
	char *amount[MAXLAGUAGENUBER];
	char *column[MAXLAGUAGENUBER];
	char *price[MAXLAGUAGENUBER];
	char *dispense[MAXLAGUAGENUBER];
	char *takecolumn[MAXLAGUAGENUBER];
	char *soldout[MAXLAGUAGENUBER];
	char *payout[MAXLAGUAGENUBER];
	char *takemoney[MAXLAGUAGENUBER];
	char *IOU[MAXLAGUAGENUBER];
	char *end[MAXLAGUAGENUBER];
	char *changeempty[MAXLAGUAGENUBER];
	char *error[MAXLAGUAGENUBER];
} UIMEMU;
extern const UIMEMU UIMenu;

/*********************************************维护菜单**************************************************/

/********************************************************************************************************
** @Define name:       LoginUI Parameter Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Prompt[MAXLAGUAGENUBER];
	char *Error[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} LOGINUI;
extern const LOGINUI LoginUI;
/********************************************************************************************************
** @Define name:       MaintenUI Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *DeviceInfo[MAXLAGUAGENUBER];
	char *DeviceConfig[MAXLAGUAGENUBER];
	char *DeviceTest[MAXLAGUAGENUBER];
	char *TradeSet[MAXLAGUAGENUBER];
	char *TradeLOG[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} MAITENMENU;
extern const MAITENMENU MaintenUI;
/********************************************************************************************************
** @Define name:       DevInfo Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Version[MAXLAGUAGENUBER];
	char *DevID[MAXLAGUAGENUBER];
	char *DevList[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} DEVINFO;
extern const DEVINFO DevInfo;
/********************************************************************************************************
** @Define name:       DevConfig Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Bill[MAXLAGUAGENUBER];
	char *Coin[MAXLAGUAGENUBER];
	char *Cashless[MAXLAGUAGENUBER];
	char *Lan[MAXLAGUAGENUBER];
	char *Rtc[MAXLAGUAGENUBER];
		char *RtcTheme[MAXLAGUAGENUBER];
		char *RtcYear[MAXLAGUAGENUBER];
		char *RtcMon[MAXLAGUAGENUBER];
		char *RtcDay[MAXLAGUAGENUBER];
		char *RtcHour[MAXLAGUAGENUBER];
		char *RtcMin[MAXLAGUAGENUBER];
	char *Note[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} DEVCONFIG;
extern const DEVCONFIG DevConfig;
/********************************************************************************************************
** @Define name:       DevTest Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *TBill[MAXLAGUAGENUBER];
	char *TCoin[MAXLAGUAGENUBER];
	char *TCashless[MAXLAGUAGENUBER];
	char *TGdsChnl[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} DEVTEST;
extern const DEVTEST DevTest;
/********************************************************************************************************
** @Define name:       BillTest Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Bill[MAXLAGUAGENUBER];
	char *BillMsg0[MAXLAGUAGENUBER];
	char *BillMsg1[MAXLAGUAGENUBER];
	char *BillMsg2[MAXLAGUAGENUBER];
	char *BillMsg3[MAXLAGUAGENUBER];
	char *BillMsg4[MAXLAGUAGENUBER];
	char *BillMsg5[MAXLAGUAGENUBER];
	char *BillMsg6[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} BILLDEVTEST;
extern const BILLDEVTEST BillTest;
/********************************************************************************************************
** @Define name:       CoinTest Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Coin[MAXLAGUAGENUBER];
	char *CoinMsg0[MAXLAGUAGENUBER];
	char *CoinMsg1[MAXLAGUAGENUBER];
	char *CoinMsg2[MAXLAGUAGENUBER];
	char *CoinMsg3[MAXLAGUAGENUBER];
	char *CoinMsg4[MAXLAGUAGENUBER];
	char *CoinMsg5[MAXLAGUAGENUBER];
	char *CoinMsg6[MAXLAGUAGENUBER];
	char *CoinMsg7[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} COINDEVTEST;
extern const COINDEVTEST CoinTest;
/********************************************************************************************************
** @Define name:       CashlessTest Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Cashless[MAXLAGUAGENUBER];
	char *CashlessMsg0[MAXLAGUAGENUBER];
	char *CashlessMsg1[MAXLAGUAGENUBER];
	char *CashlessMsg2[MAXLAGUAGENUBER];
	char *CashlessMsg3[MAXLAGUAGENUBER];
	char *CashlessMsg4[MAXLAGUAGENUBER];
	char *CashlessMsg5[MAXLAGUAGENUBER];
	char *CashlessMsg6[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} CASHLESSDEVTEST;
extern const CASHLESSDEVTEST CashlessTest;
/********************************************************************************************************
** @Define name:       ChannelTest Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Channel[MAXLAGUAGENUBER];
	char *ChannelMsg0[MAXLAGUAGENUBER];
	char *ChannelMsg1[MAXLAGUAGENUBER];
	char *ChannelMsg2[MAXLAGUAGENUBER];
	char *ChannelMsg3[MAXLAGUAGENUBER];
	char *ChannelMsg4[MAXLAGUAGENUBER];
	char *ChannelMsg5[MAXLAGUAGENUBER];
	char *ChannelMsg6[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} CHANNELDEVTEST;
extern const CHANNELDEVTEST ChannelTest;
/********************************************************************************************************
** @Define name:       TradeConfig Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *GdsChnl[MAXLAGUAGENUBER];
	char *BillEscrow[MAXLAGUAGENUBER];
	char *BillType[MAXLAGUAGENUBER];
	char *CoinType[MAXLAGUAGENUBER];
	char *AddGoods[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} TRADECONFIG;
extern const TRADECONFIG TradeConfig;
/********************************************************************************************************
** @Define name:       TradeConfig->GC Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *GdNumb[MAXLAGUAGENUBER];
	char *GdNumbL[MAXLAGUAGENUBER];
	char *GdNumbLC[MAXLAGUAGENUBER];
	char *GdCoinfig[MAXLAGUAGENUBER];
	char *Enale[MAXLAGUAGENUBER];
	char *MaxCap[MAXLAGUAGENUBER];
	char *Price[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} TRADECONFIG_GD;
extern const TRADECONFIG_GD TradeConfigGC;
/********************************************************************************************************
** @Define name:       TradeConfig->Bill Escrow set
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *BillChl[MAXLAGUAGENUBER];
	char *Ch0[MAXLAGUAGENUBER];
	char *Ch1[MAXLAGUAGENUBER];
	char *Ch2[MAXLAGUAGENUBER];
	char *Ch3[MAXLAGUAGENUBER];
	char *Ch4[MAXLAGUAGENUBER];
	char *Ch5[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} TRADECONFIG_BILL_ESCROW;
extern const TRADECONFIG_BILL_ESCROW TradeConfigBillEscrow;
/********************************************************************************************************
** @Define name:       TradeConfig->Bill Channel set
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *BillChl[MAXLAGUAGENUBER];
	char *Ch0[MAXLAGUAGENUBER];
	char *Ch1[MAXLAGUAGENUBER];
	char *Ch2[MAXLAGUAGENUBER];
	char *Ch3[MAXLAGUAGENUBER];
	char *Ch4[MAXLAGUAGENUBER];
	char *Ch5[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} TRADECONFIG_BILL_CHL;
extern const TRADECONFIG_BILL_CHL TradeConfigBillChannel;
/********************************************************************************************************
** @Define name:       TradeConfig->Coin Channel set
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *CoinChl[MAXLAGUAGENUBER];
	char *Chl0[MAXLAGUAGENUBER];
	char *Chl1[MAXLAGUAGENUBER];
	char *Chl2[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} TRADECONFIG_COIN_CHL;
extern const TRADECONFIG_COIN_CHL TradeConfigCoinChannel;
/********************************************************************************************************
** @Define name:       TradeConfig->Add Goods
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *All[MAXLAGUAGENUBER];
	char *Layer[MAXLAGUAGENUBER];
	char *Channel[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
	//子菜单
	char *ALL_YON[MAXLAGUAGENUBER];
	char *EnterLayer[MAXLAGUAGENUBER];
	char *EnterLayer0[MAXLAGUAGENUBER];
	char *AddLayerYON[MAXLAGUAGENUBER];
	char *EnterChannel[MAXLAGUAGENUBER];
	char *EnterChannel0[MAXLAGUAGENUBER];
	char *EnterChannel00[MAXLAGUAGENUBER];
	char *EnterNumb[MAXLAGUAGENUBER];
	char *EnterNumb0[MAXLAGUAGENUBER];
	char *NoteP[MAXLAGUAGENUBER];
	char *NoteOk[MAXLAGUAGENUBER];
	char *CancelOrEnter[MAXLAGUAGENUBER];
	char *CurrentNumb[MAXLAGUAGENUBER];
} TRADECONFIG_ADD_GOODS;
extern const TRADECONFIG_ADD_GOODS TradeConfigAddGoods;
/********************************************************************************************************
** @Define name:       TradeLog Define
********************************************************************************************************/
typedef struct
{
	char *Theme[MAXLAGUAGENUBER];
	char *Coin[MAXLAGUAGENUBER];
	char *Bill[MAXLAGUAGENUBER];
	char *Cashless[MAXLAGUAGENUBER];
	char *TotalPay[MAXLAGUAGENUBER];
	char *TtlTradNum[MAXLAGUAGENUBER];
	char *Exit[MAXLAGUAGENUBER];
} TRADELOG;
extern const TRADELOG TradeLog;
#endif

/**************************************End Of File*******************************************************/
