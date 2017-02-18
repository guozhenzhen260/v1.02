/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			Menu.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "Menu.h"
/*****************************************用户交易菜单**************************************************/
/********************************************************************************************************
** @Define name:       welcome
********************************************************************************************************/
const UIMEMU UIMenu =
{
	{"欢迎光临","welcome"},
	{"购买请投币","Pls insert cash"},
	{"您已投入的金额:","You Pay in:"},
	{"请输入商品编号:%s","Merchandise NO:%s"},
	{"%s 商品价格: %s","%s U.P.: %s"},
	{"正在出货...","Vending..."},
	{"请取走商品","Take Merchandise away"},
	{"商品已售完","Sold out"},
	{"找零:%s","Changer:%s"},
	{"请取走零钱","Take coin away:"},
	{"本机零币不够,欠款: %s","IOU: %s"},
	{"谢谢惠顾","Thank you"},
	{"本机无零币","Unable to Give Change Now"},
	{"暂停服务","Stop service"}
};
/*********************************************维护菜单**************************************************/
/*********************************************************************************************************
** @Menu Define name:   LoginUI
*********************************************************************************************************/
const LOGINUI LoginUI =
{
	{"登陆维护","LOGIN"},
	{"输入密码:","Enter Password:"},
	{"密码错误","Password ERR"},
	{"按<取消>退出","Press 'cancel' to exit"}
};
/*********************************************************************************************************
** @Menu Define name:   MaintenUI
*********************************************************************************************************/
const MAITENMENU MaintenUI =
{
	{"运营维护","Maint"},
	{"A.设备信息","A.Dev Info"},
	{"B.设备配置","B.Dev Config"},
	{"C.设备检测","C.Dev Test"},
	{"D.交易设置","D.Trade Config"},
	{"E.交易记录","E.Trade LOG"},
	{"按<取消>退出","Press 'cancel' to exit"}
};
/*********************************************************************************************************
** @Menu Define name:   DevInfo
*********************************************************************************************************/
const DEVINFO DevInfo =
{
	{"运营维护->设备信息","Maint->Dev Info"},
	{"A.软件版本:","A.Version:"},
	{"B.设备序列号:","B.VMC ID:"},
	{"C.故障码:","C.Err code:"},
	{"按<取消>返回","Press 'cancel' to exit"}
};
/*********************************************************************************************************
** @API Define name:   DevConfig
*********************************************************************************************************/
const DEVCONFIG DevConfig =
{
	{"运营维护->设备配置","Maint->Dev Config"},
	{"A.是否开启纸币器:%d","A.Bill validator:%d"},
	{"B.是否开启硬币器:%d","B.Coin acceptor/changer:%d"},
	{"C.是否开启读卡器:%d","C.Cashless:%d"},
	{"D.语言设置:%d","D.Language:%d"},
	{"E.时间设置","E.RTC Set"},
		{"运营维护->设备配置->时间","Maint->Dev Config->RTC"},
		{"A.年:%d","A.Year:%d"},
		{"B.月:%d","B.Mon:%d"},
		{"C.日:%d","C.Day:%d"},
		{"D.时:%d","D.Hour:%d"},
		{"E.分:%d","E.Min:%d"},		
	{"提示:0-Close,1-Open","Note:0-Close,1-Open"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   DevTest
*********************************************************************************************************/
const DEVTEST DevTest =
{
	{"运营维护->设备测试","Maint->Dev Test"},
	{"A.纸币器测试","A.Bill validator Test"},
	{"B.硬币器测试","B.Coin acceptor/changer Test"},
	{"C.读卡器测试","C.Cashless Test"},
	{"D.货道测试","D.Goods channel Test"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   BillTest
*********************************************************************************************************/
const BILLDEVTEST BillTest =
{
	{"运营维护->设备测试->纸币器","Maint->Dev Test->Bill"},
	{"<--纸币器测试-->","<--Bill validator Test-->"},
	{"纸币未开启","Bill validator is not config"},					//msg0
	{"未发现纸币器,请检查硬件","Not found,pls check hardware"},		//msg1
	{"硬件正常,请投币","Check Ok,Pls insert bill"},					//msg2
	{"钞箱满,请取走纸币","Stacker is full,Pls remove bill"},		//msg3
	{"纸币器故障,请检查硬件","validator ERR,pls check hardware"},	//msg4
	{"该面额禁收","Bill Type is Disable"},							//msg5
	{"纸币：收币--%s","OK:Type currency--%s"},				//msg6
	{"按<取消>退出测试","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   CoinTest
*********************************************************************************************************/
const COINDEVTEST CoinTest =
{
	{"运营维护->设备测试->硬币器","Maint->Dev Test->Coin"},
	{"<--硬币器测试-->","<--Bill validator Test-->"},
	{"纸币未开启","Bill validator is not config"},					//msg0
	{"未发现纸币器,请检查硬件","Not found,pls check hardware"},		//msg1
	{"硬件正常,请投币","Check Ok,Pls insert bill"},					//msg2
	{"钞箱满,请取走纸币","Stacker is full,Pls remove bill"},		//msg3
	{"纸币器故障,请检查硬件","validator ERR,pls check hardware"},	//msg4
	{"该面额禁收","Bill Type is Disable"},							//msg5
	{"硬币：收币--%s","OK:Type currency--%s"},				//msg6
	{"硬币：找零--%s","OK:Payout currency--%s"},				//msg7
	{"按<取消>退出测试","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   CashlessTest
*********************************************************************************************************/
const CASHLESSDEVTEST CashlessTest =
{
	{"运营维护->设备测试->读卡器","Maint->Dev Test->Cashless"},
	{"<--读卡器测试-->","<--Bill validator Test-->"},
	{"纸币未开启","Bill validator is not config"},					//msg0
	{"未发现纸币器,请检查硬件","Not found,pls check hardware"},		//msg1
	{"硬件正常,请投币","Check Ok,Pls insert bill"},					//msg2
	{"钞箱满,请取走纸币","Stacker is full,Pls remove bill"},		//msg3
	{"纸币器故障,请检查硬件","validator ERR,pls check hardware"},	//msg4
	{"该面额禁收","Bill Type is Disable"},							//msg5
	{"正常：通道--%d,面额--","OK:Type--%d;currency--"},				//msg6
	{"按<取消>退出测试","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   ChannelTest
*********************************************************************************************************/
const CHANNELDEVTEST ChannelTest=
{
	{"运营维护->设备测试->货道","Maint->Dev Test->GC"},
	{"<--货道测试-->","<--Bill validator Test-->"},
	{"纸币未开启","Bill validator is not config"},					//msg0
	{"未发现纸币器,请检查硬件","Not found,pls check hardware"},		//msg1
	{"硬件正常,请投币","Check Ok,Pls insert bill"},					//msg2
	{"钞箱满,请取走纸币","Stacker is full,Pls remove bill"},		//msg3
	{"纸币器故障,请检查硬件","validator ERR,pls check hardware"},	//msg4
	{"该面额禁收","Bill Type is Disable"},							//msg5
	{"正常：通道--%d,面额--","OK:Type--%d;currency--"},				//msg6
	{"按<取消>退出测试","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfig
*********************************************************************************************************/
const TRADECONFIG TradeConfig =
{
	{"运营维护->交易设置","Maint->Trade Config"},
	{"A.货道配置","A.Channel config"},
	{"B.纸币暂存配置","B.Bill Escrow"},
	{"C.纸币通道配置","C.Bill Type config"},
	{"D.硬币通道配置","D.Coin Type config"},
	{"E.添货","E.Add Goods"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigGC
*********************************************************************************************************/
const TRADECONFIG_GD TradeConfigGC =
{
	{"运营维护->交易设置->货道","Maint->Trade Config->GC"},
	{"请输入货道编号:","Pls Enter GD NO:"},
	{"请输入货道编号:%C","Pls Enter GD NO:%C"},
	{"请输入货道编号:%C%d","Pls Enter GD NO:%C%d"},
	{"配置货道%C%D:","Config GD %C%d:"},
	{"A.是否开启货道:%d","A.Enalble GC:%d"},
	{"B.配置最大存货量:%d","B.Set MAX Cap:%d"},
	{"C.配置货道价格:%d.%d","C.Set Price:%d.%d"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigBillEscrow
*********************************************************************************************************/
const TRADECONFIG_BILL_ESCROW TradeConfigBillEscrow =
{
	{"运营维护->交易设置->纸币暂存","Maint->Trade Config->Escrow"},
	{"纸币暂存配置:","Bill Escrow list:"},
	{"A.通道0:%d","A.Chl0:%d"},
	{"B.通道1:%d","B.Chl1:%d"},
	{"C.通道2:%d","C.Chl2:%d"},
	{"D.通道3:%d","D.Chl3:%d"},
	{"E.通道4:%d","E.Chl4:%d"},
	{"F.通道5:%d","F.Chl5:%d"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigBillChannel
*********************************************************************************************************/
const TRADECONFIG_BILL_CHL TradeConfigBillChannel =
{
	{"运营维护->交易设置->纸币通道","Maint->Trade Config->Bill"},
	{"纸币通道设置列表:","Bill Chl EA list:"},
	{"A.通道0: %d","A.Chl0: %d"},
	{"B.通道1: %d","B.Chl1: %d"},
	{"C.通道2: %d","C.Chl2: %d"},
	{"D.通道3: %d","D.Chl3: %d"},
	{"E.通道4: %d","E.Chl4: %d"},
	{"F.通道5: %d","F.Chl5: %d"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigCoinChannel
*********************************************************************************************************/
const TRADECONFIG_COIN_CHL TradeConfigCoinChannel =
{
	{"运营维护->交易设置->硬币通道","Maint->Trade Config->Coin"},
	{"硬币通道设置列表:","Coin Chl EA list:"},
	{"A.通道0:%d","A.Chl0:%d"},
	{"B.通道1:%d","B.Chl2:%d"},
	{"C.通道2:%d","C.Chl4:%d"},
	{"按<取消>返回","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigCoinChannel
*********************************************************************************************************/
const TRADECONFIG_ADD_GOODS TradeConfigAddGoods =
{
	{"运营维护->交易设置->添货","Maint->Trade Config->Add"},
	{"A.全部添满","A.All"},
	{"B.按层添满","B.Layer"},
	{"C.按单货道添货","C.Channel"},
	{"按<取消>返回","Press 'cancel' back"},
	//子菜单
	{"全部添满货道?","Add All?"},
	{"请输入层号:","Enter layer:"},
	{"请输入层号:%C","Enter layer:%C"},
	{"确认添满%C层?","Add layer %C full?"},
	{"请输入货道号:","Enter layer:"},
	{"请输入货道号:%C","Enter layer:%C"},
	{"请输入货道号:%C%d","Enter layer:%C%d"},
	{"请输入货道%C%d数量:","Enter<%C%d>Numb:"},
	{"请输入货道%C%d数量:%d","Enter<%C%d>Numb:%d"},
	{"已按系统设定值添满","Set as system def."},
	{"添货成功","Set Ok"},
	{"A.取消  B.确认","A.Cancel  B.OK"},
	{"当前货道%C%d数量:%d","current<%C%d>Numb:%d"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfig
*********************************************************************************************************/
const TRADELOG TradeLog =
{
	{"运营维护->交易记录","Maint->Trade Log"},
	{"1.硬币-收入:%d.%d,支出:%d.%d","1.Coin Payin:%d.%d,out:%d.%d"},
	{"2.纸币-收入:%d.%d,支出:%d.%d","2.Bill Payin:%d.%d,out:%d.%d"},
	{"3.非现金收入:%d.%d","3.Cashless Payin:%d.%d"},
	{"4.总收入:%d.%d,总支出:%d.%d","4.Total IN:%d.%d,OUT:%d.%d"},
	{"5.交易次数-成功:%d,失败:%d","5.TradeNum-Ok:%d,Err:%d"},
	{"按<取消>返回    按<确定>清除","Press 'cancel' back      'enter'clean"}
};
/**************************************End Of File*******************************************************/
