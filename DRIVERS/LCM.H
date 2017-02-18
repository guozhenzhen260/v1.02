/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			LCM.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _LCM_H
#define _LCM_H

void LCMInit(void);
void LCMClearScreen(void);
void LCMClearArea(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void LCMDrawline(unsigned char x,unsigned char y);
void LCMSetXY(unsigned char x,unsigned char y);
unsigned char LCMGetX(void);
unsigned char LCMGetY(void);
void LCMPrintfOneAscIICh16x8(unsigned char x,unsigned char y,char *str,unsigned char Inv);
void LCMPrintfOneAscIICh32x16(unsigned char x,unsigned char y,char *str,unsigned char Inv);
void LCMPrintfOneChinese16x16(unsigned char x,unsigned char y,char *str,unsigned char Inv);
void LCMPrintfBmp(unsigned char x,unsigned char y,unsigned char with,unsigned char hight,char *str);
void LCMBacklightControl(unsigned char cmd);

#endif
/**************************************End Of File*******************************************************/
