/***********************************************Copyright (c)************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info------------------------------------------------------------------------------------
 ** @Filename:			vendservice.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *************************************************************************************************************************************/
#include "..\API\API.H"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "MdbBillDeviceOperation.h"
#include "MdbCashlessDeviceOperation.h"
#include "MdbCoinDeviceOperation.h"
#include "Menu.h"
#include "Log.h"

#define DEBUG_VENDSERVICE							//调试信息串口0输出

#define VMC_FREE		  0                         //空闲状态
#define VMC_CHAXUN	      1							//货道查询状态
#define VMC_SALE 	      3							//交易状态
#define VMC_XUANHUO    	  5							//选货
#define VMC_READVENDFAIL  9							//读卡器扣款失败
#define VMC_CHUHUO  	  10						//出货
#define VMC_QUHUO  	  	  11						//取货
#define VMC_PAYOUT 	      13						//找零
#define VMC_END 	      14						//结束交易
#define VMC_ERROR		  15                        //故障状态

char     	ChannelNum[3] = {0};					//选择货道值
uint8_t	 	vmcStatus = VMC_FREE;					//当前状态
uint8_t	 	vmcChangeLow = 0;						//零钱是否不够找,1不够找,0够找
uint8_t	    IsTuibi = 0;							//是否按下退币按钮,1是
uint8_t	 	vmcEorr = 0;							//本机故障,1故障

uint32_t 	g_coinAmount = 0;						//当前投币硬币总金额
uint32_t 	g_billAmount = 0;    					//当前压入纸币总金额
uint32_t 	g_readerAmount = 0;						//当前读卡器总金额

uint16_t	vmcColumn = 0;							//当前购买的商品编号
uint32_t 	vmcPrice  = 0;							//当前购买的商品单价
//uint8_t		LogicChannel[6][8];						//逻辑货道,每次重启交易需要重新计算 

static void VendingService(void);
static void VendingIdle(void);
void Vend_DispSalePage(void);
//static void SetupLogicChannel(void);				//重映射货道,即交易时送出逻辑货道并映射到物理货道

/***************************************************************************************************************************************
** @APP Function name:   VendService
** @APP Descriptions:	 交易主任务
** @APP Input para:      pvParameters:no used
** @APP retrun para:     None
***************************************************************************************************************************************/
void VendService(void *pvParameters)
{
	pvParameters = pvParameters;
	API_SYSTEM_PinFuctionSelect();
	API_VENDING_Init();
	API_RTC_RtcInit();
	API_FRAM_Init();
	API_SYSTEM_TimerInit(0);
	API_SYSTEM_TimerInit(1);
	API_COM_Init(0,115200);		//Debug输出
	API_MDB_UartInit();
	API_LCM_Init();
	API_KEY_KeyboardInit();
	API_BUZZER_Buzzer();
	InitVmcParam();
	InitTradeParam();
	InitTotalLog();
	VMCParam.VMCMode = VMC_MODE_VEND;
	while(1)
	{
		switch(VMCParam.VMCMode)
		{
			case VMC_MODE_VEND:
				VendingService();
				break;
			case VMC_MODE_MAIN:
				VendingIdle();
				break;
		}
		vTaskDelay(2);
	}
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_GetAmountMoney
** @APP Descriptions:	 获取投币总金额;g_coinAmount硬币投币金额,g_billAmount纸币投币金额,g_readerAmount读卡器获得金额
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
uint32_t Vend_GetAmountMoney(void)
{	
	return (g_coinAmount + g_billAmount + g_readerAmount);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_IsTuibiAPI
** @APP Descriptions:	 是否有按下找零器
** @APP Input para:      None
** @APP retrun para:     1按下,0没按下
***************************************************************************************************************************************/
uint8_t Vend_IsTuibiAPI(void)
{
	if(IsTuibi == 1)
	{
		IsTuibi=0;
		return 1;
	}
	else
	{
		return 0;
	}
}
/***************************************************************************************************************************************
** @APP Function name:   BillCoinEnable
** @APP Descriptions:	 使能禁能纸币器硬币器
** @APP Input para:      billCtr控制纸币器=1,使能,2禁能,0不操作;coinCtr控制硬币器=1,使能,2禁能,0不操作;readerCtr控制读卡器=1,使能,2禁能,0不操作
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_BillCoinCtr(uint8_t billCtr,uint8_t coinCtr,uint8_t readerCtr)
{
	uint32_t InValue=0;
	uint8_t  billOptBack = 0;
	uint8_t Billtype = 0;
	//1.纸币器控制
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		switch(billCtr)
		{
			case 1:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Enable BillDevice",__FILE__,__LINE__);
				#endif
				BillDevProcess(&InValue,&Billtype,MBOX_BILLENABLEDEV,&billOptBack);
				break;
			case 2:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Disable BillDevice",__FILE__,__LINE__);
				#endif
				BillDevProcess(&InValue,&Billtype,MBOX_BILLDISABLEDEV,&billOptBack);
				break;
			default:break;	
		}
	}
	//2.硬币器控制
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		switch(coinCtr)
		{
			case 1:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Enable CoinDevice",__FILE__,__LINE__);
				#endif
				MdbCoinTypeEanbleOrDisable(1,1);
				break;
			case 2:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Disable CoinDevice",__FILE__,__LINE__);
				#endif
				MdbCoinTypeEanbleOrDisable(0,1);
				break;
			default:break;	
		}
	}
	//3.读卡器控制
	if(VMCParam.MdbCashlessDeviceEAB == 0x01)
	{
		switch(readerCtr)
		{
			case 1:
				//ReaderDevEnableAPI();
				break;
			case 2:
				//ReaderDevDisableAPI();
				break;
			default:break;	
		}
	}
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_ClearDealPar
** @APP Descriptions:	 回到空闲状态前，先清除交易的数据
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_ClearDealPar(void)
{
	memset(ChannelNum,0,sizeof(ChannelNum));
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_CLrBusinessText
** @APP Descriptions:	 清交易时提示语言
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_CLrBusinessText(void)
{	
	API_LCM_ClearArea(0,12,239,15);
	vTaskDelay(2);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_CLrBusinessText
** @APP Descriptions:	 清空闲状态时显示的时间屏幕信息
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_CLrFreeText(void)
{	
	API_LCM_ClearArea(0,0,239,11);
	vTaskDelay(2);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispFreePage
** @Descriptions Func:	 空闲页面,vmcEorr代表进入故障页面显示,vmcChangeLow代表进入缺币页面显示
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispFreePage(void)
{
	unsigned char DisplayX;	//居中X坐标	
	API_LCM_ClearArea(0,6,239,7);
	vTaskDelay(2);	
	if(vmcEorr == 1)
	{
		DisplayX = (239 - (strlen(UIMenu.error[VMCParam.Language]) * 8)) / 2;
		API_LCM_Printf(DisplayX,6,0,0,UIMenu.error[VMCParam.Language]);
	}
	else if(vmcChangeLow == 1) 
	{
		DisplayX = (239 - (strlen(UIMenu.changeempty[VMCParam.Language]) * 8)) / 2;
		API_LCM_Printf(DisplayX,6,0,0,UIMenu.changeempty[VMCParam.Language]);
	}
	else
	{
		DisplayX = (239 - (strlen(UIMenu.welcome[VMCParam.Language]) * 8)) / 2;
		API_LCM_Printf(DisplayX,6,0,0,UIMenu.welcome[VMCParam.Language]);
	}
	vTaskDelay(2);
	API_RTC_Read((void *)VMCParam.Rtc);
	API_LCM_Printf(55,8,0,0,"%04D/%02D/%02D %02D:%02D",((((unsigned int)VMCParam.Rtc[5]) << 8) + VMCParam.Rtc[6]),VMCParam.Rtc[4],VMCParam.Rtc[3],VMCParam.Rtc[2],VMCParam.Rtc[1]);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispChaxunPage
** @APP Descriptions:	 输入查询货道的页面
** @APP Input para:      keyValue代表按键值
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispChaxunPage(uint8_t *keyValue)
{
	unsigned char layer = 0x00,channel = 0x00;
	char    *pstr;
	char	strMoney[10];
	static unsigned char channelInput,LayerOk;
	//取消
	if(*keyValue == '>')
	{
		Vend_ClearDealPar();
		Vend_CLrBusinessText();
		vmcStatus = VMC_FREE;
		LayerOk = 0x00;
		channelInput = 0x00;
		return;
	}
	if(LayerOk == 0x00)
	{
		if((*keyValue >= 'A') && (*keyValue <= 'F'))
		{
			LayerOk = 0x01;
			channelInput = 0x00;
			ChannelNum[channelInput++] = *keyValue;
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],ChannelNum);
		}
		else
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"--");
	}
	else
	{
		if((*keyValue >= '0') && (*keyValue <= '9'))
		{
			ChannelNum[channelInput++] = *keyValue;
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],ChannelNum);
			vTaskDelay(100);
		}
		else
		{
			LayerOk = 0x00;
			channelInput = 0x00;
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"--");
		}
	}
	if(channelInput >= 2)
	{
		LayerOk = 0x00;
		channelInput = 0x00;
		layer = ChannelNum[0];
		channel = ChannelNum[1] - 0x30;
		vmcColumn = ((layer - 0x41)*8 + (channel - 1));
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:%s column%ld=%d,%d,%d\r\n",__FILE__,__LINE__,ChannelNum,vmcColumn,VMCParam.GoodsChannelArray[vmcColumn],TradeParam.GoodsPrice[vmcColumn],TradeParam.RemainGoods[vmcColumn]);
		#endif
		if((VMCParam.GoodsChannelArray[vmcColumn] == 0) || (TradeParam.GoodsPrice[vmcColumn] == 0))	//货道未开启 或 货道单价为0
		{
			Vend_ClearDealPar();
			Vend_CLrBusinessText();
			vmcStatus = VMC_FREE;
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Channel is disable or Price = 0",__FILE__,__LINE__);
			#endif
		}
		else if(TradeParam.RemainGoods[vmcColumn] == 0)	//货道当前存货数量
		{
			Vend_ClearDealPar();
			Vend_CLrBusinessText();
			vmcStatus = VMC_FREE;
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Goods is sold out",__FILE__,__LINE__);
			#endif
		}
		else
		{
			vmcPrice=(uint32_t)TradeParam.GoodsPrice[vmcColumn]*10;//以分为单位处理
			pstr = PrintfMoney(vmcPrice);
			strcpy(strMoney, pstr);
			Vend_CLrBusinessText();
			API_LCM_DrawLine(0,12);
			API_LCM_Printf(7,13,0,0,UIMenu.price[VMCParam.Language],ChannelNum,strMoney);
			Vend_DispSalePage();
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:User need pay in more",__FILE__,__LINE__);
			#endif				
			API_SYSTEM_TimerChannelSet(3,30 * 100);
			vmcStatus = VMC_SALE;
		}
	}	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispSalePage
** @APP Descriptions:	 投币进行交易页面
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispSalePage(void)
{
	char    *pstr;
	char	strMoney[10];
	pstr = PrintfMoney(Vend_GetAmountMoney());
	strcpy(strMoney,pstr);
	Vend_CLrFreeText();
	vTaskDelay(2);
	API_LCM_Printf(7,1,0,0,UIMenu.amount[VMCParam.Language]);
	API_LCM_PutRMBSymbol(31,5);//24x32 bmp
	API_LCM_Printf(71,5,1,0,"%S",strMoney);//dot 32x16	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispChuhuoPage
** @APP Descriptions:	 出货界面
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispChuhuoPage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.dispense[VMCParam.Language]);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispQuhuoPage
** @APP Descriptions:	 取货界面
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispQuhuoPage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.takecolumn[VMCParam.Language]);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   DispChhuoFailPage
** @APP Descriptions:	 出货失败界面
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void DispChhuoFailPage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.soldout[VMCParam.Language]);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispPayoutPage
** @APP Descriptions:	 正在找零界面
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispPayoutPage(void)
{
	char    *pstr;
	char	strMoney[10];
	Vend_CLrBusinessText();
	pstr = PrintfMoney(Vend_GetAmountMoney());
	strcpy(strMoney, pstr);
	API_LCM_Printf(7,13,0,0,UIMenu.payout[VMCParam.Language],strMoney);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispQuChangePage
** @APP Descriptions:	 取零钱
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispQuChangePage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.takemoney[VMCParam.Language]);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispIOUPage
** @APP Descriptions:	 找零失败
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispIOUPage(uint32_t debtMoney)
{
	char    *pstr;
	char	strMoney[10];
	Vend_CLrBusinessText();
	pstr = PrintfMoney(debtMoney);
	strcpy(strMoney, pstr);
	API_LCM_Printf(7,13,0,0,UIMenu.IOU[VMCParam.Language],strMoney);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispEndPage
** @APP Descriptions:	 交易结束
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispEndPage(void)
{
	API_LCM_ClearScreen();	
	vTaskDelay(20);
	API_LCM_Printf(((240 - (strlen(UIMenu.end[VMCParam.Language]) * 8)) / 2),7,0,0,UIMenu.end[VMCParam.Language]);
	vTaskDelay(300*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_UpdateTubeMoney
** @APP Descriptions:	 更新硬币器剩余硬币金额,当小于3元时,关闭现金设备并将缺币位置位。当大于3元时,开启现金设备并清除缺币位
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_UpdateTubeMoney(void)
{
	uint32_t coinMoney=0;	
	uint8_t i;
	MdbCoinGetTubeStatus();
	for(i=0;i<16;i++)
	{
		coinMoney+=MDBCoinDevice.CoinTypePresentInTube[i]*MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[i]);
	}
	#ifdef DEBUG_VENDSERVICE
	Trace("\r\n%S,%d:Update coin in tube:%d",__FILE__,__LINE__,coinMoney);
	#endif
	//零钱小于3元
	if(coinMoney < 300)
	{
		if(vmcChangeLow == 0)
		{
			vmcChangeLow = 1;
			Vend_BillCoinCtr(2,2,0);
		}
	}
	else
	{
		if(vmcChangeLow == 1)
		{
			vmcChangeLow=0;
			Vend_BillCoinCtr(1,1,0);
		}
	}
	vTaskDelay(10);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_GetMoney
** @APP Descriptions:	 硬币器轮询,纸币器轮询，获取用户投币金额
** @APP Input para:      None
** @APP retrun para:     1代表收到投币信息,0没有
***************************************************************************************************************************************/
uint8_t Vend_GetMoney(void)
{
	uint32_t InValue=0,billEscrow=0;
	uint8_t  billOptBack = 0;
	uint8_t Billtype = 0;
	//1.硬币轮询
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		vTaskDelay(20);
		CoinDevProcess(&InValue,&Billtype,&billOptBack);
		if(InValue > 0)
		{			
			g_coinAmount += InValue;
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Current Coin Pay in:%ld",__FILE__,__LINE__,InValue);
			#endif
			LogGetMoneyAPI(InValue,1);//记录日志
			return 1;		
		}
		//判断是否按下硬币器退币按键
		if(billOptBack == 1)
		{
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:User press down escrow request",__FILE__,__LINE__);
			#endif
			API_BUZZER_Buzzer();
			IsTuibi = 1;
		}
	}
	//2.纸币轮询
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		//判断是否有纸币投入
		vTaskDelay(20);
		BillDevProcess(&InValue,&Billtype,0,&billOptBack);
		if(InValue > 0)
		{
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Current Bin Pay in:%ld",__FILE__,__LINE__,InValue);
			#endif			
			BillDevProcess(&billEscrow,&Billtype,MBOX_BILLESCROW,&billOptBack);
			if(billOptBack == 2)
			{
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:EscrowSuccess:%ld",__FILE__,__LINE__,InValue);
				#endif					
				g_billAmount += InValue;
				LogGetMoneyAPI(InValue,2);//记录日志
				InValue = 0;
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_ChangerMoney
** @APP Descriptions:	 找零
** @APP Input para:      None
** @APP retrun para:     返回金额，找零失败返回欠款金额，找零成功返回0
***************************************************************************************************************************************/
uint32_t Vend_ChangerMoney(void)
{	
	uint32_t tempmoney=0,backmoney=0;
	#ifdef DEBUG_VENDSERVICE
	Trace("\r\n%S,%d:App Changer:%d",__FILE__,__LINE__,Vend_GetAmountMoney());
	#endif
	tempmoney = Vend_GetAmountMoney();
	//找零硬币
	if(Vend_GetAmountMoney())
	{		
		ChangePayoutProcessLevel3(Vend_GetAmountMoney(),&backmoney);	
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:App Pay back money:%d",__FILE__,__LINE__,backmoney);
		#endif
	}	
	LogChangeAPI(backmoney);//记录日志
	//找零失败
	if(tempmoney > backmoney)
	{		
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:App change Fail",__FILE__,__LINE__);
		#endif		
		g_coinAmount = 0;
		g_billAmount = 0;
		return  tempmoney-backmoney;
	}
	//找零成功
	else
	{
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:App change Sucess",__FILE__,__LINE__);
		#endif	
		g_coinAmount = 0;
		g_billAmount = 0;
		return 0;
	}
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_SaleCostMoney
** @APP Descriptions:	 出货后扣除金额
** @APP Input para:      PriceSale商品金额
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_SaleCostMoney(uint32_t PriceSale)
{	
	//扣除单价
	if(PriceSale)
	{
		/*//向读卡器发送交易成功
		if(g_readerAmount > 0)
		{
			TraceReader("\r\n AppCHuhuoSucc");
			ReaderDevVendoutResultAPI(1);
			g_readerAmount -= PriceSale;
			PriceSale = 0;
		}		
		else 
		*/
		if(PriceSale >= g_billAmount)
		{
			PriceSale -= g_billAmount;
			g_billAmount = 0;
		}
		else
		{
			g_billAmount -= PriceSale;
			PriceSale = 0;
		}
	}
	if(PriceSale)
	{
		g_coinAmount -= PriceSale;					
	}	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_IsErrorState
** @APP Descriptions:	 是否进入故障状态
** @APP Input para:      None
** @APP retrun para:     1故障设备，0正常 
***************************************************************************************************************************************/
uint8_t Vend_IsErrorState()
{ 
	uint8_t coinError = 0,billError = 0;
	//纸币器	
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		if((MdbBillErr.Communicate)||(MdbBillErr.moto)||(MdbBillErr.sensor)||(MdbBillErr.romchk)||(MdbBillErr.jam)||(MdbBillErr.removeCash)||(MdbBillErr.cashErr))
		{
			//billError = 1;			
		}	
	}
	#ifdef DEBUG_VENDSERVICE	
	Trace("\r\n%S,%d:BillErr=%d,%d,%d,%d,%d,%d,%d",__FILE__,__LINE__,MdbBillErr.Communicate,MdbBillErr.moto,MdbBillErr.sensor,MdbBillErr.romchk,MdbBillErr.jam,MdbBillErr.removeCash,MdbBillErr.cashErr);
	#endif
	//硬币器
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		if((MdbCoinErr.Communicate)||(MdbCoinErr.sensor)||(MdbCoinErr.tubejam)||(MdbCoinErr.romchk)||(MdbCoinErr.routing)||(MdbCoinErr.jam)||(MdbCoinErr.removeTube))
		{
			coinError = 1;	
		}		
	}
	#ifdef DEBUG_VENDSERVICE	
	Trace("\r\n%S,%d:CoinErr=%d,%d,%d,%d,%d,%d,%d",__FILE__,__LINE__,MdbCoinErr.Communicate,MdbCoinErr.sensor,MdbCoinErr.tubejam,MdbCoinErr.romchk,MdbCoinErr.routing,MdbCoinErr.jam,MdbCoinErr.removeTube);
	#endif
	if(coinError || billError)
	{
		vmcEorr=1;
		return 1;
	}	
	else
	{
		vmcEorr=0;
		return 0;
	}
}
/***************************************************************************************************************************************
** @APP Function name:   VendingService
** @APP Descriptions:	 用户交易流程
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
static void VendingService(void)
{
	uint8_t  billOptBack = 0;
	uint32_t InValue = 0;
	uint8_t Billtype = 0;

	uint8_t keyValue = 0;//按键值
	uint8_t moneyGet = 0;//是否已经有钱了
	uint32_t debtMoney;	
	uint8_t ChuhuoRst = 0;
	API_LCM_ClearScreen();
	API_LCM_Printf(0,0,0,0,"Init system,please wait...\r\n");
	//初始化硬币器
	if(VMCParam.MdbCoinDeviceEAB == 0x00)
	{
		API_LCM_Printf(0,2,0,0,"CoinDevice is disable...\r\n");
	}
	else
	{
		MdbCoinResetAndSetup();
		if(MDBCoinDevice.Oneline)
			API_LCM_Printf(0,2,0,0,"Init CoinDevice OK...\r\n");
		else
			API_LCM_Printf(0,2,0,0,"Init CoinDevice ERR...\r\n");
		MdbCoinTypeEanbleOrDisable(1,1);
		vTaskDelay(20);
		CoinDevProcess(&InValue,&Billtype,&billOptBack);
		vTaskDelay(20);		
	}
	//初始化纸币器
	if(VMCParam.MdbBillDeviceEAB == 0x00)
	{
		API_LCM_Printf(0,4,0,0,"BillDevice is disable...\r\n");
	}
	else
	{
		MdbBillResetAndSetup();
		if(MDBBillDevice.Oneline)
			API_LCM_Printf(0,4,0,0,"Init BillDevice OK...\r\n");
		else
			API_LCM_Printf(0,4,0,0,"Init BillDevice ERR...\r\n");
		BillDevProcess(&InValue,&Billtype,MBOX_BILLENABLEDEV,&billOptBack);
		vTaskDelay(20);
		BillDevProcess(&InValue,&Billtype,0,&billOptBack);
		vTaskDelay(20);
	}
	API_LCM_ClearScreen();
	API_SYSTEM_TimerChannelSet(4,5 * 100);		
	while(1)
	{
		switch(vmcStatus)
		{
			case VMC_FREE:
				//1.显示空闲页面
				if(API_SYSTEM_TimerReadChannelValue(1) == 0)
				{
					API_SYSTEM_TimerChannelSet(1,5 * 100);
					Vend_DispFreePage();
				}
				//2.检测设备故障状态
				if(API_SYSTEM_TimerReadChannelValue(4) == 0)
				{
					API_SYSTEM_TimerChannelSet(4,5 * 100);				
					if(Vend_IsErrorState())
					{
						API_SYSTEM_TimerChannelSet(1,0);
						Vend_BillCoinCtr(2,2,0);
						vmcStatus = VMC_ERROR;
					}
				}
				//4.轮询硬币器可找零硬币
				if(API_SYSTEM_TimerReadChannelValue(2) == 0)
				{
					API_SYSTEM_TimerChannelSet(2,5 * 100);
					Vend_UpdateTubeMoney();
				}
				//2.轮询按键
				keyValue = API_KEY_ReadKey();
				if(keyValue)
				{
					//确定
					if((keyValue >= 'A') && (keyValue <= 'F'))
					{
						#ifdef DEBUG_VENDSERVICE
						Trace("\r\n%S,%d:App User Press key = %d",__FILE__,__LINE__,keyValue);
						#endif
						API_SYSTEM_TimerChannelSet(3,5 * 100);
						vmcStatus = VMC_CHAXUN;
						API_LCM_DrawLine(0,12);
						break;
					}
					//5.判断是否进入维护页面
					else if(keyValue== 'M')
					{
						VMCParam.VMCMode = VMC_MODE_MAIN;
						break;
					}				
				}			
				break;
			case VMC_CHAXUN:
				//1.轮询按键
				if(keyValue)
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User press key",__FILE__,__LINE__);
					#endif
					API_SYSTEM_TimerChannelSet(3,5 * 100);
					Vend_DispChaxunPage(&keyValue);
				}
				keyValue = API_KEY_ReadKey();
				if(keyValue)
				{					
					if((keyValue >= '1') && (keyValue <= '8'))
						vTaskDelay(2);
					else if(keyValue == '>')
						vTaskDelay(2);
					else
						keyValue=0;
				}
				//3.超时退出查询页面
				if(API_SYSTEM_TimerReadChannelValue(3) == 0)
				{
					keyValue = '>';
					API_SYSTEM_TimerChannelSet(3,5 * 100);
				}
				break;	
			case VMC_SALE:
				//1.轮询投纸币和硬币金额
				moneyGet = Vend_GetMoney();
				if(moneyGet == 1)
				{
					Vend_DispSalePage();
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Pay in,amount = %ld",__FILE__,__LINE__,Vend_GetAmountMoney());
					#endif					
				}
				//.未投币时超时退出查询页面
				if(Vend_GetAmountMoney()==0)
				{					
					if(API_SYSTEM_TimerReadChannelValue(3) == 0)
					{
						Vend_ClearDealPar();
						API_LCM_ClearScreen();
						API_SYSTEM_TimerChannelSet(1,1 * 100);
						vmcStatus = VMC_FREE;
					}
				}
				//3.有按下退币按键
				if(Vend_IsTuibiAPI())
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Press Escrow button",__FILE__,__LINE__);
					#endif
					Vend_BillCoinCtr(2,2,0);
					vmcStatus = VMC_PAYOUT;
				}
				break;
			case VMC_CHUHUO:
				//出货前禁能按键，现金设备等
				Vend_BillCoinCtr(2,2,0);				
				Vend_DispChuhuoPage();
				API_KEY_KeyboardCtrl(0x00);			
				ChuhuoRst = API_VENDING_Vend(ChannelNum[0],ChannelNum[1]);
				API_KEY_KeyboardCtrl(0x01);				
				if(ChuhuoRst==1)
				{	
					//货道存货数量递减
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App Vending success and go to deduct money",__FILE__,__LINE__);
					#endif
					if(TradeParam.RemainGoods[vmcColumn] > 0)
					{
						TradeParam.RemainGoods[vmcColumn]--;
						LoadNewTradeParam();
					}
					Vend_DispQuhuoPage();	
					Vend_SaleCostMoney(vmcPrice);//扣款				
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:Deduct money,Remain = %d",__FILE__,__LINE__,Vend_GetAmountMoney());
					#endif							
					LogTransactionAPI(1);//记录日志
				}
				else
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App Vending Fail",__FILE__,__LINE__);
					#endif
					DispChhuoFailPage();			
					LogTransactionAPI(0);//记录日志
				}
				vmcPrice = 0;
				vmcColumn= 0;
				Vend_ClearDealPar();
				if(Vend_GetAmountMoney())
				{
					//用户有余额，开启现金设备并继续售卖
					Vend_BillCoinCtr(1,1,0);	
					Vend_DispSalePage();
					vmcStatus = VMC_SALE;
				}
				else
				{
					vmcStatus = VMC_END;
				}	
				break;	
			case VMC_PAYOUT:				
				Vend_DispPayoutPage();
				debtMoney = Vend_ChangerMoney();
				if(debtMoney)
				{
					Vend_DispIOUPage(debtMoney);
				}
				else
				{
					Vend_DispQuChangePage();
				}				
				vmcStatus = VMC_END;
				break;
			case VMC_END:				
				Vend_DispEndPage();
				LogEndAPI();
				Vend_ClearDealPar();
				vmcColumn = 0;	
				Vend_BillCoinCtr(1,1,0);				
				API_LCM_ClearScreen();
				vmcStatus = VMC_FREE;
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n\r\n%S,%d:App Vending Flow end...\r\n",__FILE__,__LINE__);
				#endif
				break;	
			case VMC_ERROR:					
				//1.显示欢迎或故障页面
				if(API_SYSTEM_TimerReadChannelValue(1) == 0)
				{
					API_SYSTEM_TimerChannelSet(1,5 * 100);
					Vend_DispFreePage();
				}	
				//2.检测设备是否恢复正常
				if(API_SYSTEM_TimerReadChannelValue(4) == 0)
				{
					API_SYSTEM_TimerChannelSet(4,5 * 100);	
					if(!Vend_IsErrorState())
					{	
						API_SYSTEM_TimerChannelSet(1,0);
						Vend_BillCoinCtr(1,1,1);
						vmcStatus = VMC_FREE;
					}
				}
				//3.只是轮询
				Vend_GetMoney();
				//4.判断是否进入维护页面
				if(API_KEY_ReadKey() == 'M')
					VMCParam.VMCMode = VMC_MODE_MAIN;
				break;
			default:
				break;
		}
		//判断是否进入维护页面
		if(VMCParam.VMCMode == VMC_MODE_MAIN)
		{
			vmcStatus = VMC_FREE;
			break;
		}
		vTaskDelay(2);
	}
}
/***************************************************************************************************************************************
** @APP Function name:   VendingIdle
** @APP Descriptions:	 回到空闲状态前，先清除交易的数据；此时系统进入维护模式
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
static void VendingIdle(void)
{
	LPC_GPIO2->FIODIR |= 1ul <<12;
	LPC_GPIO2->FIOSET |= 1ul <<12;
	vTaskDelay(50);
	LPC_GPIO2->FIODIR |= 1ul <<12;	
	LPC_GPIO2->FIOCLR |= 1ul <<12;
	vTaskDelay(50);
}
/**************************************End Of File*************************************************************************************/
