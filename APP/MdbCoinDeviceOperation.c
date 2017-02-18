/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			MdbCoinDeviceOperation.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\API\API.h"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "MdbCoinDeviceOperation.h"
uint32_t  CoinScale;		 //比例因子
/*********************************************************************************************************
** @APP Function name:   MdbCoinResetAndSetup
** @APP Input para:      None
** @APP retrun para:     None
*********************************************************************************************************/
void MdbCoinResetAndSetup(void)
{
	unsigned char err,i,MDBAck[36],MDBAckLen;	
	uint16_t CoinDecimal;		 //10^小数位数
	memset((void *)MDBCoinDevice.ManufacturerCode,0x00,3);
	memset((void *)MDBCoinDevice.CurrencyCode,0x00,2);
	memset((void *)MDBCoinDevice.CoinTypeRouting,0x00,2);
	memset((void *)MDBCoinDevice.CoinTypeCredit,0x00,16);
	memset((void *)MDBCoinDevice.CoinTypeTubeStatus,0x00,16);
	memset((void *)MDBCoinDevice.CoinTypePresentInTube,0x00,16);
	//Poll
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,1000);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_Poll_CoinDevice(MDBAck,&MDBAckLen))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err == 0x00)
	{
		//Poll err turn to Reset flow
		API_SYSTEM_TimerChannelSet(0,400);
		while(API_SYSTEM_TimerReadChannelValue(0))
		{
			if(API_MDB_Reset_CoinDevice())
			{
				err = 0x01;
				break;
			}
			vTaskDelay(10);
		}
		if(err == 0x00)
		{
			MDBCoinDevice.Oneline = 0x00;
			return;
		}
		else
		{
			vTaskDelay(50);
			err = API_MDB_Poll_CoinDevice(MDBAck,&MDBAckLen);
			if(err)
			{
				if((MDBAckLen >= 0x01) && (MDBAck[0] == 0x06))
				{
					#ifdef APP_DBG_MDB_COIN
					Trace("MdbCoinResetAndSetup():POLL to obtain JUST RESET OK\r\n");
					#endif
					MDBCoinDevice.Oneline = 0x01;
				}
				else
				{
					#ifdef APP_DBG_MDB_COIN
					Trace("MdbCoinResetAndSetup():POLL Ok but ack ERR!(len = %d)\r\n",MDBAckLen);
					#endif
					MDBCoinDevice.Oneline = 0x00;
					return;			
				}			
			}
			else
			{
				#ifdef APP_DBG_MDB_COIN
				Trace("MdbCoinResetAndSetup():POLL to obtain JUST RESET ERR!\r\n");
				#endif
				MDBCoinDevice.Oneline = 0x00;
				return;
			}
		}
	}
	else
		MDBCoinDevice.Oneline = 0x01;
	//SETUP
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_Setup_CoinDevice(MDBAck,&MDBAckLen))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err)
	{
		MDBCoinDevice.Level = MDBAck[0];
		MDBCoinDevice.CurrencyCode[0] = MDBAck[1];
		MDBCoinDevice.CurrencyCode[1] = MDBAck[2];
		MDBCoinDevice.ScalingFactor = MDBAck[3];
		MDBCoinDevice.Decimal = MDBAck[4];
		CoinDecimal = 100;
	      for(i = 0; i < MDBAck[4]; i++) 
	      {
		   CoinDecimal /= 10;
	      }
		CoinScale = MDBAck[3] * CoinDecimal;
		MDBCoinDevice.CoinTypeRouting[0] = MDBAck[5];
		MDBCoinDevice.CoinTypeRouting[1] = MDBAck[6];
		for(i=0;i<(MDBAckLen - 7);i++)
		{
			if(MDBAck[7+i] == 0xFF)
			{
			     break;
			} 
			MDBCoinDevice.CoinTypeCredit[i] = MDBAck[7+i];
		}
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup()--Coin Device Info:\r\n");
		Trace("    Level = %02d\r\n",MDBCoinDevice.Level);
		Trace("    CurCode = %02X%02X\r\n",MDBCoinDevice.CurrencyCode[0],MDBCoinDevice.CurrencyCode[1]);
		Trace("    ScalingFactor = %02d\r\n",MDBCoinDevice.ScalingFactor);
		Trace("    Decimal = %02d\r\n",MDBCoinDevice.Decimal);
		Trace("    CoinTypeRouting = %02X%02X\r\n",MDBCoinDevice.CoinTypeRouting[0],MDBCoinDevice.CoinTypeRouting[1]);
		for(i=0;i<16;i++)
		{
			Trace("    CoinType%dCredit = %02d\r\n",i,MDBCoinDevice.CoinTypeCredit[i]);
		}
		Trace("\r\nDrvCOINDec=%d,%d",MDBAck[3],MDBAck[4]);
		Trace("\r\nDrvCOINDec2=%ld,%ld,%ld", MDBAck[3],CoinDecimal,CoinScale);		
		#endif
	}
	else
	{
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup():Setup err\r\n");
		#endif
		MDBCoinDevice.Oneline = 0x00;
	}
	if(MDBCoinDevice.Level <= 0x02)
	{
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup():software not support level 2 or lower level\r\n");
		#endif
		return;
	}
	//Expansion identification
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_ExpanIdentification_CoinDevice(MDBAck,&MDBAckLen))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err)
	{
		MDBCoinDevice.ManufacturerCode[0] = MDBAck[0];
		MDBCoinDevice.ManufacturerCode[1] = MDBAck[1];
		MDBCoinDevice.ManufacturerCode[2] = MDBAck[2];
		MDBCoinDevice.Feature[0] = MDBAck[0];
		MDBCoinDevice.Feature[1] = MDBAck[1];
		MDBCoinDevice.Feature[2] = MDBAck[2];
		MDBCoinDevice.Feature[3] = MDBAck[3];
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup():identification Ok(ManuCode:%02X%02X%02X)\r\n",MDBAck[0],MDBAck[1],MDBAck[2]);
		#endif
	}
	else
	{
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup():identification err\r\n");
		#endif
		return;
	}
	//Expansion Feature enable
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_ExpanFeatureEnable_CoinDevice((void *)MDBCoinDevice.Feature))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err)
	{
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup():ExpanFeatureEnable Ok\r\n");
		#endif
	}
	#ifdef APP_DBG_MDB_COIN
	else
	{
		Trace("MdbCoinResetAndSetup():ExpanFeatureEnable err\r\n");
	}
	#endif
	//Expansion Send Diag status
	err = 0x00;
	err = API_MDB_ExpanDiagStatus_CoinDevice(&MDBAck[0],&MDBAck[1]);
	if(err)
	{
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinResetAndSetup():ExpanDiagStatus Ok\r\n");
		#endif
	}
	#ifdef APP_DBG_MDB_COIN
	else
	{
		Trace("MdbCoinResetAndSetup():ExpanDiagStatus err\r\n");
	}
	#endif
	return;
}

/*********************************************************************************************************
** Function name:       CoinDevProcess
** Descriptions:        硬币器收币操作
** input parameters:    无
** output parameters:   RecvMoney——收入的硬币金额以分为单位
						CoinType——收入的硬币通道
** Returned value:      有硬币收入返回1，无返回0
*********************************************************************************************************/
uint8_t CoinDevProcess(uint32_t *RecvMoney,unsigned char *CoinType,unsigned char *coinOptBack)
{
	unsigned char CoinRdBuff[36],CoinRdLen,ComStatus;
	uint8_t type=0;
	uint8_t  i;
	static uint8_t payoutFlag = 0;
	//轮询硬币器是否有收到硬币，有返回1
	ComStatus = API_MDB_Poll_CoinDevice(&CoinRdBuff[0],&CoinRdLen);
	if(ComStatus == 1)
	{
		MdbCoinErr.Communicate = 0;
		if(CoinRdLen==0)
		{
			memset(&MdbCoinErr,0,sizeof(MdbCoinErr));	
		}
		for(i = 0; i < CoinRdLen; i++) 
		{    
			if(CoinRdBuff[i] & 0x80) 
			{        	      
				i++; //忽略第2字节
			} 
			else if((CoinRdBuff[i] & 0xC0) == 0x40) 
			{	 
				//coins deposited有硬币投入
				if(((CoinRdBuff[i] & 0x30) == 0x10) || ((CoinRdBuff[i] & 0x30) == 0)) //in tubes
				{	 
					//in cashbox
				    //CoinAcceptor_UpDateDeposit(COINACCEPTOR_STACKED,CoinRdBuff[i] & 0x0F);
					type = CoinRdBuff[i] & 0x0F;
					*RecvMoney = (uint32_t)MDBCoinDevice.CoinTypeCredit[type] * CoinScale;
					*CoinType = type;
					#ifdef APP_DBG_MDB_COIN
					Trace("Drvcoin=%d,%ld,%d\r\n",MDBCoinDevice.CoinTypeCredit[type],*RecvMoney,*CoinType);
					#endif
					return 1;
				}
				i++; //忽略第2字节
			} 
			else if((CoinRdBuff[i] & 0xE0) == 0x20) 
			{	 //slug
			}
			else 
			{	
				//status
				switch(CoinRdBuff[i]) 
				{						
				   case 0x01:			                 //escrow request	
				   	   payoutFlag ++;
					   if(payoutFlag >= 1)
					   {
					   		*coinOptBack = 1;
							payoutFlag = 0;
					   }
					   break;
				   case 0x02:			                 //changer pay out busy
					   break;
				   case 0x03:			                 //no credit
					   //不能进入对应通道
					   break;
				   case 0x04:			                 //defective tube sensor				   	   	
					   //传感器故障
					   MdbCoinErr.sensor = 1;
					   break;
				   case 0x05:			                 //double arriver
				   	   break;
				   case 0x06:			                 //acceptor unplugged						   
					   break;
				   case 0x07:			                 //tube jam
					   //出币卡币
					   MdbCoinErr.tubejam = 1;
					   break;
				   case 0x08:			                 //rom chksum err
					   //ROM出错
					   MdbCoinErr.romchk = 1;
					   break;
				   case 0x09:			                 //coin routing err
					   //进币通道出错
					   MdbCoinErr.routing = 1;
					   break;
				   case 0x0A:			                 //changer busy
				   	   break;
				   case 0x0B:			                 //changer was reset
					   //刚复位
					   break;
				   case 0x0C:			                 //coin jam
					   //投币卡币
					    MdbCoinErr.jam = 1;
					   break;
				   case 0x0D:		              	     //possible credited coin removal
					   //试图移除硬币
					   MdbCoinErr.removeTube = 1;
					   break;
				   default:	
				   	   memset(&MdbCoinErr,0,sizeof(MdbCoinErr));	
					   break;
				}
		    }
	    }
		
	}
	else
	{
		#ifdef APP_DBG_MDB_COIN
		Trace("\r\nCoinDevProcess():communication Fail");
		#endif
		MdbCoinErr.Communicate = 1;
	}		
	return 0;
}


/*********************************************************************************************************
** @APP Function name:   MdbCoinGetTubeStatus
** @APP Input para:      None
** @APP retrun para:     None
*********************************************************************************************************/
void MdbCoinGetTubeStatus(void)
{
	unsigned char err,i,Ack[36],len;
	unsigned int status;
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_TubeStatus_CoinDevice(Ack,&len))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err)
	{
		status = (((unsigned int)Ack[0] << 8) + Ack[1]);
		for(i=0x00;i<(len - 2);i++)
		{
			MDBCoinDevice.CoinTypeTubeStatus[i] = (((status & (0x01 << i)) >> i) & 0x01);
			MDBCoinDevice.CoinTypePresentInTube[i] = Ack[2 + i];//每个面值的数量
		}
		#ifdef APP_DBG_MDB_COIN
		Trace("MdbCoinGetTubeStatus() Ok(%d):\r\n",len);
		Trace("%dDrvChangebufStatus=");
		for(err=0;err<16;err++)
		{
			Trace(" %d",MDBCoinDevice.CoinTypeTubeStatus[err]);
		}
		Trace("\r\n");
		Trace("%dDrvChangebufNum=");
		for(err=0;err<16;err++)
		{
			Trace(" %d",MDBCoinDevice.CoinTypePresentInTube[err]);
		}
		Trace("\r\n");
		#endif
	}
	#ifdef APP_DBG_MDB_COIN
	else
	{
		Trace("MdbCoinGetTubeStatus():err\r\n");
	}
	#endif
}
/*********************************************************************************************************
** @APP Function name:   MdbCoinEanbleOrDisable
** @APP Input para:      None
** @APP retrun para:     None
*********************************************************************************************************/
void MdbCoinTypeEanbleOrDisable(unsigned char AcceptCtrl,unsigned char DispenCtrl)
{
	unsigned char Acmd,Dcmd;
	if(AcceptCtrl)
	{
		if(((TradeParam.TypeCoinEnable >> 15) & 0x0001)!=0)
			Acmd |= 0x01;
		if(((TradeParam.TypeCoinEnable >> 14) & 0x0001)!=0)
			Acmd |= 0x02;
		if(((TradeParam.TypeCoinEnable >> 14) & 0x0001)!=0)
			Acmd |= 0x04;
	}
	else
		Acmd = 0x00;
	if(DispenCtrl)
		Dcmd = 0xff;
	else
		Dcmd = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_CoinType_CoinDevice(Acmd,Dcmd))
			break;
		vTaskDelay(10);
	}
}
/*********************************************************************************************************
** @APP Function name:   MdbGetCoinValue
** @APP Input para:      CoinTypeCredit通道值
** @APP retrun para:     以分为单位
*********************************************************************************************************/
uint32_t MdbGetCoinValue(unsigned char CoinTypeCredit)
{
	return (uint32_t)CoinTypeCredit*CoinScale;
}	
/*********************************************************************************************************
** @APP Function name:   MdbCoinPayout
** @APP Input para:      PayoutValue以分为单位
** @APP retrun para:     AcuPayout以分为单位
*********************************************************************************************************/
unsigned char MdbCoinPayout(uint32_t PayoutValue,uint32_t *AcuPayout)
{
	unsigned char err,i,status,TypePayout[16],len,errNumber;
	unsigned int PayNumber,Paying,Payed;
	memset((void *)TypePayout,0x00,16);
	errNumber = 0x00;
	Paying = 0x00;
	Payed = 0x00;
	MdbCoinGetTubeStatus();
	vTaskDelay(10);
	
	PayNumber = PayoutValue /CoinScale;//发送找零基准数量	
	#ifdef APP_DBG_MDB_COIN
	Trace("\r\nDrvChangescale = %d,%d", CoinScale,PayNumber);
	#endif
	while(PayNumber > 0)
	{
		Paying = (PayNumber > 200)?200:PayNumber;
		//1.发送找币指令
		err = API_MDB_ExpanPayout_CoinDevice(Paying);
		if(err)
		{
			errNumber = 0x00;
			PayNumber -= Paying;
			#ifdef APP_DBG_MDB_COIN
			Trace("\r\nDrvChangedispense = %d,%d", PayNumber,Paying);
			#endif
			API_SYSTEM_TimerChannelSet(0,Paying * 20);
			while(API_SYSTEM_TimerReadChannelValue(0))
			{
				//2发送扩展poll指令，检测找币是否完成
				err = API_MDB_ExpanPayoutValuePoll_CoinDevice(&status);
				if(err == 0)
					break;
				vTaskDelay(30);
			}
			if(err == 0)
			{
			      //3发送扩展指令，检测本次找币各通道找多少枚
				err = API_MDB_ExpanPayoutStatus_CoinDevice(TypePayout,&len);
				if(err)
				{
					#ifdef APP_DBG_MDB_COIN
					Trace("\r\nDrvChangeOut=%d,%d,%d,%d",TypePayout[0],TypePayout[1],TypePayout[2],TypePayout[3]);
					#endif
					for(i=0x00;i<len;i++)
					{
						Payed += ((uint32_t)TypePayout[i] * MDBCoinDevice.CoinTypeCredit[i] * CoinScale);
					}
				}
			}	
		}
		else
			errNumber++;
		if(errNumber >= 100)
		{
			err = 0x00;
			break;
		}
		vTaskDelay(20);
	}
	*AcuPayout = Payed;
	return err;
}
/*********************************************************************************************************
** Function name:       ChangePayoutProcessLevel3
** Descriptions:        level3找零操作
** input parameters:    PayMoney——需要找出的硬币金额
** output parameters:   
                        PayoutNum--各通道实际出币数量						
** Returned value:      有硬币找出返回1，无返回0
*********************************************************************************************************/
unsigned char ChangePayoutProcessLevel3(uint32_t PayMoney,uint32_t *PayoutMoney)
{
	unsigned char CoinRdBuff[36],CoinRdLen,ComStatus;
	uint32_t dispenseValue,Payed=0;
	uint8_t  i;
	uint8_t tempdispenseValue;
	
	memset(CoinRdBuff,0,sizeof(CoinRdBuff));
	//NowChangerDev = SystemPara.CoinChangerType;
	//Trace("6\r\n");	
	
	MdbCoinGetTubeStatus();
	vTaskDelay(10);
	*PayoutMoney=0;
	//stDevValue.CoinLevel = 2;
	/***************************************
	** level3级别，直接发送找零金额即可
	***************************************/
	//if(stDevValue.CoinLevel >= 3)
	{
		dispenseValue = PayMoney / CoinScale;//发送找零基准数量	
		#ifdef APP_DBG_MDB_COIN		
		Trace("\r\nDrvChangescale = %d,%d", CoinScale,dispenseValue);
		#endif
		while(dispenseValue>0)
		{
			tempdispenseValue=(dispenseValue>200)?200:dispenseValue;
			dispenseValue-=tempdispenseValue;
			#ifdef APP_DBG_MDB_COIN
			Trace("\r\nDrvChangedispense = %d,%d", dispenseValue,tempdispenseValue);
			#endif
			//1发送找币指令
			ComStatus = API_MDB_ExpanPayout_CoinDevice(tempdispenseValue);
			if(ComStatus == 1)
			{
				API_SYSTEM_TimerChannelSet(0,(tempdispenseValue/2 + 20)*100);
				while(API_SYSTEM_TimerReadChannelValue(0))
				{					
					//2发送扩展poll指令，检测找币是否完成
					ComStatus = API_MDB_ExpanPayoutValuePoll_CoinDevice(&CoinRdBuff[0]);
					//找零进行时，CoinRdLen=1 找零完成后，CoinRdLen = 0
					if( ComStatus == 0 )
					{
						memset(CoinRdBuff,0,sizeof(CoinRdBuff));
						//CoinRdLen = 0;
						//3发送扩展指令，检测本次找币各通道找多少枚
						ComStatus =API_MDB_ExpanPayoutStatus_CoinDevice(&CoinRdBuff[0],&CoinRdLen);
						if( ComStatus > 0 )
						{
							#ifdef APP_DBG_MDB_COIN
							Trace("\r\n%dDrvChangeOut=%d,%d,%d,%d",CoinRdLen,CoinRdBuff[0],CoinRdBuff[1],CoinRdBuff[2],CoinRdBuff[3]);
							#endif
							for(i = 0;i < CoinRdLen;i++)
							{
								Payed += ((uint32_t)CoinRdBuff[i] * MDBCoinDevice.CoinTypeCredit[i] * CoinScale);
							}
							break;
						}
					}
					vTaskDelay(10);
				}
			}
			#ifdef APP_DBG_MDB_COIN		
			else
			{
				Trace("\r\nDrvChangesendFail");
			}
			#endif
		}
	}		
	*PayoutMoney=Payed;
	return 1;
}
/********************************************************************************************************
** @TESTED COIN DEVICE:
** 	1.Panasonic:EUC 82M
** 	2.NRI:BV6-F-CNY
** 	3.MEI:CF 7000/8000
** 	4.FJ:FJM211CN
** @Operation Flow:
	1.CoinAcceptorChanger Initialization flow:
		1.1 RESET											--0x08
		1.2 POLL to obtain "Just Reset" response			--0x09
		1.3 EXPANSION IDENTIFICATION						--0x0F 0x00
		1.4 EXPANSION FEATURE ENABLE 						--0x0F 0x00
		1.5 EXPANSION SEND DIAG STATUS 						--0x0F 0x05
		1.6 TUBE STATUS										--0x0A
		1.7 COIN TYPE										--0x0C
		NOTE:A minimum 500 msec delay is required between a reset and the first TUBE STATUS command.
	2.CoinAcceptorChanger Payout flow(Level 3):
		2.1 Expansion Payout Coin							--0x0F 0x02
		2.2 Expansion Payout Value Poll						--0x0F 0x03
		2.3 Expansion Payout Status							--0x0F 0x04	
	3.CoinAcceptorChanger Poll flow:
		3.1 POLL											--0x09
	4.Note:This project is not support Level-1/Level-2 coin acceptor/changer.
*********************************************************************************************************/
/**************************************End Of File*******************************************************/
