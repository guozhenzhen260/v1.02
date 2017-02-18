/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			CoinAcceptorChanger.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\DRIVERS\MDB.h"
#include "..\DRIVERS\uart0.h"
#include "API.H"
#define	 COINACCEPTORCHANGER_BASE_MDBADDR	0x08
#define	 RESET_COINACCEPTORCHANGER			(COINACCEPTORCHANGER_BASE_MDBADDR + 0x00)
#define	 SETUP_COINACCEPTORCHANGER			(COINACCEPTORCHANGER_BASE_MDBADDR + 0x01)
#define	 TUBESTATUS_COINACCEPTORCHANGER		(COINACCEPTORCHANGER_BASE_MDBADDR + 0x02)
#define	 POLL_COINACCEPTORCHANGER			(COINACCEPTORCHANGER_BASE_MDBADDR + 0x03)
#define	 COINTYPE_COINACCEPTORCHANGER		(COINACCEPTORCHANGER_BASE_MDBADDR + 0x04)
#define	 DESPENSE_COINACCEPTORCHANGER		(COINACCEPTORCHANGER_BASE_MDBADDR + 0x05)
#define	 EXPANSION_COINACCEPTORCHANGER		(COINACCEPTORCHANGER_BASE_MDBADDR + 0x07)
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Reset_CoinDevice
** @DRIVER Description:   	software reset CoinAcceptorChanger;
** @DRIVER Input para:     	None
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_Reset_CoinDevice(void)
{
	unsigned char err,cmd,ack,acklen;
	cmd = RESET_COINACCEPTORCHANGER;
	err = MDBConversation(&cmd,1,&ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("@API MDB CoinReset:OK \r\n");
	}
	else
		Trace("@API MDB CoinReset:ERR\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Setup_CoinDevice
** @DRIVER Description:   	SETUP,Get CoinAcceptorChanger's para
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:data;*acklen:data lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_Setup_CoinDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd;
	cmd = SETUP_COINACCEPTORCHANGER;
	err = MDBConversation(&cmd,1,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("@API MDB DATA(%d):",*acklen);
		for(cmd=0;cmd<*acklen;cmd++)
		{
			Trace("%02X ",ack[cmd]);
		}
		Trace("\r\n");
	}
	else
		Trace("@API MDB ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_TubeStatus_CoinDeviceTubeStatus
** @DRIVER Function name:   Get all cointype's tube status
** @DRIVER Description:   	SETUP,Get CoinAcceptorChanger's para
** @DRIVER Input para:     	None
** @DRIVER Output para:     *CoinType0:CoinType0's tube status;*CoinType1:CoinType1's tube status
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_TubeStatus_CoinDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd;
	cmd = TUBESTATUS_COINACCEPTORCHANGER;
	err = MDBConversation(&cmd,1,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("@API MDB Coin TubeStatus OK DATA(%d):",*acklen);
		for(cmd=0;cmd<*acklen;cmd++)
		{
			Trace("%02X ",ack[cmd]);
		}
		Trace("\r\n");
	}
	else
		Trace("@API MDB Coin TubeStatus:ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_Poll_CoinDevice
** @DRIVER Description:   	POLL CoinAcceptorChanger
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:data;*acklen:data lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_Poll_CoinDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd;
	cmd = POLL_COINACCEPTORCHANGER;
	err = MDBConversation(&cmd,1,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("@API MDB Coin Poll:OK DATA(%d):",*acklen);
		for(cmd=0;cmd<*acklen;cmd++)
		{
			Trace("%02X ",ack[cmd]);
		}
		Trace("\r\n");
	}
	else
		Trace("@API MDB Coin Poll:ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_CoinType_CoinDevice  
** @DRIVER Description:   	POLL CoinAcceptorChanger
** @DRIVER Input para:     	CoinEnable:Enable coin Deposited enable;DispenseEnable:Manual Dispense Enable
** @DRIVER Output para:     None
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_CoinType_CoinDevice(unsigned char CoinAcceptorCtrl,unsigned char CoinDispenseCtrl)
{
	unsigned char i,err,cmd[5],ack[36],acklen;
	i = i;
	cmd[0] = COINTYPE_COINACCEPTORCHANGER;
	cmd[1] = 0x00;
	cmd[2] = CoinAcceptorCtrl;
	cmd[3] = 0x00;
	cmd[4] = CoinDispenseCtrl;
	err = MDBConversation(cmd,5,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("@API MDB Coin Type set OK:00,%02X,00,%02X\r\n",CoinAcceptorCtrl,CoinDispenseCtrl);
	else
		Trace("@API MDB Coin Type set:ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_ExpanIdentification_CoinDevice
** @DRIVER Description:   	Get CoinAcceptorChanger's identification
** @DRIVER Input para:     	None
** @DRIVER Output para:     *ack:data;*acklen:data lenght
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_ExpanIdentification_CoinDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char i,err,cmd[2];
	i = i;
	cmd[0] = EXPANSION_COINACCEPTORCHANGER;
	cmd[1] = 0x00;
	err = MDBConversation(cmd,2,ack,acklen);
	#ifdef DEBUG_MDB
	if(err)
	{
		Trace("@API MDB Coin Identification OK DATA(%d):\r\n\r\n",*acklen);
		for(i=0;i<*acklen;i++)
		{
			Trace("%02X ",ack[i]);
		}
		Trace("\r\n\r\n");
	}
	else
		Trace("@API MDB Coin Identification:ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_ExpanFeatureEnable_CoinDevice
** @DRIVER Description:   	set CoinAcceptorChanger's feature(Data in identification)
** @DRIVER Input para:     	*Feature:feature data(get from identification)
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_ExpanFeatureEnable_CoinDevice(unsigned char *Feature)
{
	unsigned char err,cmd[6],ack[36],acklen;
	cmd[0] = EXPANSION_COINACCEPTORCHANGER;
	cmd[1] = 0x01;
	cmd[2] = Feature[0];
	cmd[3] = Feature[1];
	cmd[4] = Feature[2];
	cmd[5] = Feature[3];
	err = MDBConversation(cmd,6,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("@API MDB Coin Feature enable:OK DATA(%02X,%02X,%02X,%02X)\r\n",cmd[2],cmd[3],cmd[4],cmd[5]);
	else
		Trace("@API MDB Coin Feature enable:ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_ExpanPayout_CoinDevice
** @DRIVER Input para:     	value:value of coin that CoinAcceptorChanger will dispense
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_ExpanPayout_CoinDevice(unsigned char value)
{
	unsigned char err,cmd[5],ack[36],acklen;
	cmd[0] = EXPANSION_COINACCEPTORCHANGER;
	cmd[1] = 0x02;
	cmd[2] = value;
	err = MDBConversation(cmd,3,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("@API MDB Coin ExpanPayout:OK (Value=%D)\r\n",cmd[2]);
	else
		Trace("@API MDB Coin ExpanPayout:ERR!\r\n");
	#endif
	return err;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_ExpanPayoutStatus_CoinDevice
** @DRIVER Input para:     	None
** @DRIVER Output para:     *Type0Payout:num of Type0 dispensed*Type1Payout:num of Type1 dispensed
** @DRIVER retrun para:     0x01:Payout Finished;0x00:Buzy;other:err
*********************************************************************************************************/
unsigned char API_MDB_ExpanPayoutStatus_CoinDevice(unsigned char *ack,unsigned char *acklen)
{
	unsigned char err,cmd[2],finished;
	err = 0x00;
	cmd[0] = EXPANSION_COINACCEPTORCHANGER;
	cmd[1] = 0x03;
	err = MDBConversation(cmd,2,ack,acklen);
	Trace("\r\nDrvChange s = %d,%d,%d,%d\r\n",err,ack[0],ack[1],*acklen);
	if(err)
	{
		if(acklen > 0)
		{
			finished = 0x01;
		}
		else
		{
			#ifdef DEBUG_MDB
			Trace("@API MDB Coin ExpanPayoutStatus Ok:Finished\r\n");
			#endif
			finished = 0;
		}
	}
	else
	{
		#ifdef DEBUG_MDB
		Trace("@API MDB Coin ExpanPayoutStatus:ERR!\r\n");
		#endif
		finished = 0x00;
	}
	return finished;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_ExpanPayoutValuePoll_CoinDevice
** @DRIVER Input para:     	None
** @DRIVER Output para:     *value:the value dispensed form the last Payout cmd
** @DRIVER retrun para:     0x01:Buzy;0x00:Payout Finished;other:err
*********************************************************************************************************/
unsigned char API_MDB_ExpanPayoutValuePoll_CoinDevice(unsigned char *value)
{
	unsigned char err,cmd[5],ack[36],acklen,finished = 0x00;
	cmd[0] = EXPANSION_COINACCEPTORCHANGER;
	cmd[1] = 0x04;
	err = MDBConversation(cmd,2,ack,&acklen);
	#ifdef DEBUG_MDB
	Trace("@API MDB Coin DrvChangesend = %d,%d,%d,%d\r\n",err,ack[0],ack[1],acklen);
	#endif
	if(err)
	{
		if(acklen == 0)
		{
			finished = 0x00;
			value[0] = ack[0];
			#ifdef DEBUG_MDB
			Trace("@API MDB Coin ExpanPayoutValuePoll Ok:Buzzy value = %d\r\n",*value);
			#endif
		}
		else
		{
			#ifdef DEBUG_MDB
			Trace("@API MDB Coin ExpanPayoutValuePoll Ok:Finished\r\n");
			#endif
			finished = 0x01;
		}
	}
	else
	{
		#ifdef DEBUG_MDB
		Trace("@API MDB Coin ExpanPayoutValuePoll:ERR!\r\n");
		#endif
		finished = 0x02;
	}
	return finished;
}
/*********************************************************************************************************
** @DRIVER Function name:   API_MDB_CoinType_CoinDevice
** @DRIVER Input para:     	None
** @DRIVER Output para:     MainCode;SubCode
** @DRIVER retrun para:     0x00:err;0x01:ok
*********************************************************************************************************/
unsigned char API_MDB_ExpanDiagStatus_CoinDevice(unsigned char *MainCode,unsigned char *SubCode)
{
	unsigned char err,cmd[5],ack[36],acklen;
	cmd[0] = EXPANSION_COINACCEPTORCHANGER;
	cmd[1] = 0x05;
	err = MDBConversation(cmd,2,ack,&acklen);
	#ifdef DEBUG_MDB
	if(err)
		Trace("@API MDB Coin ExpanDiagStatus OK DATA(%02X,%02X)\r\n",ack[0],ack[1]);
	else
		Trace("@API MDB Coin ExpanDiagStatus:ERR!\r\n");
	#endif
	if(err)
	{
		*MainCode = ack[0];
		*SubCode = ack[1];
	}
	return err;
}
/**************************************End Of File*******************************************************/
