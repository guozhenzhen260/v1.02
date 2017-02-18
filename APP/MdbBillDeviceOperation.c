/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			MdbBillDeviceOperation.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\API\API.h"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "MdbBillDeviceOperation.h"
unsigned int BillScale;	//±ÈÀýÒò×Ó
/*********************************************************************************************************
** @APP Function name:   MdbBillResetAndSetup
** @APP Description:	 Init BillDevice And Get Info
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
*********************************************************************************************************/
void MdbBillResetAndSetup(void)
{
	unsigned char i,err,MDBAck[36],MDBAckLen;
	uint16_t BillDecimal;		  //10^Ð¡ÊýÎ»Êý
	err = 0x00;
	//Paramter
	memset((void *)MDBBillDevice.ManufacturerCode,0x00,3);
	memset((void *)MDBBillDevice.CurrencyCode,0x00,2);
	memset((void *)MDBBillDevice.BillSecurityLevel,0x00,2);
	memset((void *)MDBBillDevice.BillTypeEscrowEnable,0x00,16);
	memset((void *)MDBBillDevice.BillTypeCredit,0x00,16);
	memset((void *)MDBBillDevice.Level2Feature,0x00,4);
	//Poll,MEI-VN2672/ITL-NV9/ITL-NV11/CASHCODE/ICT
	API_SYSTEM_TimerChannelSet(0,400);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_Poll_BillDevice(MDBAck,&MDBAckLen))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(50);
	}
	if(err == 0x00)
	{
		//Poll err turn to Reset flow
		#ifdef APP_DBG_MDB_BILL
		Trace("InitBillDevice():Fst Poll ERR,turn to reset flow\r\n");
		#endif
		API_SYSTEM_TimerChannelSet(0,200);
		while(API_SYSTEM_TimerReadChannelValue(0))
		{
			if(API_MDB_Reset_BillDevice())
			{
				err = 0x01;
				break;
			}
			vTaskDelay(10);
		}
		if(err == 0x00)
		{
			MDBBillDevice.Oneline = 0x00;
			#ifdef APP_DBG_MDB_BILL
			Trace("InitBillDevice():Reset ERR\r\n");
			#endif
			return;
		}
		else
		{
			vTaskDelay(20);
			err = API_MDB_Poll_BillDevice(MDBAck,&MDBAckLen);
			if(err)
			{
				if((MDBAckLen >= 0x01) && (MDBAck[0] == 0x06))
				{
					#ifdef APP_DBG_MDB_BILL
					Trace("InitBillDevice():POLL to obtain JUST RESET OK\r\n");
					#endif
					MDBBillDevice.Oneline = 0x01;
				}
				else
				{
					#ifdef APP_DBG_MDB_BILL
					Trace("InitBillDevice():POLL Ok but ack ERR!(len = %d)\r\n",MDBAckLen);
					#endif
					MDBBillDevice.Oneline = 0x00;
					return;			
				}			
			}
			else
			{
				#ifdef APP_DBG_MDB_BILL
				Trace("InitBillDevice():POLL to obtain JUST RESET ERR!\r\n");
				#endif
				MDBBillDevice.Oneline = 0x00;
				return;
			}
		}
	}
	else
		MDBBillDevice.Oneline = 0x01;
	err = 0x00;
	//Setup
	API_SYSTEM_TimerChannelSet(0,500);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_Setup_BillDevice(MDBAck,&MDBAckLen))
		{
			err = 0x01;
			break;
		}
	}
	if(err)
	{
		MDBBillDevice.Level = MDBAck[0];
		MDBBillDevice.CurrencyCode[0] = MDBAck[1];
		MDBBillDevice.CurrencyCode[1] = MDBAck[2];
		MDBBillDevice.ScalingFactor = (((unsigned int)MDBAck[3]) << 8) + MDBAck[4];
		MDBBillDevice.Decimal = MDBAck[5];
		BillDecimal = 100;
	      for(i = 0; i < MDBAck[5]; i++) 
	      {
		   BillDecimal /= 10;
	      }
		BillScale = (uint32_t)((((uint16_t)MDBAck[3]) << 8) | MDBAck[4]) * BillDecimal;
		MDBBillDevice.StackerCapacity = (((unsigned int)MDBAck[6]) << 8) + MDBAck[7];
		MDBBillDevice.BillSecurityLevel[0] = MDBAck[8];
		MDBBillDevice.BillSecurityLevel[1] = MDBAck[9];
		MDBBillDevice.Escrow = MDBAck[10];
		for(i=0;i<(MDBAckLen - 11);i++)
		{
			if(MDBAck[11+i] == 0xFF) 
			{
			     break;
			}			
			MDBBillDevice.BillTypeCredit[i] = MDBAck[11 + i];
		}
		#ifdef APP_DBG_MDB_BILL
		Trace("InitBillDevice() SETUP OK Info:\r\n");
		Trace("    Level = %d\r\n",MDBBillDevice.Level);
		Trace("    CurCode = %d%d\r\n",MDBBillDevice.CurrencyCode[0],MDBBillDevice.CurrencyCode[1]);
		Trace("    ScalingFactor = %d\r\n",MDBBillDevice.ScalingFactor);
		Trace("    Decimal = %d\r\n",MDBBillDevice.Decimal);
		Trace("    StackerCapacity = %d\r\n",MDBBillDevice.StackerCapacity);
		Trace("    BillSecurityLevel = %d%d\r\n",MDBBillDevice.BillSecurityLevel[0],MDBBillDevice.BillSecurityLevel[1]);
		Trace("    Escrow = %d\r\n",MDBBillDevice.Escrow);
		Trace("    BillType:");
		for(i=0;i<(MDBAckLen - 11);i++)
		{
			Trace("%02X ",MDBBillDevice.BillTypeCredit[i]);
		}
		Trace("\r\n");
		Trace("\r\nDrvBILLDec=%d,%d,%d",MDBAck[3],MDBAck[4],MDBAck[5]);
		Trace("\r\nDrvBILLDec2=%ld,%ld,%ld", ((((uint16_t)MDBAck[3]) << 8) | MDBAck[4]),BillDecimal,BillScale);		
		#endif
	}
	else
	{
		MDBBillDevice.Oneline = 0x00;
		#ifdef APP_DBG_MDB_BILL
		Trace("InitBillDevice():POLL ok but Setup ERR!\r\n");
		#endif
		return;
	}
	err = 0x00;
	//EXPANSION only level 2 support BillRecycSupport
	if((MDBBillDevice.Level > 0x02) || (MDBBillDevice.Level == 0x00))
	{
		MDBBillDevice.Oneline = 0x00;
		#ifdef APP_DBG_MDB_BILL
		Trace("InitBillDevice():unknowned Level\r\n");
		#endif
		return;	
	}
	if(MDBBillDevice.Level == 0x01)
	{
		#ifdef APP_DBG_MDB_BILL
		Trace("InitBillDevice():BillRecycSupport is not support for Device is Level 1\r\n");
		#endif
		MDBBillDevice.BillRecycSupport = 0x00;
	}
	if(MDBBillDevice.Level == 0x02)
	{
		API_SYSTEM_TimerChannelSet(0,500);
		while(API_SYSTEM_TimerReadChannelValue(0))
		{
			if(API_MDB_L2IdentificationWithOptBit_BillDevice(MDBAck,&MDBAckLen))
			{
				err = 0x01;
				break;
			}
		}
		if(err == 0x00)
		{
			#ifdef APP_DBG_MDB_BILL
			Trace("InitBillDevice():L2Identification ERR\r\n");
			#endif
			return;
		}
		else
		{
			if(MDBAckLen >= 33)
			{
				if(MDBAck[32] & 0x02)
				{
					#ifdef APP_DBG_MDB_BILL
					Trace("InitBillDevice():BillRecycSupport is support\r\n");
					#endif
					MDBBillDevice.BillRecycSupport = 0x01;
					MDBBillDevice.Level2Feature[0] = MDBAck[29];
					MDBBillDevice.Level2Feature[1] = MDBAck[30];
					MDBBillDevice.Level2Feature[2] = MDBAck[31];
					MDBBillDevice.Level2Feature[3] = MDBAck[32];
				}
				else
				{
					#ifdef APP_DBG_MDB_BILL
					Trace("InitBillDevice():BillRecycSupport is not support\r\n");
					#endif
					MDBBillDevice.BillRecycSupport = 0x00;
				}
			}
			else
				MDBBillDevice.BillRecycSupport = 0x00;
		}
	}
	//Enable Feature Level 2 only
	err = 0x00;
	if(MDBBillDevice.Level == 0x02)
	{
		API_SYSTEM_TimerChannelSet(0,100);
		while(API_SYSTEM_TimerReadChannelValue(0))
		{
			if(API_MDB_L2FeatureEnable_BillDevice((unsigned char *)MDBBillDevice.Level2Feature))
			{
				err = 0x01;
				break;
			}
			vTaskDelay(5);
		}
		#ifdef APP_DBG_MDB_BILL
		if(err)
			Trace("InitBillDevice():Enable feature ok\r\n");
		else
			Trace("InitBillDevice():Enable feature ERR\r\n");
		#endif
	}
	//Get Stacker status
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_Stacker_BillDevice(MDBAck,&MDBAckLen))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err)
	{
		if(MDBAckLen == 0x02)
		{
			#ifdef APP_DBG_MDB_BILL
			Trace("InitBillDevice():Get Stacker status Ok\r\n");
			#endif
			MDBBillDevice.StackerStatus = (MDBAck[0] & 0x80);
		}
		#ifdef APP_DBG_MDB_BILL
		else
			Trace("InitBillDevice():Get Stacker status ack ok but data err!\r\n");
		#endif
	}
	#ifdef APP_DBG_MDB_BILL
	else
		Trace("InitBillDevice():Get Stacker status ERR\r\n");
	#endif
	//Bill Type Enable
	err = 0x00;
	API_SYSTEM_TimerChannelSet(0,200);
	while(API_SYSTEM_TimerReadChannelValue(0))
	{
		if(API_MDB_BillType_BillDevice(0x003F,0x003F))
		{
			err = 0x01;
			break;
		}
		vTaskDelay(10);
	}
	if(err)
	{
		#ifdef APP_DBG_MDB_BILL
		Trace("InitBillDevice():Enable Bill type ok\r\n");
		#endif
	}
	#ifdef APP_DBG_MDB_BILL
	else
		Trace("InitBillDevice():Enable Bill type ERR\r\n");
	#endif
	/*
	//Recycler Setup
	err = 0x00;
	if(MDBBillDevice.BillRecycSupport)
	{
		API_SYSTEM_TimerChannelSet(0,200);
		while(API_SYSTEM_TimerReadChannelValue(0))
		{
			if(API_MDB_RecyclerSetup_BillDevice(MDBAck,&MDBAckLen))
			{
				err = 0x01;
				break;
			}
			vTaskDelay(10);
		}
		if(err)
		{
			MDBBillDevice.BillTpyeRoutedToRecycler = ((unsigned short)MDBAck[0] << 8) +  MDBAck[1];
			#ifdef APP_DBG_MDB_BILL
			Trace("InitBillDevice():Recycler setup ok\r\n");
			#endif
		}
		else
		{
			#ifdef APP_DBG_MDB_BILL
			Trace("InitBillDevice():Recycler setup ERR\r\n");
			#endif
			MDBBillDevice.BillTpyeRoutedToRecycler = 0x00;
			return;
		}
	}*/
}

/*********************************************************************************************************
** @APP Function name:      BillDevProcess
** @APP Descriptions:       Ö½±ÒÆ÷ÊÕ±ÒÑ¹³®²Ù×÷
** @APP input parameters:   ÎÞ
** @APP output parameters:  RecvMoney¡ª¡ªÊÕÈëµÄÖ½±Ò½ð¶îÒÔ·ÖÎªµ¥Î
							BillType¡ª¡ªÊÕÈëµÄÖ½±ÒÍ¨µÀ
							billOpt--Ö½±ÒÆ÷¿ØÖÆ²ÎÊý
							billOptBack--Ö½±ÒÆ÷¿ØÖÆ·µ»Ø½á¹û
** Returned value:      	ÓÐÖ½±ÒÊÕÈë·µ»Ø1£¬ÎÞ·µ»Ø0
*********************************************************************************************************/
uint8_t BillDevProcess(uint32_t *RecvMoney,unsigned char *BillType,unsigned char billOpt,unsigned char *billOptBack)
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus;
	uint8_t type=0,i=0;
	static uint8_t billrec=0;
	switch(billOpt)
	{
		case MBOX_BILLENABLEDEV:
			//Trace("enable bill\r\n");
			MdbBillTypeEanbleOrDisable(1,1);
			break;
		case MBOX_BILLDISABLEDEV:
			//Trace("disable bill\r\n");
			//Trace("\r\n DrvBILLDISABLEDEV opt");
			MdbBillTypeEanbleOrDisable(0,1);
			break;			
		case MBOX_BILLESCROW:
			//Trace("\r\nescrow bill");
			if(API_MDB_Escrow_BillDevice(1))
				*billOptBack = 2;	
			else
				*billOptBack = 1;	
			break;	
		case MBOX_BILLRETURN:
			//Trace("\r\nescrow bill");
			if(API_MDB_Escrow_BillDevice(0))
				*billOptBack = 4;	
			else
				*billOptBack = 3;	
			break;
		default:break;	
	}
	vTaskDelay(10);
	//ÂÖÑ¯Ö½±ÒÆ÷ÊÇ·ñÓÐÊÕµ½Ö½±Ò£¬ÓÐ·µ»Ø1
	ComStatus = API_MDB_Poll_BillDevice(&BillRdBuff[0],&BillRdLen);
	if(ComStatus == 1)
	{
		MdbBillErr.Communicate = 0;
		billrec=0;
		vTaskDelay(10);
		if(BillRdLen==0)
		{
			vTaskDelay(10);
			memset(&MdbBillErr,0,sizeof(MdbBillErr));
		}
		for(i = 0; i < BillRdLen; i++) 
		{
			if((BillRdBuff[i]&0xf0)==0x90)
			{
				type = BillRdBuff[i]&0x0f;				
				{
					*RecvMoney = (uint32_t)MDBBillDevice.BillTypeCredit[type] * BillScale;
					*BillType = type;
					#ifdef APP_DBG_MDB_BILL
					Trace("Drvbill=%d,%ld,%d\r\n",MDBBillDevice.BillTypeCredit[type],*RecvMoney,*BillType);
					#endif
					vTaskDelay(10);
					return 1;
				}
			}
			else if((BillRdBuff[i]&0xf0)==0xa0)
			{
				#ifdef APP_DBG_MDB_BILL
				Trace("\r\n Drvbill return");
				#endif
				*billOptBack = 1;
			}
			else if((BillRdBuff[i] & 0xF0) == 0) 
			{   
				//validator status
			    switch(BillRdBuff[i]) 
				{
		            case 0x01:			                 //defective motor
						#ifdef APP_DBG_MDB_BILL
		            	Trace("\r\n Drvbill motor");
						#endif
				        MdbBillErr.moto = 1;
				        break;
			        case 0x02:			                 //sensor problem
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbill sensor");
					    MdbBillErr.sensor = 1;
						#endif
				        break;
			        case 0x03:			                 //validator busy
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbil busy");
						#endif
						MdbBillErr.disable = 1;
			        	break;
			        case 0x04:			                 //rom chksum err
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbill chksum");
						#endif
				    	MdbBillErr.romchk = 1;
						break;
			        case 0x05:			                 //validator jammed
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbill jammed");
						#endif
					    MdbBillErr.jam = 1;			       
				        break;	
			        case 0x06:			                 //validator was reset
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbil reset");
						#endif
						break;
			        case 0x07:			                 //bill removed
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbil removed");
						#endif
			        	break;
			        case 0x08:			                 //cash box out of position
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbill removeCash");
						#endif
					    MdbBillErr.removeCash = 1;	
				        break;
			        case 0x09:			                 //validator disabled	
			        	//Trace("\r\n Drvbill disabled");
			        	MdbBillErr.disable = 1;
						break;
			
			        case 0x0A:			                 //invalid escrow request
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbil invalid");
						#endif
			       		break;
			
			        case 0x0B:			                 //bill rejected
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbil rejected");
						#endif
			        	break;
			        case 0x0C:			                 //possible credited bill removal
						#ifdef APP_DBG_MDB_BILL
			        	Trace("\r\n Drvbill cashErr");
						#endif
					    MdbBillErr.cashErr = 1;	
				        break;
					case 0x29:
						#ifdef APP_DBG_MDB_BILL
						Trace("\r\n Drvbill recyclerErr");
						#endif
						MdbBillErr.recyErr=1;
						break;
					default:
						#ifdef APP_DBG_MDB_BILL
						Trace("\r\n Drvbill default");
						#endif
						memset(&MdbBillErr,0,sizeof(MdbBillErr));
					    break;
		         }
		    }
		}
	}
	else
	{
		billrec++;
		if(billrec>=30)
		{
			billrec=0;
		}
		MdbBillErr.Communicate = 1;
	}	
	return 0;
}
/*********************************************************************************************************
** @APP Function name:  MdbCoinEanbleOrDisable
** @APP Input para:     None
** @APP retrun para:    None
*********************************************************************************************************/
void MdbBillTypeEanbleOrDisable(unsigned char AcceptCtrl,unsigned char DispenCtrl)
{
	unsigned int Acmd,Dcmd;
	if(AcceptCtrl)
	{
		if(((TradeParam.TypeBillEnable >> 15) & 0x0001)!=0)
			Acmd |= 0x01;
		if(((TradeParam.TypeBillEnable >> 14) & 0x0001)!=0)
			Acmd |= 0x02;
		if(((TradeParam.TypeBillEnable >> 13) & 0x0001)!=0)
			Acmd |= 0x04;
		if(((TradeParam.TypeBillEnable >> 12) & 0x0001)!=0)
			Acmd |= 0x08;
		if(((TradeParam.TypeBillEnable >> 11) & 0x0001)!=0)
			Acmd |= 0x10;
		if(((TradeParam.TypeBillEnable >> 10) & 0x0001)!=0)
			Acmd |= 0x20;
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
		if(API_MDB_BillType_BillDevice(Acmd,Dcmd))
			break;
		vTaskDelay(10);
	}
}
/**************************************End Of File*******************************************************/
