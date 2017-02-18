/****************************************Copyright (c)*************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-----------------------------------------------------------------
 ** @Filename:			MDB.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0
 *****************************************************************************************************************/
#include "..\config.h"
#define  UART_IIR_INTID_RLS		((unsigned int)(3 << 1)) 	/* Interrupt identification: Receive line status*/
#define  UART_IIR_INTID_RDA		((unsigned int)(2 << 1)) 	/* Interrupt identification: Receive data available*/
#define  UART_IIR_INTID_CTI		((unsigned int)(6 << 1)) 	/* Interrupt identification: Character time-out indicator*/
#define  UART_IIR_INTID_MASK	((unsigned int)(7 << 1))	/* Interrupt identification: Interrupt ID mask */
#define  UART_LSR_OE			((unsigned int)(1 << 1))	/* Line status register: Overrun error*/
#define  UART_LSR_PE			((unsigned int)(1 << 2))	/* Line status register: Parity error*/
#define  UART_LSR_FE			((unsigned int)(1 << 3))	/* Line status register: Framing error*/
#define  UART_LSR_BI			((unsigned int)(1 << 4))	/* Line status register: Break interrupt*/
#define  UART_LSR_THRE			((unsigned int)(1 << 5))	/* Line status register: Transmit holding register empty*/
#define  UART_LSR_TEMT			((unsigned int)(1 << 6))	/* Line status register: Transmitter empty*/
#define  UART_LSR_RXFE			((unsigned int)(1 << 7))	/* Error in RX FIFO*/
#define  UART_LCR_WLEN8     	((unsigned int)(3 << 0)) 	/* UART 8 bit data mode */
#define  UART_LCR_PARITY_EN		((unsigned int)(1 << 3))	/* UART Parity Enable */
#define  UART_LCR_PARITY_F_1	((unsigned int)(2 << 4))	/* UART force 1 stick parity */
#define  UART_LCR_PARITY_F_0	((unsigned int)(3 << 4))	/* UART force 0 stick parity */	
#define  MDB_START				0xA1						/* Start mdb bus receving */
#define  MDB_RECING				0xA2						/* Receving data */
#define  MDB_RECVED				0xA3						/* Recevied data and check sum ok */
#define  MDB_BUFF_ERR			0xA4						/* Recevied data out of buffer range */
#define  MDB_CHKS_ERR			0xA5						/* Recevied data check sum error */
#define  MDB_ADDR_MODE			0x01						/* Send addrres to mdb bus */
#define  MDB_DATA_MODE			0x00						/* Send data to mdb bus */	
volatile unsigned char MdbBuffer[MDB_MAX_MSG_LEN];
volatile unsigned char MdbBufRIndex = 0x00;
volatile unsigned char MdbBufFIndex = 0x00;
volatile unsigned char MdbStatus = MDB_START;
static void MdbBufferClr(void);
static void MdbPutCh(unsigned char data,unsigned char pmode);
static void MdbSetParityMode(unsigned char mode);
/*******************************************************************************************************************
** @DRIVER Function name:   API_MDB_UartInit
** @DRIVER Input para:     	None
** @DRIVER retrun para:     None
*******************************************************************************************************************/
void API_MDB_UartInit(void)
{
	MdbBufferClr();
	LPC_SC->PCONP |= (1ul << 24);
    LPC_UART2->LCR  = 0x80;	//bit7 of LCR must be set hight for enable access to divisor latches
    LPC_UART2->DLM  = 0;
    LPC_UART2->DLL  = 125;
	LPC_UART2->FDR  = 0x41;
    LPC_UART2->LCR  = 0x03;	//Disable access to divisor and set UART2 mode:9600/8/ODD/1
    LPC_UART2->FCR  = 0x07;	//Enable FIFO and set FIFO trigger level 0(one byte)
    LPC_UART2->IER  = 0x05;	//Eanble RDA and RXD interrupts,THRE interrupt is be disable
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_SetPriority(UART2_IRQn,5);
}
/*******************************************************************************************************************
** @IRQ Function name:		UART2_IRQHandler
** @IRQ Input para:     	None
** @IRQ retrun para:		None
*******************************************************************************************************************/
void UART2_IRQHandler(void) 
{
	unsigned int IntID,Lsr;
	unsigned char Recv;
	static unsigned char checksum;
	IntID = LPC_UART2->IIR & UART_IIR_INTID_MASK;
	if(IntID == UART_IIR_INTID_RLS)
	{
		Lsr = (LPC_UART2->LSR & UART_LSR_PE);
		Recv = LPC_UART2->RBR;
		if(Lsr)
		{
			if(MdbStatus == MDB_START)
			{
				MdbBuffer[MdbBufRIndex++] = Recv;
				//MdbAckLen = MdbBufRIndex;
				MdbStatus = MDB_RECVED;
				MdbPutCh(MDB_ACK,MDB_DATA_MODE);
			}
			else if(MdbStatus == MDB_RECING)
			{
				if(MdbBufRIndex < MDB_MAX_MSG_LEN)
				{
					MdbBuffer[MdbBufRIndex++] = Recv;
					if(Recv == checksum)
					{
						MdbStatus = MDB_RECVED;
						//MdbAckLen = MdbBufRIndex;
						MdbPutCh(MDB_ACK,MDB_DATA_MODE);
					}
					else
					{
						MdbStatus = MDB_CHKS_ERR;
						MdbPutCh(MDB_NAK,MDB_DATA_MODE);
					}
				}
				else
				{
					MdbStatus = MDB_BUFF_ERR;
					MdbPutCh(MDB_NAK,MDB_DATA_MODE);
				}
			}
		}
	} 
	if((IntID == UART_IIR_INTID_RDA) || (IntID == UART_IIR_INTID_CTI))
	{
		if(MdbBufRIndex < MDB_MAX_MSG_LEN)
		{
			Recv = LPC_UART2->RBR;
			MdbBuffer[MdbBufRIndex++] = Recv;
			if(MdbStatus == MDB_START)
				checksum = 0;
			MdbStatus = MDB_RECING;
			checksum += Recv;
		}
		else
		{
		    Recv = LPC_UART2->RBR;
			MdbStatus = MDB_BUFF_ERR;
			MdbPutCh(MDB_NAK,MDB_DATA_MODE);
		}
	}
}
/*******************************************************************************************************************
** @API Function name:   API_MDB_ResetBus
** @API Input para:      None
** @API retrun para:     None
*******************************************************************************************************************/
void API_MDB_ResetBus(void)
{
	LPC_PINCON->PINSEL0 &= ~(1ul << 20);
	LPC_GPIO0->FIODIR |= 1ul << 10;
	LPC_GPIO0->FIOCLR |= 1ul << 10;
	vTaskDelay(60);
	LPC_PINCON->PINSEL0 |= 1ul << 20;
	/* 
	The VMC may reset all peripherals by pulling the transmit line "Active" for a minimum of
	100 mS. This informs all peripherals to abort any activity and return to its power-on
	reset state.It is recommended that the VMC re-initialize each peripheral after this type
	of reset. */
}
/*******************************************************************************************************************
** @DRIVER Function name:   MDBConversation
** @DRIVER Input para:     	cmd,cmd_len,ack,ack-len
** @DRIVER Retrun para:     0x00:err;0x01:Ok
** @DRIVER Note:			After sending mdb msg,force Uart2's parity LOW
*******************************************************************************************************************/
unsigned char MDBConversation(unsigned char *cmd,unsigned char cmd_len,unsigned char *ack,unsigned char *ack_len)
{
	unsigned char i,checksum,err,MdbAckLen;
	extern volatile SYSTEMTIMER SystemTimer;
	err = 0x00;
	MdbBufferClr();
	MdbAckLen = 0x00;
	MdbStatus = MDB_START;
	checksum = cmd[0];
	if((cmd_len == 0x00) || (cmd_len > 36))
		return err;
	else
	{
		MdbPutCh(cmd[0],MDB_ADDR_MODE);
		checksum = cmd[0];
		for(i=1;i<cmd_len;i++)
		{
			MdbPutCh(cmd[i],MDB_DATA_MODE);
			checksum += cmd[i];
		}
		MdbPutCh(checksum,MDB_DATA_MODE);
	}
	SetSystemTimerValue((unsigned int *)&SystemTimer.MDBConversationTimer,MDB_NON_RESPONSE_TIME/10);//200ms
	while(SystemTimer.MDBConversationTimer)
	{
		if(MdbStatus == MDB_RECVED)
		{
			err = 0x01;
			if(MdbBufRIndex > 0)
				MdbAckLen = MdbBufRIndex - 1;
			else
				MdbAckLen = 0x00;
			for(i=0;i<MdbAckLen;i++)
			{
				*ack++ = MdbBuffer[i];
			}
			*ack_len = MdbAckLen;
			break;
		}
		vTaskDelay(3);
	}
	return err;
}
/*******************************************************************************************************************
** @DRIVER Function name:   MdbBufferClr
** @DRIVER Input para:     	data
** @DRIVER retrun para:     None
*******************************************************************************************************************/
static void MdbBufferClr(void)
{
	memset((void *)MdbBuffer,0x00,MDB_MAX_MSG_LEN);
	MdbBufRIndex = 0x00;
	MdbBufFIndex = 0x00;
}
/*******************************************************************************************************************
** @DRIVER Function name:   MdbPutCh
** @DRIVER Input para:     	data
** @DRIVER retrun para:     None
*******************************************************************************************************************/
static void MdbPutCh(unsigned char data,unsigned char pmode)
{
	switch(pmode) 
	{
		case MDB_DATA_MODE:
			MdbSetParityMode(MDB_DATA_MODE);
			LPC_UART2->THR = data;
			while(!(LPC_UART2->LSR & UART_LSR_THRE)); 
			break;
		case MDB_ADDR_MODE:
			MdbSetParityMode(MDB_ADDR_MODE);
			LPC_UART2->THR = data;
			while(!(LPC_UART2->LSR & UART_LSR_THRE)); 
			break;
		default:
			break;
	}
	while(!(LPC_UART2->LSR & UART_LSR_TEMT));
}
/*******************************************************************************************************************
** @DRIVER Function name:   MdbSetParityMode
** @DRIVER Input para:     	mode:ODD,EVEN,Force Hight,force Low
** @DRIVER retrun para:     None
*******************************************************************************************************************/
static void MdbSetParityMode(unsigned char mode)
{
	switch(mode)
	{
		case MDB_ADDR_MODE:
			LPC_UART2->LCR = UART_LCR_PARITY_F_1 | UART_LCR_PARITY_EN | UART_LCR_WLEN8;
			break;
		case MDB_DATA_MODE:
			LPC_UART2->LCR = UART_LCR_PARITY_F_0 | UART_LCR_PARITY_EN | UART_LCR_WLEN8;
			break;
		default:
			LPC_UART2->LCR = UART_LCR_WLEN8;
			break;
	}
}
/**********************************MDB Protocol brief***************************************************************
1.BYTE FORMAT:9600 NRZ/1 start bit/8 data bits/1 mode bit/1 stop bit
	Mode bit(Master to peripheral):The mode bit is set to indicate an ADDRESS byte, and not set in DATA byte
	Mode bit(Peripheral to master):The mode bit must be set on the last byte in the msg
2.MSG FORMAT:
	2.1 Master-to-Peripheral:
		--MSG is define as (ADDRESS* + Optional data + CHK byte)/ACK/NAK/RET;MSG max lenght is 36.
		--ADDRRES:The upper five bits (ADDRESSING)+ The lower three bits(CMD)
	2.2 Peripheral-to-Master:
		--MSG is define as (data + CHK byte)/ACK/NAK;Max lenght is 36.
	2.3 Response Codes:
		--ACK:0x00
		--NAK:0xAA
		--RET:0xFF
		--A CHK byte is not required when a peripheral responds with NAK or ACK byte,but must set mode bit.
		__When master responds peripheral's msg with ACK/RET,the ACK/RET's mode bit is not  low.
*******************************************************************************************************************/
/**************************************End Of File*****************************************************************/
