/****************************************Copyright (c)********************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-------------------------------------------------------------
 ** @Filename:			Key.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 ************************************************************************************************************/
#include "..\config.h"
#define		KROW0	(1ul <<  8)	//P1.8
#define		KROW1	(1ul <<  9)	//P1.9
#define		KROW2	(1ul << 10)	//P1.10
#define		KROW3	(1ul << 14)	//P1.14
#define		KROW4	(1ul << 15)	//P1.15
#define		KROW5	(1ul << 16)	//P1.16
#define 	KCOL0	(1ul <<  0)	//P1.0
#define 	KCOL1	(1ul <<  1)	//P1.1
#define 	KCOL2	(1ul <<  4)	//P1.4
#define 	KINT	(1ul << 11) //P2.11
#define		GetCol0Value()	((LPC_GPIO1->FIOPIN >> 0x00) & 0x01)
#define		GetCol1Value()	((LPC_GPIO1->FIOPIN >> 0x01) & 0x01)
#define		GetCol2Value()	((LPC_GPIO1->FIOPIN >> 0x04) & 0x01)
#define		GetRow0Value()	((LPC_GPIO1->FIOPIN >> 0x08) & 0x01)
#define		GetRow1Value()	((LPC_GPIO1->FIOPIN >> 0x09) & 0x01)
#define		GetRow2Value()	((LPC_GPIO1->FIOPIN >> 0x0A) & 0x01)
#define		GetRow3Value()	((LPC_GPIO1->FIOPIN >> 0x0E) & 0x01)
#define		GetRow4Value()	((LPC_GPIO1->FIOPIN >> 0x0F) & 0x01)
#define		GetRow5Value()	((LPC_GPIO1->FIOPIN >> 0x10) & 0x01)
static void ClrAllKeyRow(void);
static void SetAllkeyColInput(void);
static void SetAKeyColOutputLow(unsigned char col);
static unsigned char GetAKeyRowValue(unsigned char row);
static void GPIOChangeDirectionDelay(unsigned char n);
const unsigned char KEYTABLE[3][6] = {'A','B','C','D','E','F','1','2','3','4','>','0','5','6','7','8','<','9'};
// >:取消--向上箭头;<:确认--向下箭头
xQueueHandle KeyQueue;
/**************************************************************************************************************
** @DRIVER Function name:     	KeyboardInit
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
**************************************************************************************************************/
void KeyboardInit(void)
{
	ClrAllKeyRow();
	SetAllkeyColInput();
	InitTimer(1,240000);
	InitTimer(2,360000);
	LPC_GPIO2->FIODIR &= ~(1ul << 10);
	KeyQueue = xQueueCreate(10,sizeof(unsigned char));
	LPC_PINCON->PINSEL4 |= (0x01 << 22);
	LPC_SC->EXTMODE  = 0x02;
	LPC_SC->EXTPOLAR = 0x00;
	LPC_SC->EXTINT = 0x02;
    NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_SetPriority(EINT1_IRQn,8);
}
/**************************************************************************************************************
** @DRIVER Function name:     	KeyBoardCtrl
** @DRIVER Input para:     		ctrl:0-disable;1-enable
** @DRIVER retrun para:     	None
**************************************************************************************************************/
void KeyboardCtrl(unsigned char ctrl)
{
	LPC_GPIO0->FIODIR |= 1ul << 24;
	LPC_GPIO0->FIOCLR |= 1ul << 24;
	if(ctrl)
	{
		ClrAllKeyRow();
		SetAllkeyColInput();
		LPC_PINCON->PINSEL4 |= (0x01 << 22);
		LPC_SC->EXTINT = 0x02;
		NVIC_EnableIRQ(EINT1_IRQn);
	}
	else
	{
		LPC_SC->EXTINT = 0x02;
		NVIC_DisableIRQ(EINT1_IRQn);
	}
}
/**************************************************************************************************************
** @DRIVER Function name:     	ReadKeyValue
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	0x00(no key value) or key value define in KEYTABLE
**************************************************************************************************************/
unsigned char ReadKeyValue(void)
{
	unsigned char Key;
	if(xQueueReceive(KeyQueue,&Key,0) != pdPASS)
		Key = 0x00;
	return Key;
}
/**************************************************************************************************************
** @DRIVER Function name:     	KeyboardBacklightCtrl
** @DRIVER Input para:     		ctrl:0-close;1-open
** @DRIVER retrun para:     	None
**************************************************************************************************************/
void KeyboardBacklightCtrl(unsigned char ctrl)
{
	LPC_GPIO1->FIODIR |= 1ul << 17;
	if(ctrl)
		LPC_GPIO1->FIOSET |= 1ul << 17;
	else
		LPC_GPIO1->FIOCLR |= 1ul <<17;
}
/**************************************************************************************************************
** @IRQ Function name:     	EINT1_IRQHandler
** @IRQ Input para:     	None
** @IRQ retrun para:     	None
** @IRQ note:				EINT1_IRQHandler just start Timer1
**************************************************************************************************************/
void EINT1_IRQHandler(void)
{
	LPC_SC->EXTINT = 0x02;
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4 &= ~(0x01 << 22);
	LPC_GPIO2->FIODIR &= ~KINT;
	TimerCtrl(1,1);
}
/**************************************************************************************************************
** @DRIVER Function name:     	KeyBoardScan
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	keyvalue
** @DRIVER note:				this fuction called by TIMER1_IRQHandler(sample mode)
**************************************************************************************************************/
unsigned char KeyBoardScan(void)
{
	unsigned char i,KIntValue,ColValue,KeyValue;
	KIntValue = ((LPC_GPIO2->FIOPIN >> 11) & 0x01);
	if(KIntValue)
		KeyValue = 0xFF;
	else
	{
		ColValue = (((GetCol2Value() << 0x02) | (GetCol1Value() << 0x01) | (GetCol0Value() << 0x00)) & 0x07);
		LPC_GPIO1->FIODIR &= (~(KROW0 | KROW1 | KROW2 | KROW3 | KROW4 | KROW5));
		GPIOChangeDirectionDelay(40);
		switch(ColValue)
		{
			case 0x03://Col2 PessDown
				SetAKeyColOutputLow(0x02);
				for(i=0;i<6;i++)
				{
					if(GetAKeyRowValue(i))
						KeyValue = 0xFF;
					else
					{
						KeyValue = KEYTABLE[2][i];
						break;
					}
				}	
				break;
			case 0x05://Col1 PessDown
				SetAKeyColOutputLow(0x01);
				for(i=0;i<6;i++)
				{
					if(GetAKeyRowValue(i))
						KeyValue = 0xFF;
					else
					{
						KeyValue = KEYTABLE[1][i];
						break;
					}
				}	
				break;
			case 0x06://Col0 PessDown
				SetAKeyColOutputLow(0x00);
				for(i=0;i<6;i++)
				{
					if(GetAKeyRowValue(i))
						KeyValue = 0xFF;
					else
					{
						KeyValue = KEYTABLE[0][i];
						break;
					}
				}	
				break;
			default:
				KeyValue = 0xFF;
				break;
		}
	}
	ClrAllKeyRow();
	SetAllkeyColInput();
	return KeyValue;
}
/**************************************************************************************************************
** @Driver Function name:     	ClrAllKeyRow
** @Driver Input para:     		None
** @Driver retrun para:     	None
**************************************************************************************************************/
static void ClrAllKeyRow(void)
{
	LPC_GPIO1->FIODIR |= KROW0;
	LPC_GPIO1->FIOCLR |= KROW0;
	LPC_GPIO1->FIODIR |= KROW1;
	LPC_GPIO1->FIOCLR |= KROW1;
	LPC_GPIO1->FIODIR |= KROW2;
	LPC_GPIO1->FIOCLR |= KROW2;
	LPC_GPIO1->FIODIR |= KROW3;
	LPC_GPIO1->FIOCLR |= KROW3;
	LPC_GPIO1->FIODIR |= KROW4;
	LPC_GPIO1->FIOCLR |= KROW4;
	LPC_GPIO1->FIODIR |= KROW5;
	LPC_GPIO1->FIOCLR |= KROW5;
}
/**************************************************************************************************************
** @Driver Function name:     	SetAllkeyColInput
** @Driver Input para:     		None
** @Driver retrun para:     	None
**************************************************************************************************************/
static void SetAllkeyColInput(void)
{
	LPC_GPIO1->FIODIR &= ~KCOL0;
	LPC_GPIO1->FIODIR &= ~KCOL1;
	LPC_GPIO1->FIODIR &= ~KCOL2;
}
/**************************************************************************************************************
** @Driver Function name:     	SetAKeyColOutputLow
** @Driver Input para:     		col:key col
** @Driver retrun para:     	None
**************************************************************************************************************/
static void SetAKeyColOutputLow(unsigned char col)
{
	switch(col)
	{
		case 0x00:
			LPC_GPIO1->FIODIR |= KCOL0;
			LPC_GPIO1->FIOCLR |= KCOL0;
			break;
		case 0x01:
			LPC_GPIO1->FIODIR |= KCOL1;
			LPC_GPIO1->FIOCLR |= KCOL1;
			break;
		case 0x02:
			LPC_GPIO1->FIODIR |= KCOL2;
			LPC_GPIO1->FIOCLR |= KCOL2;
			break;
		default:break;
	}
	GPIOChangeDirectionDelay(10);
}
/**************************************************************************************************************
** @Driver Function name:     	GetAKeyRowValue
** @Driver Input para:     		row:keyrow
** @Driver retrun para:     	value
**************************************************************************************************************/
static unsigned char GetAKeyRowValue(unsigned char row)
{
	unsigned rowvalue;
	switch(row)
	{
		case 0:
			if((LPC_GPIO1->FIOPIN >> 8) & 0x01)
				rowvalue = 0x01;
			else
				rowvalue = 0x00;
			break;
		case 1:
			if((LPC_GPIO1->FIOPIN >> 9) & 0x01)
				rowvalue = 0x01;
			else
				rowvalue = 0x00;
			break;
		case 2:
			if((LPC_GPIO1->FIOPIN >> 10) & 0x01)
				rowvalue = 0x01;
			else
				rowvalue = 0x00;
			break;
		case 3:
			if((LPC_GPIO1->FIOPIN >> 14) & 0x01)
				rowvalue = 0x01;
			else
				rowvalue = 0x00;
			break;
		case 4:
			if((LPC_GPIO1->FIOPIN >> 15) & 0x01)
				rowvalue = 0x01;
			else
				rowvalue = 0x00;
			break;
		case 5:
			if((LPC_GPIO1->FIOPIN >> 16) & 0x01)
				rowvalue = 0x01;
			else
				rowvalue = 0x00;
			break;
		default:
			rowvalue = 0x01;
			break;
	}
	return rowvalue;
}
/**************************************************************************************************************
** @Driver Function name:     	GPIOChangeDirectionDelay
** @Driver Input para:     		n:number __nop;
** @Driver retrun para:     	None
**************************************************************************************************************/
static void GPIOChangeDirectionDelay(unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++)
	{
		__nop();
	}
}
/**************************************End Of File************************************************************/
