/****************************************Copyright (c)*****************************************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info----------------------------------------------------------------------------------------------------------
 ** @Filename:			ParamDefine.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *********************************************************************************************************************************************************/
#include "..\API\API.h"
#include "..\API\OS.H"
#include "ParamDefine.h"
volatile VMCPARAM VMCParam;
volatile TRADEPARAM TradeParam;
volatile SYSTEMERROR SysError;
volatile TOTALTRADELOG TotalTradeLog;
volatile SINGLETRADELOG SingleTradeLog;
volatile MDBCOINACCEPTORCHANGER MDBCoinDevice;
volatile MDBBILLVALIDATORRECYCLER MDBBillDevice;
volatile unsigned char Rtc[7];//年月日时分秒,其中年两字节
MDBBILLERROR MdbBillErr;
MDBCOINERROR MdbCoinErr;
/***********************************************************************************************************************************************************
** @API Function name:   InitVmcParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void InitSysErrCode(void)
{
	SysError.FramErr = 0x00;
	SysError.MdbErr = 0x00;
	SysError.MotorErr = 0x00;
	SysError.TradeParamErr = 0x00;
}
/***********************************************************************************************************************************************************
** @API Function name:   InitVmcParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void InitVmcParam(void)
{
	unsigned char err,Temp[19 + 2 * (MAXARRAY)],Flag = 0x00;
	unsigned short i,j,Crc;
	err = API_FRAM_ReadFram(VMC_FRAM_ADDR,VMC_PARA_PAGE_S,Temp,(19 + 2 * MAXARRAY));
	if(err)
	{
		Crc = CrcCheck(Temp,17 + 2 * MAXARRAY);
		if((Temp[18 + 2 * MAXARRAY] == (unsigned char)Crc) && (Temp[17 + 2 * MAXARRAY] == (unsigned char)(Crc >> 8)))
		{
			#ifdef APP_DBG_FRAM
			Trace("InitVmcParam():Read OK(%04X):\r\n",Crc);
			for(i=0;i<19 + 2 * MAXARRAY;i++)
			{
				Trace(" %02X",Temp[i]);
				if(i == 47)
					Trace(" --chn config\r\n");
				if(i == 95)
					Trace(" --Max capa\r\n");
				if(i == 102)
					Trace(" --ID\r\n");
				if(i == 108)
					Trace(" --PWD\r\n");
				if(i == 109)
					Trace(" --Lan\r\n");
				if(i == 110)
					Trace(" --Bill\r\n");
				if(i == 111)
					Trace(" --Coin\r\n");
				if(i == 112)
					Trace(" --Cashless\r\n");
			}
			Trace("\r\n");
			#endif
			
			for(i=0,j=0;i<MAXARRAY;i++,j++)
			{
				VMCParam.GoodsChannelArray[j] = Temp[i];
				//Trace(" i=%02X,j=%02X,GoodsChannelArray=%02X,Temp=%02X",i,j,VMCParam.GoodsChannelArray[j],Temp[i]);
			}
			for(i=MAXARRAY,j=0;i<2 * MAXARRAY;i++,j++)
			{
				VMCParam.GoodsMaxCapacity[j] = Temp[i];
				//Trace(" i=%02X,j=%02X,GoodsMaxCapacity=%02X,Temp=%02X",i,j,VMCParam.GoodsMaxCapacity[j],Temp[i]);
			}
			Flag = 0x00;
			for(i=2 * MAXARRAY,j=0;i<(2 * MAXARRAY + 7);i++,j++)
			{
				VMCParam.VMCID[j] = Temp[i];
				if(Temp[i] == 0x00)
					Flag = 0x01;
				else
					Flag = 0x00;
				//Trace(" i=%02X,j=%02X,VMCID=%02X,Temp=%02X",i,j,VMCParam.VMCID[j],Temp[i]);
			}
			for(i=(2 * MAXARRAY + 7),j=0;i<(2 * MAXARRAY + 13);i++,j++)
			{
				VMCParam.Password[j] = Temp[i];
			}
			VMCParam.Language = Temp[2 * MAXARRAY + 13];
			VMCParam.MdbBillDeviceEAB = Temp[2 * MAXARRAY + 14];
			VMCParam.MdbCoinDeviceEAB =Temp[2 * MAXARRAY + 15];
			VMCParam.MdbCashlessDeviceEAB =Temp[2 * MAXARRAY + 16];
			
			#ifdef APP_DBG_FRAM
			Trace("GoodsChannelArray:\r\n");
			for(i=0;i<MAXARRAY;i++)
			{
				Trace(" %02X",VMCParam.GoodsChannelArray[i]);
			}
			Trace("\r\n");

			Trace("GoodsMaxCapacity:\r\n");
			for(i=0;i<MAXARRAY;i++)
			{
				Trace(" %02X",VMCParam.GoodsMaxCapacity[i]);
			}
			Trace("\r\n");

			Trace("VMCID:\r\n");
			for(i=0;i<7;i++)
			{
				Trace(" %02X",VMCParam.VMCID[i]);
			}
			Trace("\r\n");
			Trace("Password:\r\n");
			for(i=0;i<6;i++)
			{
				Trace(" %02X",VMCParam.Password[i]);
			}
			Trace("\r\n");
			#endif
			if(Flag)
				LoadDefaultVmcParam();//VMCID全为零,加载默认参数
		}
		else
		{	
			#ifdef APP_DBG_FRAM
			Trace("InitVmcParam():Read ok but data is err\r\n");
			#endif
			LoadDefaultVmcParam();
		}
	}
	else
	{
		#ifdef APP_DBG_FRAM
		Trace("InitVmcParam():Read ERR\r\n");
		#endif
		LoadDefaultVmcParam();
	}
}
/***********************************************************************************************************************************************************
** @API Function name:   LoadDefaultVmcParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void LoadDefaultVmcParam(void)
{
	unsigned char err,Temp[2 * MAXARRAY + 19];
	unsigned short i,Crc;
	for(i=0;i<MAXARRAY;i++)
	{
		VMCParam.GoodsChannelArray[i] = 0x01;
		Temp[i] = 0x01;
	}
	for(i=0;i<MAXARRAY;i++)
	{
		VMCParam.GoodsMaxCapacity[i] = 0x08;
		Temp[i + MAXARRAY] = 0x08;
	}
	for(i=0;i<7;i++)
	{
		VMCParam.VMCID[i] = i;
		Temp[2 * MAXARRAY + i] = i;
	}
	for(i=0;i<6;i++)
	{
		VMCParam.Password[i] = 0x31 + i;
		Temp[2 * MAXARRAY + 7 + i] = 0x31 + i;
	}
	VMCParam.Language = 0x00;
	Temp[2 * MAXARRAY + 13] = 0x00;
	VMCParam.MdbBillDeviceEAB = 0x01;
	Temp[2 * MAXARRAY + 14] = 0x01;
	VMCParam.MdbCoinDeviceEAB = 0x01;
	Temp[2 * MAXARRAY + 15] = 0x01;
	VMCParam.MdbCashlessDeviceEAB = 0x00;
	Temp[2 * MAXARRAY + 16] = 0x00;
	Crc = CrcCheck(Temp,(2 * MAXARRAY + 17));
	Temp[2 * MAXARRAY + 17] = (unsigned char)(Crc >> 8);
	Temp[2 * MAXARRAY + 18] = (unsigned char)Crc;
	err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_PARA_PAGE_S,Temp,(2 * MAXARRAY + 19));
	if(err == 0x00)
	{
		vTaskDelay(20);
		err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_PARA_PAGE_S,Temp,(2 * MAXARRAY + 19));
	}
	#ifdef APP_DBG_FRAM
	Trace("LoadDefaultVmcParam():\r\n");
	for(i=0;i<19 + 2 * MAXARRAY;i++)
	{
		Trace(" %02X",Temp[i]);
		if(i == 47)
			Trace(" --chn config\r\n");
		if(i == 95)
			Trace(" --Max capa\r\n");
		if(i == 102)
			Trace(" --ID\r\n");
		if(i == 108)
			Trace(" --PWD\r\n");
		if(i == 109)
			Trace(" --Lan\r\n");
		if(i == 110)
			Trace(" --Bill\r\n");
		if(i == 111)
			Trace(" --Coin\r\n");
		if(i == 112)
			Trace(" --Cashless\r\n");
	}
	Trace("\r\n");
	#endif
}
/***********************************************************************************************************************************************************
** @API Function name:   LoadNewVmcParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void LoadNewVmcParam(void)
{
	unsigned char err,Temp[2 * MAXARRAY + 19];
	unsigned short i,Crc;
	for(i=0;i<MAXARRAY;i++)
	{
		Temp[i] = VMCParam.GoodsChannelArray[i];
	}
	for(i=0;i<MAXARRAY;i++)
	{
		Temp[i + MAXARRAY] = VMCParam.GoodsMaxCapacity[i];
	}
	for(i=0;i<7;i++)
	{
		Temp[2 * MAXARRAY + i] = VMCParam.VMCID[i];
	}
	for(i=0;i<6;i++)
	{
		Temp[2 * MAXARRAY + 7 + i] = VMCParam.Password[i];
	}
	Temp[2 * MAXARRAY + 13] = VMCParam.Language;
	Temp[2 * MAXARRAY + 14] = VMCParam.MdbBillDeviceEAB;
	Temp[2 * MAXARRAY + 15] = VMCParam.MdbCoinDeviceEAB;
	Temp[2 * MAXARRAY + 16] = VMCParam.MdbCashlessDeviceEAB;
	Crc = CrcCheck(Temp,(2 * MAXARRAY + 17));
	Temp[2 * MAXARRAY + 17] = (unsigned char)(Crc >> 8);
	Temp[2 * MAXARRAY + 18] = (unsigned char)Crc;
	err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_PARA_PAGE_S,Temp,(2 * MAXARRAY + 19));
	if(err == 0x00)
	{
		vTaskDelay(20);
		err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_PARA_PAGE_S,Temp,(2 * MAXARRAY + 19));
		#ifdef APP_DBG_FRAM
		Trace("LoadNewVmcParam():err\r\n");
		#endif
	}
	#ifdef APP_DBG_FRAM
	else
	{
		Trace("LoadNewVmcParam():\r\n");
		for(i=0;i<19 + 2 * MAXARRAY;i++)
		{
			Trace(" %02X",Temp[i]);
			if(i == 47)
				Trace(" --chn config\r\n");
			if(i == 95)
				Trace(" --Max capa\r\n");
			if(i == 102)
				Trace(" --ID\r\n");
			if(i == 108)
				Trace(" --PWD\r\n");
			if(i == 109)
				Trace(" --Lan\r\n");
			if(i == 110)
				Trace(" --Bill\r\n");
			if(i == 111)
				Trace(" --Coin\r\n");
			if(i == 112)
				Trace(" --Cashless\r\n");
		}
		Trace("\r\n");	
	}
	#endif
}
/***********************************************************************************************************************************************************
** @API Function name:   InitTradeParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void InitTradeParam(void)
{
	unsigned char err,Temp[3 * MAXARRAY + 10];
	unsigned short i,Crc;
	err = API_FRAM_ReadFram(VMC_FRAM_ADDR,VMC_TDPA_PAGE_S,Temp,(3 * MAXARRAY + 10));
	if(err)
	{
		Crc = CrcCheck(Temp,3 * MAXARRAY + 8);
		if(Crc == 0x00)
		{
			LoadDefaultTradeParam();
			#ifdef APP_DBG_FRAM
			Trace("InitTradeParam():Read ok but Crc == 0x00,LoadDefaultTradeParam()...\r\n");
			#endif
		}
		else
		{
			if((Temp[3 * MAXARRAY + 9] == (unsigned char)Crc) && (Temp[3 * MAXARRAY + 8] == (unsigned char)(Crc >> 8)))
			{
				for(i=0;i<MAXARRAY;i++)
				{
					TradeParam.GoodsPrice[i] = ((((unsigned short)Temp[i]) << 8) + Temp[i + MAXARRAY]);
					if(TradeParam.GoodsPrice[i] == 0x00)
						SysError.TradeParamErr = 0x01;// Price cannot be zero
				}
				for(i=0;i<MAXARRAY;i++)
				{
					TradeParam.RemainGoods[i] = Temp[i + 2*MAXARRAY];
				}
				TradeParam.TypeBillEscrow = (((unsigned short)Temp[3 * MAXARRAY]) << 8) + Temp[3 * MAXARRAY + 1];
				TradeParam.TypeBillEnable = (((unsigned short)Temp[3 * MAXARRAY + 2]) << 8) + Temp[3 * MAXARRAY + 3];
				TradeParam.TypeCoinEnable = (((unsigned short)Temp[3 * MAXARRAY + 4]) << 8) + Temp[3 * MAXARRAY + 5];
				TradeParam.ServiceWhenNoChanger = Temp[3 * MAXARRAY + 6];
				TradeParam.ServiceWhenBillStackerIsFull = Temp[3 * MAXARRAY + 7];
				#ifdef APP_DBG_FRAM
				Trace("InitTradeParam() Ok:\r\n");
				Trace("Price:");
				for(i=0;i<MAXARRAY;i++)
				{
					Trace("%d ",TradeParam.GoodsPrice[i]);
				}
				Trace("\r\n");
				Trace("RemainGoods:");
				for(i=0;i<MAXARRAY;i++)
				{
					Trace("%d ",TradeParam.RemainGoods[i]);
				}
				Trace("\r\n");
				Trace("TradeParam.TypeBillEscrow = %04X\r\n",TradeParam.TypeBillEscrow);
				Trace("TradeParam.TypeBillEnable = %04X\r\n",TradeParam.TypeBillEnable);
				Trace("TradeParam.TypeCoinEnable = %04X\r\n",TradeParam.TypeCoinEnable);
				Trace("TradeParam.ServiceWhenNoChanger = %d\r\n",TradeParam.ServiceWhenNoChanger);
				Trace("TradeParam.ServiceWhenBillStackerIsFull = %d\r\n",TradeParam.ServiceWhenBillStackerIsFull);
				#endif
			}
			#ifdef APP_DBG_FRAM
			else
				Trace("InitTradeParam():Read ok but data is err\r\n");
			#endif
		}		
	}
	else
	{
		LoadDefaultTradeParam();
		#ifdef APP_DBG_FRAM
		Trace("InitTradeParam():Read ok but data is err,LoadDefaultTradeParam()...\r\n");
		#endif
	}
	
}
/***********************************************************************************************************************************************************
** @API Function name:   LoadDefaultTradeParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void LoadDefaultTradeParam(void)
{
	unsigned char i;
	for(i=0;i<MAXARRAY;i++)
	{
		TradeParam.GoodsPrice[i] = 100;
	}
	for(i=0;i<MAXARRAY;i++)
	{
		TradeParam.RemainGoods[i] = 0x08;
	}
	TradeParam.TypeBillEscrow = 0x00;
	TradeParam.TypeBillEnable = 0xFFFF;
	TradeParam.TypeCoinEnable = 0xFFFF;
	TradeParam.ServiceWhenNoChanger = 0x01;
	TradeParam.ServiceWhenBillStackerIsFull = 0x01;
}
/***********************************************************************************************************************************************************
** @API Function name:   InitTotalLogParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void LoadNewTradeParam(void)
{
	unsigned char err,Temp[3 * MAXARRAY + 10];
	unsigned short i,Crc;
	#ifdef APP_DBG_FRAM
	Trace("LoadNewTradeParam():\r\n");
	Trace("Price:");
	for(i=0;i<MAXARRAY;i++)
	{
		Trace("%d ",TradeParam.GoodsPrice[i]);
	}
	Trace("\r\n");
	Trace("RemainGoods:");
	for(i=0;i<MAXARRAY;i++)
	{
		Trace("%d ",TradeParam.RemainGoods[i]);
	}
	Trace("\r\n");
	#endif
	for(i=0;i<MAXARRAY;i++)
	{
		Temp[i] = TradeParam.GoodsPrice[i] >> 8;
		Temp[i + MAXARRAY] = TradeParam.GoodsPrice[i];
		if(TradeParam.GoodsPrice[i] == 0x00)
			SysError.TradeParamErr = 0x01;// Price cannot be zero
	}
	for(i=0;i<MAXARRAY;i++)
	{
		Temp[i + 2*MAXARRAY] = TradeParam.RemainGoods[i];
	}
	Temp[3 * MAXARRAY]     = (TradeParam.TypeBillEscrow >> 8);
	Temp[3 * MAXARRAY + 1] = TradeParam.TypeBillEscrow;
	Trace("TradeParam.TypeBillEscrow = %04X\r\n",TradeParam.TypeBillEscrow);
	Temp[3 * MAXARRAY + 2] = (TradeParam.TypeBillEnable >> 8);
	Temp[3 * MAXARRAY + 3] = TradeParam.TypeBillEnable;
	Trace("TradeParam.TypeBillEnable = %04X\r\n",TradeParam.TypeBillEnable);
	Temp[3 * MAXARRAY + 4] = (TradeParam.TypeCoinEnable >> 8);
	Temp[3 * MAXARRAY + 5] = TradeParam.TypeCoinEnable;
	Trace("TradeParam.TypeCoinEnable = %04X\r\n",TradeParam.TypeCoinEnable);
	Temp[3 * MAXARRAY + 6] = TradeParam.ServiceWhenNoChanger;
	Trace("TradeParam.ServiceWhenNoChanger = %02X\r\n",TradeParam.ServiceWhenNoChanger);
	Temp[3 * MAXARRAY + 7] = TradeParam.ServiceWhenBillStackerIsFull;
	Trace("TradeParam.ServiceWhenBillStackerIsFull = %02X\r\n",TradeParam.ServiceWhenBillStackerIsFull);
	Crc = CrcCheck(Temp,3 * MAXARRAY + 8);
	Temp[3 * MAXARRAY + 8] = (unsigned char)(Crc >> 8);
	Temp[3 * MAXARRAY + 9] = Crc;
	err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_TDPA_PAGE_S,Temp,(3 * MAXARRAY + 10));
	if(err == 0x00)
	{
		vTaskDelay(20);
		err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_TDPA_PAGE_S,Temp,(3 * MAXARRAY + 10));
		#ifdef APP_DBG_FRAM
		Trace("LoadNewTradeParam():err\r\n");
		#endif
	}
}
/***********************************************************************************************************************************************************
** @API Function name:   InitTotalLog
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void InitTotalLog(void)
{
	unsigned char err,Temp[30];
	unsigned short Crc;
	err = API_FRAM_ReadFram(VMC_FRAM_ADDR,VMC_TLOG_PAGE_S,Temp,30);
	if(err)
	{
		Crc = CrcCheck(Temp,208);
		TotalTradeLog.TotalCoinPayIn    = (((unsigned int)Temp[0]  << 24) + ((unsigned int)Temp[1]  << 16) + ((unsigned int)Temp[2]  << 8) + (unsigned int)Temp[3]);
		TotalTradeLog.TotalCoinPayout   = (((unsigned int)Temp[4]  << 24) + ((unsigned int)Temp[5]  << 16) + ((unsigned int)Temp[6]  << 8) + (unsigned int)Temp[7]);
		TotalTradeLog.TotalBillPayIn    = (((unsigned int)Temp[8]  << 24) + ((unsigned int)Temp[9]  << 16) + ((unsigned int)Temp[10] << 8) + (unsigned int)Temp[11]);
		TotalTradeLog.TotalVendNumber   = (((unsigned int)Temp[12] << 24) + ((unsigned int)Temp[13] << 16) + ((unsigned int)Temp[14] << 8) + (unsigned int)Temp[15]);
		TotalTradeLog.TotalSuccesNumber = (((unsigned int)Temp[16] << 24) + ((unsigned int)Temp[17] << 16) + ((unsigned int)Temp[18] << 8) + (unsigned int)Temp[19]);
		TotalTradeLog.TotalErrorNumber  = (((unsigned int)Temp[20] << 24) + ((unsigned int)Temp[21] << 16) + ((unsigned int)Temp[22] << 8) + (unsigned int)Temp[23]);
		TotalTradeLog.SingleTradeLogPointer = (((unsigned int)Temp[24] << 24) + ((unsigned int)Temp[25] << 16) + ((unsigned int)Temp[26] << 8) + (unsigned int)Temp[27]);
		if(!(Temp[28] == (unsigned char)Crc) && (Temp[29] == (unsigned char)(Crc >> 8)))
		{
			LoadNewTotalLog();
			#ifdef APP_DBG_FRAM
			Trace("InitTotalLog():Read ok but data is err,Reload data\r\n");
			#endif
		}
		#ifdef APP_DBG_FRAM
		Trace("InitTotalLog():ok\r\n");
		Trace("\r\nTotalCoinPayIn=%d.%d,TotalBillPayIn=%d.%d,TotalCoinPayout=%d.%d,TotalSuccesNumber=%d",TotalTradeLog.TotalCoinPayIn/10,TotalTradeLog.TotalCoinPayIn%10,
		TotalTradeLog.TotalBillPayIn/10,TotalTradeLog.TotalBillPayIn%10,TotalTradeLog.TotalCoinPayout/10,TotalTradeLog.TotalCoinPayout%10,
		TotalTradeLog.TotalSuccesNumber);
		#endif
	}
	else
	{
		#ifdef APP_DBG_FRAM
		Trace("InitTotalLog():Caution ,Total Read false! \r\n");
		#endif
		TotalTradeLog.TotalCoinPayIn = 0x00;
		TotalTradeLog.TotalCoinPayout = 0x00;
		TotalTradeLog.TotalBillPayIn = 0x00;
		TotalTradeLog.TotalVendNumber = 0x00;
		TotalTradeLog.TotalSuccesNumber = 0x00;
		TotalTradeLog.TotalErrorNumber = 0x00;
		TotalTradeLog.SingleTradeLogPointer = 0x00;
		SysError.TradeParamErr = 0x01;
	}
}
/***********************************************************************************************************************************************************
** @API Function name:   InitTotalLog
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void LoadNewTotalLog(void)
{
	unsigned char err,Temp[30];
	unsigned short i,Crc;
	for(i=0;i<4;i++)
	{
		Temp[i] = (unsigned char)(TotalTradeLog.TotalCoinPayIn >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 4] = (unsigned char)(TotalTradeLog.TotalCoinPayout >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 8] = (unsigned char)(TotalTradeLog.TotalBillPayIn >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 12] = (unsigned char)(TotalTradeLog.TotalVendNumber >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 16] = (unsigned char)(TotalTradeLog.TotalSuccesNumber >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 20] = (unsigned char)(TotalTradeLog.TotalErrorNumber >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 24] = (unsigned char)(TotalTradeLog.SingleTradeLogPointer >> (24 - i * 8));
	}
	Crc = CrcCheck(Temp,28);
	Temp[28] = (unsigned char)(Crc >> 8);
	Temp[29] = (unsigned char)Crc;
	#ifdef APP_DBG_FRAM
	Trace("LoadTotalLog():ok\r\n");
	Trace("\r\n TotalCoinPayIn=%d.%d,TotalBillPayIn=%d.%d,TotalCoinPayout=%d.%d,TotalSuccesNumber=%d",TotalTradeLog.TotalCoinPayIn/10,TotalTradeLog.TotalCoinPayIn%10,
	TotalTradeLog.TotalBillPayIn/10,TotalTradeLog.TotalBillPayIn%10,TotalTradeLog.TotalCoinPayout/10,TotalTradeLog.TotalCoinPayout%10,
	TotalTradeLog.TotalSuccesNumber);
	#endif
	err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_TLOG_PAGE_S,Temp,30);
	if(err == 0x00)
	{
		vTaskDelay(20);
		API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_TLOG_PAGE_S,Temp,30);
	}
}
/***********************************************************************************************************************************************************
** @API Function name:   InitSingleLogParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void InitSingleLog(void)
{
	memset((void *)SingleTradeLog.Time,0x00,7);
	SingleTradeLog.Err = 0x00;
	SingleTradeLog.CoinPayIn = 0x00;
	SingleTradeLog.BillPayIn = 0x00;
	SingleTradeLog.CoinPayout= 0x00;
	SingleTradeLog.Channel = 0x00;
}
/***********************************************************************************************************************************************************
** @API Function name:   InitSingleLogParam
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern void LoadNewSingleLog(void)
{
	unsigned char err,Temp[23];
	unsigned short i,Crc;
	for(i=0;i<7;i++)
	{
		Temp[i] = SingleTradeLog.Time[i];
	}
	Temp[7] = SingleTradeLog.Err;
	for(i=0;i<4;i++)
	{
		Temp[i + 8] = (unsigned char)(SingleTradeLog.CoinPayIn >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 12] = (unsigned char)(SingleTradeLog.BillPayIn >> (24 - i * 8));
	}
	for(i=0;i<4;i++)
	{
		Temp[i + 16] = (unsigned char)(SingleTradeLog.CoinPayout >> (24 - i * 8));
	}
	Temp[20] = SingleTradeLog.Channel;
	Crc = CrcCheck(Temp,21);
	Temp[21] = (unsigned char)(Crc >> 8);
	Temp[22] = (unsigned char)Crc;
	err = API_FRAM_WriteFram(VMC_FRAM_ADDR,TotalTradeLog.SingleTradeLogPointer,Temp,23);
	if(err == 0x00)
	{
		vTaskDelay(20);
		err = API_FRAM_WriteFram(VMC_FRAM_ADDR,VMC_TLOG_PAGE_S,Temp,30);
		if(err)
		{
			TotalTradeLog.SingleTradeLogPointer += 23;
			if(TotalTradeLog.SingleTradeLogPointer >= VMC_SLOG_PAGE_E)
				TotalTradeLog.SingleTradeLogPointer = VMC_SLOG_PAGE_S;
			LoadNewTotalLog();
		}
	}
	else
	{
		TotalTradeLog.SingleTradeLogPointer += 23;
		if(TotalTradeLog.SingleTradeLogPointer >= VMC_SLOG_PAGE_E)
			TotalTradeLog.SingleTradeLogPointer = VMC_SLOG_PAGE_S;
		LoadNewTotalLog();
	}
}
/***********************************************************************************************************************************************************
** @API Function name:   CrcCheck
** @API Input para:      None
** @API retrun para:     None
***********************************************************************************************************************************************************/
extern unsigned short CrcCheck(unsigned char *data,unsigned int len)
{
    unsigned short i,j,crc = 0,current = 0;
    for(i=0;i<len;i++)
    {
        current = data[i] << 8;
        for(j=0;j<8;j++)
        {
            if((short)(crc^current) < 0)
                crc = (crc<<1) ^ 0x2102;
            else
                crc <<= 1;
            current <<= 1;
        }
    }
    return crc;
}
/********************************************************************************************************
** @API Function name:     	PrintfMoney
** @API Descriptions:	    依据小数位数，显示金额的函数
** @API input parameters:    dispnum:投币金额，以分为单位 
** @API output parameters:   无
** @API Returned value:      显示字符串
*********************************************************************************************************/
char *PrintfMoney(uint32_t dispnum)
{  
	static char strnum[10];
	uint8_t type = 1;
	switch(type) 
	{
		case 2://以分为单位
			sprintf(strnum,"%d.%02d",dispnum/100,dispnum%100);	
			break;
		case 1://以角为单位
			dispnum /= 10;
			sprintf(strnum,"%d.%d",dispnum/10,dispnum%10);
			break;
		case 0://以元为单位
			sprintf(strnum,"%d",dispnum/100);
			break;
		default:break;	  
	}
	return &strnum[0];
}
/****************************************************************End Of File*******************************************************************************/
