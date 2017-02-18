/****************************************Copyright (c)**************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info------------------------------------------------------
 ** @Filename:			TIMER.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 ******************************************************************************************************/
#include "..\config.h"
volatile unsigned int TimerForAPP[8];//0~7Í¨µÀ
volatile SYSTEMTIMER SystemTimer;
/*******************************************************************************************************
** @DRIVER Function name:     	InitTimer
** @DRIVER Input para:     		Numb:timer numb;Interval:time set
** @DRIVER retrun para:     	None
** @DRIVER note:				Time(second) = (Interval*(PR+1))/FPCLK,FPCLK = 24000000,PR = 0;
*******************************************************************************************************/
void InitTimer(unsigned char Numb,unsigned int Interval)
{
	switch(Numb)
	{
		case 0:	
			LPC_TIM0->TCR	= 0x02;
			LPC_TIM0->IR	= 1;
			LPC_TIM0->CTCR	= 0;
			LPC_TIM0->TC	= 0;
			LPC_TIM0->PR	= 0;
			LPC_TIM0->MR0	= Interval;
			LPC_TIM0->MCR	= 0x03;
			NVIC_EnableIRQ(TIMER0_IRQn);
			NVIC_SetPriority(TIMER0_IRQn,7);
			LPC_TIM0->TCR	= 0x00;
			break;
		case 1:
			LPC_TIM1->TCR	= 0x02;
			LPC_TIM1->IR	= 1;
			LPC_TIM1->CTCR	= 0;
			LPC_TIM1->TC	= 0;
			LPC_TIM1->PR	= 0;
			LPC_TIM1->MR0	= Interval;
			LPC_TIM1->MCR	= 0x03;
			NVIC_EnableIRQ(TIMER1_IRQn);
			NVIC_SetPriority(TIMER1_IRQn,6);
			LPC_TIM1->TCR	= 0x00;
			break;
		case 2:
			LPC_SC->PCONP |= 1ul << 22;
			LPC_TIM2->TCR	= 0x02;
			LPC_TIM2->IR	= 1;
			LPC_TIM2->CTCR	= 0;
			LPC_TIM2->TC	= 0;
			LPC_TIM2->PR	= 0;
			LPC_TIM2->MR0	= Interval;
			LPC_TIM2->MCR	= 0x03;
			NVIC_EnableIRQ(TIMER2_IRQn);
			NVIC_SetPriority(TIMER2_IRQn,2);
			LPC_TIM2->TCR	= 0x01;
			break;
		case 3:
			LPC_SC->PCONP |= 1ul << 23;
			LPC_TIM3->TCR	= 0x02;
			LPC_TIM3->IR	= 1;
			LPC_TIM3->CTCR	= 0;
			LPC_TIM3->TC	= 0;
			LPC_TIM3->PR	= 0;
			LPC_TIM3->MR0	= Interval;
			LPC_TIM3->MCR	= 0x03;
			NVIC_EnableIRQ(TIMER3_IRQn);
			NVIC_SetPriority(TIMER3_IRQn,4);
			LPC_TIM3->TCR	= 0x01;
			break;
		default:
			break;
	}
}
/*******************************************************************************************************
** @API Function name:     	StartAppTimerValue
** @API Input para:     	target;value
** @API retrun para:     	None
********************************************************************************************************/
void SetSystemTimerValue(unsigned int *target,unsigned int value)
{
	*target = value;
}
/*******************************************************************************************************
** @DRIVER Function name:     	TIMER0_IRQHandler
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
********************************************************************************************************/
void TIMER0_IRQHandler(void)
{
	unsigned char i,Flag;
	LPC_TIM0->IR = 1;
	Flag = 0x00;
	for(i=0;i<8;i++)
	{
		if(TimerForAPP[i])
		{
			Flag |= (0x01 << i);
			TimerForAPP[i] --;
		}
	}
	if(Flag == 0x00)
		TimerCtrl(0,0);
}
/*******************************************************************************************************
** @IRQ Function name:     	TIMER1_IRQHandler
** @IRQ Input para:     	None
** @IRQ retrun para:     	None
********************************************************************************************************/
void TIMER1_IRQHandler(void)
{
	static unsigned char n = 0x00,key[6];
	static BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	extern xQueueHandle KeyQueue;
	LPC_TIM1->IR = 1;
	if(n == 5)
	{
		Buzzer();
		xQueueSendFromISR(KeyQueue,&key[0],&xHigherPriorityTaskWoken);
		n++;
	}
	else if(n > 5)
	{
		if(n < 10)
			n++;
		else
		{
			if((LPC_GPIO2->FIOPIN >> 11) & 0x01)
			{
				n= 0x00;
				TimerCtrl(1,0);
				KeyboardCtrl(0x01);
			}
		}
	}
	else
	{
		key[n] = KeyBoardScan();
		if(key[n] == 0xFF)
		{
			n = 0x00;
			TimerCtrl(1,0);
			KeyboardCtrl(0x01);		
		}
		else
			n++;
	}
}
/********************************************************************************************************
** @DRIVER Function name:     	TIMER2_IRQHandler
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
********************************************************************************************************/
void TIMER2_IRQHandler(void)
{
	static unsigned int BuzerTimer = 0x00;
	LPC_TIM2->IR = 0x01;
	if(BuzerTimer >= 2)
	{
		LPC_GPIO0->FIODIR |= 1ul << 24;
		LPC_GPIO0->FIOCLR |= 1ul << 24;
		BuzerTimer = 0x00;
		TimerCtrl(2,0);
	}
	else
	{
		BuzerTimer++;
	}
}
/*******************************************************************************************************
** @DRIVER Function name:     	TIMER3_IRQHandler
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
********************************************************************************************************/
void TIMER3_IRQHandler(void)
{
	static unsigned char PreMaintKey,CurMaintKey,Key;
	static unsigned int Delay;
	static BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	extern xQueueHandle KeyQueue;
	LPC_TIM3->IR = 1;
	if(SystemTimer.MDBConversationTimer)
		SystemTimer.MDBConversationTimer--;
	if(SystemTimer.GoodsChannelTimer)
		SystemTimer.GoodsChannelTimer--;
	if(Delay)
		Delay--;
	else
	{
		CurMaintKey = ((LPC_GPIO2->FIOPIN >> 10) & 0x01);
		if((CurMaintKey == 0x00) && (PreMaintKey == 0x01))
		{
			Buzzer();
			Key = 'M';
			xQueueSendFromISR(KeyQueue,&Key,&xHigherPriorityTaskWoken);
			Delay = 400;
		}
		else
			PreMaintKey = CurMaintKey;
	}
}
/*******************************************************************************************************
** @DRIVER Function name:     	TimerCtrl
** @DRIVER Input para:     		Numb:timer numb;ctrl:disable or enable
** @DRIVER retrun para:     	None
********************************************************************************************************/
void TimerCtrl(unsigned char Numb,unsigned char ctrl)
{
	switch(Numb)
	{
		case 0:	if(ctrl)
				{
					LPC_TIM0->TCR = 0x02;
					LPC_TIM0->TCR = 0x01;
				}
				else
					LPC_TIM0->TCR = 0x00;
				break;
		case 1:	if(ctrl)
				{
					LPC_TIM1->TCR = 0x02;
					LPC_TIM1->TCR = 0x01;
				}
				else
					LPC_TIM1->TCR = 0x00;
				break;
		case 2:	if(ctrl)
				{
					LPC_TIM2->TCR = 0x02;
					LPC_TIM2->TCR = 0x01;
				}
				else
					LPC_TIM2->TCR = 0x00;
				break;	
		case 3:	if(ctrl)
				{
					LPC_TIM3->TCR = 0x02;
					LPC_TIM3->TCR = 0x01;
				}
				else
					LPC_TIM3->TCR = 0x00;
				break;	
		default:
			break;
	}
}
/**************************************End Of File*******************************************************/
