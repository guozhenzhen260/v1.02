/****************************************Copyright (c)**************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info-------------------------------------------------------------------------------
 ** @Filename:			I2C.H
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 ******************************************************************************************************************************/
#ifndef __I2C_H
#define __I2C_H

void I2C0Init(void);
unsigned char I2C0RdNByte(unsigned char sla,unsigned int  suba_type,unsigned int suba,unsigned char *s,unsigned int num);
unsigned char I2C0WrNByte(unsigned char sla,unsigned char suba_type,unsigned int suba,unsigned char *s,unsigned int num);
void I2C0_IRQHandler(void);

#endif
/**************************************End Of File******************************************************************************/
