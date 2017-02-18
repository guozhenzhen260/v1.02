/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			BillVallidatorReycler.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\DRIVERS\MDB.h"
#include "..\DRIVERS\uart0.h"
#include "API.H"
#include "..\API\OS.H"
#include "..\App\ParamDefine.h"
#define	 BILLVALIDATIOR_BASE_MDBADDR	0x30
#define	 RESET_BILLVALIDATIOR			(BILLVALIDATIOR_BASE_MDBADDR + 0x00)
#define	 SETUP_BILLVALIDATIOR			(BILLVALIDATIOR_BASE_MDBADDR + 0x01)
#define	 SECURITY_BILLVALIDATIOR		(BILLVALIDATIOR_BASE_MDBADDR + 0x02)
#define	 POLL_BILLVALIDATIOR			(BILLVALIDATIOR_BASE_MDBADDR + 0x03)
#define	 BILLTYPE_BILLVALIDATIOR		(BILLVALIDATIOR_BASE_MDBADDR + 0x04)
#define	 ESCROW_BILLVALIDATIOR			(BILLVALIDATIOR_BASE_MDBADDR + 0x05)
#define	 STACKER_BILLVALIDATIOR			(BILLVALIDATIOR_BASE_MDBADDR + 0x06)
#define	 EXPANSION_BILLVALIDATIOR		(BILLVALIDATIOR_BASE_MDBADDR + 0x07)
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Reset_BillDevice
** @DRIVER Description:   	Force MDB bus reset
** @DRIVER Input para:     	None
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_Reset_BillDevice(void)
{
	unsigned char err,cmd,ack,acklen;
	cmd = RESET_BILLVALIDATIOR;
	err = MDBConversation(&cmd,1,&ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("API_MDB_Reset_BillDevice OK \r\n");
	}
	else
		Trace("API_MDB_Reset_BillDevice ERR!\r\n");
	#endif
	return err;	
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Setup_BillDevice
** @DRIVER Description:   	Setup Bill device(status)
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:ack;*acklen:lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_Setup_BillDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd;
	cmd = SETUP_BILLVALIDATIOR;
	err = MDBConversation(&cmd,1,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("API_MDB_Setup_BillDevice OK DATA(%d):\r\n",*acklen);
		for(cmd=0;cmd<*acklen;cmd++)
		{
			Trace("%02X ",ack[cmd]);
		}
		Trace("\r\n");
	}
	else
		Trace("API_MDB_Setup_BillDevice ERR!\r\n");
	if(err)
	{
		Trace("Bill Validator/Recycler Info:\r\n");
		Trace("    Level = %02d\r\n",ack[0]);
		Trace("    CurCode = %04X\r\n",(((unsigned int)ack[1] << 8) + ack[2]));
		Trace("    ScalingFactor = %04X\r\n",(((unsigned int)ack[3] << 8) + ack[4]));
		Trace("    Decimal = %02d\r\n",ack[5]);
		Trace("    StacketCapacity = %04X\r\n",(((unsigned int)ack[6] << 8) + ack[7]));
		Trace("    BillSecurity Level = %02x %02X\r\n",ack[8],ack[9]);
		Trace("    Escrow = %d\r\n",ack[10]);
	}
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Security_BillDevice
** @DRIVER Description:   	Set bills's security
** @DRIVER Input para:     	Y1,Y2:security data
** @DRIVER Output para:     none
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_Security_BillDevice(unsigned char Y1,unsigned char Y2)
{
	unsigned char err,cmd[3],ack[36],acklen;
	cmd[0] = SETUP_BILLVALIDATIOR;
	cmd[1] = Y1;
	cmd[2] = Y2;
	err = MDBConversation(cmd,3,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("API_MDB_Security_BillDevice OK \r\n");
	else
		Trace("API_MDB_Security_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Poll_BillDevice
** @DRIVER Description:   	Poll Bill device
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:ack;*acklen:lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_Poll_BillDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd;
	cmd = POLL_BILLVALIDATIOR;
	err = MDBConversation(&cmd,1,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("API_MDB_Poll_BillDevice OK DATA(%d):\r\n\r\n",*acklen);
		for(cmd=0;cmd<*acklen;cmd++)
		{
			Trace("%02X ",ack[cmd]);
		}
		Trace("\r\n\r\n");
	}
	else
		Trace("API_MDB_Poll_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_BillType_BillDevice
** @DRIVER Description:   	Enable Bill type
** @DRIVER Input para:     	Y1,Y2:bill type
** @DRIVER Output para:     none
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_BillType_BillDevice(unsigned int Y1Y2,unsigned int Y3Y4)
{
	unsigned char err,cmd[5],ack[36],acklen;
	cmd[0] = BILLTYPE_BILLVALIDATIOR;
	cmd[1] = (unsigned char)(Y1Y2 >> 8);
	cmd[2] = (unsigned char)Y1Y2;
	cmd[3] = (unsigned char)(Y3Y4 >> 8);
	cmd[4] = (unsigned char)Y3Y4;
	err = MDBConversation(cmd,5,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("API_MDB_BillType_BillDevice OK\r\n");
	else
		Trace("API_MDB_BillType_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Escrow_BillDevice
** @DRIVER Description:   	Return or Stack Bill
** @DRIVER Input para:     	Y1:0x00 return bill;0x01:Stack the bill
** @DRIVER Output para:     none
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_Escrow_BillDevice(unsigned char Y1)
{
	unsigned char err,cmd[5],ack[36],acklen;
	uint8_t i=0;
	cmd[0] = ESCROW_BILLVALIDATIOR;
	cmd[1] = Y1;
	err = MDBConversation(cmd,2,ack,&acklen);
	#ifdef DEBUG_MDB
	Trace("\r\nBillDrv Escrow = %d",err);
	#endif
	API_SYSTEM_TimerChannelSet(0,13*100);
	while(API_SYSTEM_TimerReadChannelValue(0))	
	{
		//轮询检测是否压仓成功
		err = API_MDB_Poll_BillDevice(ack,&acklen);
		#ifdef DEBUG_MDB
		Trace("\r\nBillDrv Escrowsend >> %#02x\r\n",0x33);
		#endif
		if(err == 1)
		{
			for(i = 0; i < acklen; i++) 
			{
				//0x80到钞箱,0xb0到循环斗
				if( ((ack[i]&0xf0)==0x80)||((ack[i]&0xf0)==0xb0) )
				{
					#ifdef DEBUG_MDB					
					Trace("\r\nBillDrv escrow1");
					#endif
					vTaskDelay(10);
					API_MDB_Stacker_BillDevice(ack,&acklen);
					return 1;	
				}
				//没有压抄成功，中途退出
				else if((ack[i]&0xf0)==0xa0)
				{
					#ifdef DEBUG_MDB
					Trace("\r\nBillDrv escrow return");
					#endif
					return 0;
				}
			}
			vTaskDelay(10);
		}
		vTaskDelay(10);
	}
	#ifdef DEBUG_MDB
	if(err)
	{
		if(Y1)
			Trace("API_MDB_Escrow_BillDevice OK (Stack the bill)\r\n");
		else
			Trace("API_MDB_Escrow_BillDevice OK (return the bill)\r\n");
	}
	else
		Trace("API_MDB_Escrow_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Stacker_BillDevice
** @DRIVER Description:   	Get Stacker status
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:ack;*acklen:lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_Stacker_BillDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd;
	#ifdef DEBUG_MDB
	unsigned char j;
	#endif
	cmd = STACKER_BILLVALIDATIOR;
	err = MDBConversation(&cmd,1,ack,acklen);
	if(err == 1)
	{
		#ifdef DEBUG_MDB
		Trace("\r\nBillDrv Escrowr buf:",acklen);
		for(j = 0; j < *acklen; j++) 
		{
			Trace("[%d] ", ack[j]);
		}
		Trace("\r\n");
		#endif
		if((ack[0]&0x80)==0x80)
		{
			MdbBillErr.cashErr = 1;
			#ifdef DEBUG_MDB
			Trace("\r\nBillDrv Escrowr full");
			#endif
		}
		else
		{
			MdbBillErr.cashErr = 0;
			#ifdef DEBUG_MDB
			Trace("\r\nBillDrv Escrowr Empty");
			#endif
		}	
	}
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("API_MDB_Stacker_BillDevice OK DATA(%d):\r\n\r\n",*acklen);
		for(cmd=0;cmd<*acklen;cmd++)
		{
			Trace("%02X ",ack[cmd]);
		}
		Trace("\r\n\r\n");
	}
	else
		Trace("API_MDB_Stacker_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_L2IdentificationWithOptBit_BillDevice
** @DRIVER Description:   	Get Stacker status
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:ack;*acklen:lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_L2IdentificationWithOptBit_BillDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd[2];
	cmd[0] = EXPANSION_BILLVALIDATIOR;
	cmd[1] = 0x02;
	err = MDBConversation(cmd,2,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("API_MDB_L2IdentificationWithOptBit_BillDevice OK DATA(%d):\r\n\r\n",*acklen);
		for(cmd[0]=0;cmd[0]<*acklen;cmd[0]++)
		{
			Trace("%02X ",ack[cmd[0]]);
		}
		Trace("\r\n\r\n");
	}
	else
		Trace("API_MDB_L2IdentificationWithOptBit_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_L2FeatureEnable_BillDevice
** @DRIVER Description:   	Feature Enable
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:ack;*acklen:lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_L2FeatureEnable_BillDevice(unsigned char *VmcData)
{
	unsigned char err,cmd[6],ack[36],acklen;
	cmd[0] = EXPANSION_BILLVALIDATIOR;
	cmd[1] = 0x01;
	cmd[2] = VmcData[0];
	cmd[3] = VmcData[1];
	cmd[4] = VmcData[2];
	cmd[5] = VmcData[3];
	err = MDBConversation(cmd,6,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("API_MDB_L2FeatureEnable_BillDevice OK DATA(%d)\r\n",acklen);
	else
		Trace("API_MDB_L2FeatureEnable_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_RecyclerSetup_BillDevice
** @DRIVER Description:   	Feature Enable
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:ack;*acklen:lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
extern unsigned char API_MDB_RecyclerSetup_BillDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd[2];
	cmd[0] = EXPANSION_BILLVALIDATIOR;
	cmd[1] = 0x03;
	err = MDBConversation(cmd,2,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("API_MDB_RecyclerSetup_BillDevice OK DATA(%d):\r\n\r\n",*acklen);
		for(cmd[0]=0;cmd[0]<*acklen;cmd[0]++)
		{
			Trace("%02X ",ack[cmd[0]]);
		}
		Trace("\r\n\r\n");
	}
	else
		Trace("API_MDB_RecyclerSetup_BillDevice ERR!\r\n");
	#endif
	return err;
}
/*
extern unsigned char API_MDB_Diagnostics_BillDevice(unsigned char *ack,unsigned char *acklen)
extern unsigned char API_MDB_RecyclerSetup_BillDevice(unsigned char *ack,unsigned char *acklen)
extern unsigned char API_MDB_RecyclerEnable_BillDevice(unsigned char *cmd,unsigned char *cmdlen)
extern unsigned char API_MDB_BillDispenseStatus_BillDevice(unsigned char *ack,unsigned char *acklen)
extern unsigned char API_MDB_DispenseBill_BillDevice(unsigned char Y1,unsigned char Y2,unsigned char Y3)
extern unsigned char API_MDB_DispenseValue_BillDevice(unsigned char *ack,unsigned char *acklen)
extern unsigned char API_MDB_PayoutStatus_BillDevice(unsigned char *ack,unsigned char *acklen)
extern unsigned char API_MDB_PayoutValuePoll_BillDevice(unsigned char *ack,unsigned char *acklen)
extern unsigned char API_MDB_PayoutCancel_BillDevice(unsigned char *ack,unsigned char *acklen)
*/
/**************************************End Of File*******************************************************/
