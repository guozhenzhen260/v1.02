/****************************************Copyright (c)*********************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------------------------
 ** @Filename:			Display.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 **************************************************************************************************************************/
#include "..\DRIVERS\LCM.h"
#include <stdio.h>
#include <stdarg.h>
#include "API.H"
/**************************************************************************************************************************
** @API Function name:   API_KEY_KeyboardInit
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_Init(void)
{
	LCMInit();
}
/**************************************************************************************************************************
** @API Function name:   API_KEY_KeyboardCtrl
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_ClearScreen(void)
{
	LCMClearScreen();
}
/**************************************************************************************************************************
** @API Function name:   API_KEY_KeyboardCtrl
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_ClearArea(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
	LCMClearArea(x1,y1,x2,y2);
}
/**************************************************************************************************************************
** @API Function name:   API_LCM_DrawLine
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_DrawLine(unsigned char x,unsigned char y)
{
	LCMDrawline(x,y);
}
/**************************************************************************************************************************
** @API Function name:   API_LCM_SetXY
** @API Input para:      x:x×ø±ê;y:y×ø±ê
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_SetXY(unsigned char x,unsigned char y)
{
	LCMSetXY(x,y);
}
/**************************************************************************************************************************
** @API Function name:   API_LCM_GetX
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     X
**************************************************************************************************************************/
extern unsigned char API_LCM_GetX(void)
{
	return LCMGetX();
}
/**************************************************************************************************************************
** @API Function name:   API_LCM_GetY
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     Y
**************************************************************************************************************************/
extern unsigned char API_LCM_GetY(void)
{
	return LCMGetY();
}
/**************************************************************************************************************************
** @API Function name:   API_LCM_LcmPutRMBSymbol
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_PutRMBSymbol(unsigned char x,unsigned char y)
{
	extern const char RMB[];
	LCMPrintfBmp(x,y,24,32,(char *)RMB);
}
/**************************************************************************************************************************
** @API Function name:   API_KEY_ReadKey
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_Printf(unsigned char x,unsigned char y,unsigned char font,unsigned char Inv,char *format,...)
{
	char rega[2];
	char regc[3];
	unsigned char str[48];
	unsigned char Index = 0;
	unsigned short len;
	va_list start_Point;
	va_start(start_Point,format);
	len = vsprintf((char *)str,(const char *)format,start_Point);
	va_end(start_Point);
	for(Index = 0; Index < len;)
	{
		if(str[Index] < 0xA0)
		{
			rega[0] = str[Index++];
			rega[1] = '\0';
			if(font == 0x00)
			{
				LCMPrintfOneAscIICh16x8(x,y,rega,Inv);
				x = x + 8;
			}
			else
			{
				LCMPrintfOneAscIICh32x16(x,y,rega,Inv);
				x = x + 16;
			}
			
		}
		else
		{
			regc[0] = str[Index++];
			regc[1] = str[Index++];
			regc[2] = '\0';
			LCMPrintfOneChinese16x16(x,y,regc,Inv);
			x = x + 16;
		}
	}
}
/**************************************************************************************************************************
** @API Function name:   API_KEY_ReadKey
** @API Input para:      None
** @API Output para:     None
** @API retrun para:     None
**************************************************************************************************************************/
extern void API_LCM_PutBMP(unsigned char x,unsigned char y,unsigned char W,unsigned char H,unsigned char const *data)
{

}
/**************************************End Of File************************************************************************/
