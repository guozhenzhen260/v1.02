/****************************************Copyright (c)**************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-------------------------------------------------------------------------------
 ** @Filename:			MDB.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 ******************************************************************************************************************************/
#ifndef _MDB_H
#define _MDB_H

#define 	MDB_ACK				0x00	/* ACK */
#define 	MDB_NAK				0xFF	/* NAK */
#define 	MDB_RET				0xAA	/* RET */
#define		MDB_MAX_MSG_LEN		0x24	/* Max mdb Msg lenght */

void UART2_IRQHandler(void);
unsigned char MDBConversation(unsigned char *cmd,unsigned char cmd_len,unsigned char *ack,unsigned char *ack_len);

#endif
/**************************************End Of File******************************************************************************/
