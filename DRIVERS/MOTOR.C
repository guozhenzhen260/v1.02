/****************************************Copyright (c)************************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-----------------------------------------------------------------------------------------
 ** @Filename:			MOTOR.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 ****************************************************************************************************************************************/
#include "..\config.h"
#define DebugGoodChannel
#define	MOTORCTRL_ROW0	1ul << 24	//P1.24
#define	MOTORCTRL_ROW1	1ul << 23	//P1.23
#define	MOTORCTRL_ROW2	1ul << 22	//P1.22
#define	MOTORCTRL_ROW3	1ul << 21	//P1.21
#define	MOTORCTRL_ROW4	1ul << 20	//P1.20
#define	MOTORCTRL_ROW5	1ul << 19	//P1.19
#define	MOTORCTRL_COL0	1ul << 18	//P0.18
#define	MOTORCTRL_COL1	1ul << 19	//P0.19
#define	MOTORCTRL_COL2	1ul << 20	//P0.20
#define	MOTORCTRL_COL3	1ul << 21	//P0.21
#define	MOTORCTRL_COL4	1ul << 16	//P0.16
#define	MOTORCTRL_COL5	1ul << 15	//P0.15
#define	MOTORCTRL_COL6	1ul << 17	//P0.17
#define	MOTORCTRL_COL7	1ul << 26	//P1.26
#define	MOTORCTRL_COL8	1ul << 25	//P1.25
#define MOTORCTRL_PWR	1ul << 4	//P2.4
#define MOTORCTRL_BAK	1ul << 1	//P0.1
#define MOTORCTRL_SEN	1ul << 18	//P1.18
static void MotorPowerCtrl(unsigned char ctrl);
static void MotorBrakeCtrl(unsigned char ctrl);
static void MotorRowCtrl(unsigned char ctrl);
static void MotorColCtrl(unsigned char ctrl);
static unsigned char MotorSensorRead(void);
extern volatile SYSTEMTIMER SystemTimer;
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MOTORInit
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
*****************************************************************************************************************************************/
void MOTORInit(void)
{
	MotorPowerCtrl(0x00);
	MotorBrakeCtrl(0x00);
	MotorRowCtrl(0xFF);
	MotorColCtrl(0xFF);
	return;
}
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MotorVending
** @DRIVER Input para:     		layer;nmber
** @DRIVER retrun para:     	0x00:err;0x01:Ok;other:undefine
*****************************************************************************************************************************************/
unsigned char MotorVending(char layer,char channel)
{
	unsigned char err = 0x00,Start = 0x00,Row,Col;
	unsigned int LogTime;
	Row = (layer - 0x41) & 0x0F;	//层转为行
	Col = (channel - 0x31) & 0x0F;	//通道转为列
	if((Row >= 0x06) || (Col >= 0x09))
	{
		#ifdef DebugGoodChannel
		Trace("MotorVending():1.Channel Data ERR(layer = %C,channel = %c,Row = %d,Col = %d)!\r\n",layer,channel,Row,Col);
		#endif
		return 0x00;
	}
	#ifdef DebugGoodChannel
	Trace("MotorVending():1.Data OK (layer = %C,channel = %c,Row = %d,Row = %d)!\r\n",layer,channel,Row,Col);
	#endif
	MotorPowerCtrl(1);
	MotorRowCtrl(Row);
	MotorColCtrl(Col);
	vTaskDelay(2);
	if(MotorSensorRead() == 0x01)
	{
		#ifdef DebugGoodChannel
		Trace("MotorVending():1.Start--In Arrived Position\r\n");
		#endif
		SetSystemTimerValue((unsigned int *)&SystemTimer.GoodsChannelTimer,200);
		while(SystemTimer.GoodsChannelTimer)
		{
			if(MotorSensorRead() == 0x00)
			{
				Start = 0x01;
				#ifdef DebugGoodChannel
				Trace("MotorVending():1.Move--leave Arrived position OK(Time = %d)\r\n",200-SystemTimer.GoodsChannelTimer);
				#endif
				break;
			}
			vTaskDelay(2);
		}
		vTaskDelay(2);
		if(Start == 0x01)
		{
			SetSystemTimerValue((unsigned int *)&SystemTimer.GoodsChannelTimer,400);
			while(SystemTimer.GoodsChannelTimer)
			{
				if(MotorSensorRead() == 0x01)
				{
					err = 0x01;
					vTaskDelay(2);
					MotorRowCtrl(0xFF);
					MotorColCtrl(0xFF);
					MotorPowerCtrl(0);
					LogTime = (400 - SystemTimer.GoodsChannelTimer);
					#ifdef DebugGoodChannel
					Trace("MotorVending():1.Stop--Drive Ok(Time = %d)...\r\n",LogTime);
					#endif
					break;
				}
				vTaskDelay(2);				
			}
			MotorRowCtrl(0xFF);
			MotorColCtrl(0xFF);
			MotorPowerCtrl(0);
			#ifdef DebugGoodChannel
			if(err == 0x00)
				Trace("MotorVending():1.Stop--Leaved,Go to Arrived position err...\r\n");				
			#endif
		}
		else
		{
			MotorRowCtrl(0xFF);
			MotorColCtrl(0xFF);
			MotorPowerCtrl(1);
			#ifdef DebugGoodChannel
			Trace("MotorVending():1.Stop--Arrived to leaved ERR!!!(Sensor or Motor ERR or Motor not exsit)...\r\n");
			#endif
		}			
	}
	else
	{
		#ifdef DebugGoodChannel
		Trace("MotorVending():2.Start--Not In Arrived Positon\r\n");
		#endif
		SetSystemTimerValue((unsigned int *)&SystemTimer.GoodsChannelTimer,400);
		while(SystemTimer.GoodsChannelTimer)
		{
			if(MotorSensorRead() == 0x01)
			{
				err = 0x01;
				vTaskDelay(2);
				MotorRowCtrl(0xFF);
				MotorColCtrl(0xFF);
				MotorPowerCtrl(0);
				LogTime = (400 - SystemTimer.GoodsChannelTimer);
				#ifdef DebugGoodChannel
				Trace("MotorVending():2.Stop--Move to Arrived position Ok(Time = %d)...\r\n",LogTime);
				#endif
				break;
			}
			vTaskDelay(2);			
		}
		MotorRowCtrl(0xFF);
		MotorColCtrl(0xFF);
		MotorPowerCtrl(0);
		if(err == 0x01)
		{
			if(LogTime > 150)
			{
				MotorRowCtrl(0xFF);
				MotorColCtrl(0xFF);
				MotorPowerCtrl(0);
				#ifdef DebugGoodChannel
				Trace("MotorVending():2.Direct Drive Ok and Logtime = %d...\r\n",LogTime);
				#endif
			}
			else
			{
				#ifdef DebugGoodChannel
				Trace("MotorVending():2.1.Direct Drive Ok But DriveTime is < LogTime!!(LogTime = %d)...\r\n",LogTime);
				Trace("MotorVending():2.1.Restart--In Arrived Position\r\n");
				#endif
				MotorPowerCtrl(0);
				Start = 0x00;
				err = 0x00;
				MotorPowerCtrl(1);
				MotorRowCtrl(Row);
				MotorColCtrl(Col);
				vTaskDelay(2);
				SetSystemTimerValue((unsigned int *)&SystemTimer.GoodsChannelTimer,200);
				while(SystemTimer.GoodsChannelTimer)
				{
					if(MotorSensorRead() == 0x00)
					{
						Start = 0x01;
						#ifdef DebugGoodChannel
						Trace("MotorVending():2.1.Move--Move to leaved OK(Time = %d)\r\n",200-SystemTimer.GoodsChannelTimer);
						#endif
						break;
					}
					vTaskDelay(2);
				}
				vTaskDelay(2);
				if(Start == 0x01)
				{
					SetSystemTimerValue((unsigned int *)&SystemTimer.GoodsChannelTimer,400);
					while(SystemTimer.GoodsChannelTimer)
					{
						if(MotorSensorRead() == 0x01)
						{
							err = 0x01;
							MotorRowCtrl(0xFF);
							MotorColCtrl(0xFF);
							MotorPowerCtrl(0);
							LogTime = (400 - SystemTimer.GoodsChannelTimer);
							#ifdef DebugGoodChannel
							Trace("MotorVending():2.1.Stop--Drive Ok(Time = %d)...\r\n",LogTime);
							#endif
							break;
						}
						vTaskDelay(2);						
					}
					MotorRowCtrl(0xFF);
					MotorColCtrl(0xFF);
					MotorPowerCtrl(0);
					#ifdef DebugGoodChannel
					if(err == 0x00)
					{			
						Trace("MotorVending():2.1.Stop--Leaved to Arrived err...\r\n");	
					}
					#endif		
				}
				else
				{
					MotorRowCtrl(0xFF);
					MotorColCtrl(0xFF);
					MotorPowerCtrl(0);
					#ifdef DebugGoodChannel
					Trace("3.1--PutCmdToGoodsChannel():Stop--Arrived to leaved ERR!!!(Sensor ERR)...\r\n");
					#endif
				}
			}
		}
		else
		{
			#ifdef DebugGoodChannel
			Trace("2.1--PutCmdToGoodsChannel():Direct Drive err(Arrived ERR for Hardware ERR or GC is Not exsit)\r\n");
			#endif
		}
	}
	return err;
}
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MotorPowerCtrl
** @DRIVER Input para:     		layer;nmber
** @DRIVER retrun para:     	0x00:err;0x01:Ok;other:undefine
*****************************************************************************************************************************************/
static void MotorPowerCtrl(unsigned char ctrl)
{
	LPC_GPIO2->FIODIR |= MOTORCTRL_PWR;
	if(ctrl)
		LPC_GPIO2->FIOSET |= MOTORCTRL_PWR;
	else
		LPC_GPIO2->FIOCLR |= MOTORCTRL_PWR;
}
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MotorBrakeCtrl
** @DRIVER Input para:     		layer;nmber
** @DRIVER retrun para:     	0x00:err;0x01:Ok;other:undefine
*****************************************************************************************************************************************/
static void MotorBrakeCtrl(unsigned char ctrl)
{
	LPC_GPIO0->FIODIR |= MOTORCTRL_BAK;
	if(ctrl)
	{
		MotorPowerCtrl(0x00);
		vTaskDelay(10);
		LPC_GPIO0->FIOSET |= MOTORCTRL_BAK;
		vTaskDelay(50);
	}
	else
		LPC_GPIO0->FIOCLR |= MOTORCTRL_BAK;
}
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MotorRowCtrl
** @DRIVER Input para:     		layer;nmber
** @DRIVER retrun para:     	0x00:err;0x01:Ok;other:undefine
*****************************************************************************************************************************************/
static void MotorRowCtrl(unsigned char ctrl)
{
	LPC_GPIO1->FIODIR |= (MOTORCTRL_ROW0 | MOTORCTRL_ROW1 | MOTORCTRL_ROW2 | MOTORCTRL_ROW3 | MOTORCTRL_ROW4 | MOTORCTRL_ROW5);
	switch(ctrl)
	{
		case 0x00:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW1 | MOTORCTRL_ROW2 | MOTORCTRL_ROW3 | MOTORCTRL_ROW4 | MOTORCTRL_ROW5);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_ROW0;
			break;
		case 0x01:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW0 | MOTORCTRL_ROW2 | MOTORCTRL_ROW3 | MOTORCTRL_ROW4 | MOTORCTRL_ROW5);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_ROW1;
			break;
		case 0x02:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW1 | MOTORCTRL_ROW0 | MOTORCTRL_ROW3 | MOTORCTRL_ROW4 | MOTORCTRL_ROW5);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_ROW2;
			break;
		case 0x03:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW1 | MOTORCTRL_ROW2 | MOTORCTRL_ROW0 | MOTORCTRL_ROW4 | MOTORCTRL_ROW5);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_ROW3;
			break;
		case 0x04:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW1 | MOTORCTRL_ROW2 | MOTORCTRL_ROW3 | MOTORCTRL_ROW0 | MOTORCTRL_ROW5);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_ROW4;
			break;
		case 0x05:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW1 | MOTORCTRL_ROW2 | MOTORCTRL_ROW3 | MOTORCTRL_ROW4 | MOTORCTRL_ROW0);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_ROW5;
			break;
		default:
			LPC_GPIO1->FIOSET |= (MOTORCTRL_ROW1 | MOTORCTRL_ROW2 | MOTORCTRL_ROW3 | MOTORCTRL_ROW4 | MOTORCTRL_ROW5);
			LPC_GPIO1->FIOSET |= MOTORCTRL_ROW0;
			break;
	}
}
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MotorColCtrl
** @DRIVER Input para:     		layer;nmber
** @DRIVER retrun para:     	0x00:err;0x01:Ok;other:undefine
*****************************************************************************************************************************************/
static void MotorColCtrl(unsigned char ctrl)
{
	LPC_GPIO0->FIODIR |= (MOTORCTRL_COL0 | MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
	LPC_GPIO0->FIODIR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
	LPC_GPIO1->FIODIR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
	switch(ctrl)
	{
		case 0x00:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL0;
			break;
		case 0x01:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL0 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL1;
			break;
		case 0x02:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL1 | MOTORCTRL_COL0 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL2;
			break;
		case 0x03:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL0);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL3;
			break;
		case 0x04:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL0 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL4;
			break;
		case 0x05:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL0 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL5;
			break;
		case 0x06:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL0);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			LPC_GPIO0->FIOSET |= MOTORCTRL_COL6;
			break;
		case 0x07:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL0 | MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_COL8;
			LPC_GPIO1->FIOSET |= MOTORCTRL_COL7;
			break;
		case 0x08:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL0 | MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= MOTORCTRL_COL7;
			LPC_GPIO1->FIOSET |= MOTORCTRL_COL8;
			break;
		default:
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL0 | MOTORCTRL_COL1 | MOTORCTRL_COL2 | MOTORCTRL_COL3);
			LPC_GPIO0->FIOCLR |= (MOTORCTRL_COL4 | MOTORCTRL_COL5 | MOTORCTRL_COL6);
			LPC_GPIO1->FIOCLR |= (MOTORCTRL_COL7 | MOTORCTRL_COL8);
			break;
	}
}
/*****************************************************************************************************************************************
** @DRIVER Function name:     	MotorSensorRead
** @DRIVER Input para:     		layer;nmber
** @DRIVER retrun para:     	0x00:err;0x01:Ok;other:undefine
*****************************************************************************************************************************************/
static unsigned char MotorSensorRead(void)
{
	LPC_GPIO1->FIODIR &= (~MOTORCTRL_SEN);
	return(((LPC_GPIO1->FIOPIN) >> 18) & 0x01);
}
/**************************************End Of File***************************************************************************************/
