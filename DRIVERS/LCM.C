/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			LCM.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\config.h"

#define LCMWIDTH	240
#define LCMHIGHT	16

#define LCM_nCS_SET()  LPC_GPIO0->FIODIR |= (1ul << 8); LPC_GPIO0->FIOSET |= (1ul << 8) //P0.8,CS
#define LCM_nCS_CLR()  LPC_GPIO0->FIODIR |= (1ul << 8); LPC_GPIO0->FIOCLR |= (1ul << 8)

#define LCM_MOSI_SET() LPC_GPIO0->FIODIR |= (1ul << 7); LPC_GPIO0->FIOSET |= (1ul << 7) //P0.7,MOSI
#define LCM_MOSI_CLR() LPC_GPIO0->FIODIR |= (1ul << 7); LPC_GPIO0->FIOCLR |= (1ul << 7)

#define LCM_SCLK_SET() LPC_GPIO0->FIODIR |= (1ul << 6); LPC_GPIO0->FIOSET |= (1ul << 6) //P0.6,SCLK
#define LCM_SCLK_CLR() LPC_GPIO0->FIODIR |= (1ul << 6); LPC_GPIO0->FIOCLR |= (1ul << 6)

#define LCM_CMD_SET()  LPC_GPIO0->FIODIR |= (1ul << 9); LPC_GPIO0->FIOSET |= (1ul << 9) //P0.9,CMD&DATA
#define LCM_CMD_CLR()  LPC_GPIO0->FIODIR |= (1ul << 9); LPC_GPIO0->FIOCLR |= (1ul << 9)

extern const char ASCII16X8INDEX[];
extern const unsigned char ASCII16X8LIB[];
extern const char CHINESE16X16INDEX[];
extern const unsigned char CHINESE16X16LIB[];
extern const char ASCII32X16INDEX[];
extern const unsigned char ASCII32X16LIB[];

volatile unsigned char X,Y;
static void LCMPutChar(unsigned char x,unsigned char y,const unsigned char ch);
static void PutCmdIntoLcmController(unsigned char cmd);
static void PutDataIntoLcmController(unsigned char data);
static void PutByteIntoSimSpiBus(unsigned char byte);

/*********************************************************************************************************
** @DRIVER Function name:     	LCMInit
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
*********************************************************************************************************/
void LCMInit(void)
{
	PutCmdIntoLcmController(0xE2);
	vTaskDelay(20);					//Wait for 40ms
    PutCmdIntoLcmController(0x26);  //Set Multiplex Rate and Temperature Compensation
	PutCmdIntoLcmController(0x2f);  //Set Power Controlinternal vlcd(11x charge pump)
	PutCmdIntoLcmController(0xea);  //set bias=1/12
	PutCmdIntoLcmController(0x81);	//Electronic volume mode set
	PutCmdIntoLcmController(0x72);	
	PutCmdIntoLcmController(0xc8);	//set MX=0, SEG output normal direction		
    PutCmdIntoLcmController(0x40);  //set start line
	PutCmdIntoLcmController(0xAF);  //set display enable
	LCMClearScreen();				//Clear Screen 
	LCMBacklightControl(1);			//BlackLight On 
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMClearScreen
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
*********************************************************************************************************/
void LCMClearScreen(void)
{
	unsigned char x,y;
	for(y=0;y<LCMHIGHT;y++)
	{
		LCMSetXY(0,y);
		for(x=0;x<LCMWIDTH;x++)
		{
			PutDataIntoLcmController(0x00);
		}
	}
	X = 0;
	Y = 0;
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMClearArea
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
*********************************************************************************************************/
void LCMClearArea(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
	unsigned char i,j;
	for(i=0;i<=(y2-y1);i++)
	{
		for(j=x1;j<=x2;j++)
		{
			LCMSetXY(j,y1+i);
			PutDataIntoLcmController(0x00);
		}	
	}
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMDrawline
** @DRIVER Input para:     		x,y
** @DRIVER retrun para:     	None
*********************************************************************************************************/
void LCMDrawline(unsigned char x,unsigned char y)
{
	unsigned char i;
	LCMSetXY(x,y);
	for(i=0;i<(LCMWIDTH-x);i++)
	{		
		PutDataIntoLcmController(0x08);	
	}
}	
/*********************************************************************************************************
** @DRIVER Function name:     	LCMBacklightControl
** @DRIVER Input para:     		x:x coordinate(0~239);y:y coordinate(line,each line has 8 dots,0~9)
** @DRIVER retrun para:     	None
*********************************************************************************************************/
void LCMSetXY(unsigned char x,unsigned char y)
{
	X = x;
	Y = y;
	PutCmdIntoLcmController(y | 0xB0);			// PAGE ADD 
	PutCmdIntoLcmController(x >> 4 | 0x10);		// column address MSB
	PutCmdIntoLcmController((x & 0x0F) | 0x00);	// column address LSB
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMGetX
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	X
*********************************************************************************************************/
unsigned char LCMGetX(void)
{
	return X;
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMGetY
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	Y
*********************************************************************************************************/
unsigned char LCMGetY(void)
{
	return Y;
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMPrintfOneAscIICh
** @DRIVER Description:			LCM display one ascii char
** @DRIVER Input para:			x:column addr;y:row addr;Inv:0-normal,1-Inversedd
*********************************************************************************************************/
void LCMPrintfOneAscIICh16x8(unsigned char x,unsigned char y,char *str,unsigned char Inv)
{
	char *position;
	unsigned char i;
	unsigned short OffSet;
	position = (char *)strstr((const char *)ASCII16X8INDEX,(const char *)str);
	if(position < ASCII16X8INDEX)
		return;
	OffSet = ((position - ASCII16X8INDEX) << 4);
	if(Inv)
	{
		for(i=0;i<8;i++)
		{
			LCMPutChar(x+i,y,~ASCII16X8LIB[OffSet++]);
		}
		for(i=0;i<8;i++)
		{
			LCMPutChar(x+i,y+1,~ASCII16X8LIB[OffSet++]);
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			LCMPutChar(x+i,y,ASCII16X8LIB[OffSet++]);
		}
		for(i=0;i<8;i++)
		{
			LCMPutChar(x+i,y+1,ASCII16X8LIB[OffSet++]);
		}
	}		
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMPrintfOneAscIICh
** @DRIVER Description:			LCM display one ascii char
** @DRIVER Input para:			x:column addr;y:row addr;Inv:0-normal,1-Inversedd
*********************************************************************************************************/
void LCMPrintfOneAscIICh32x16(unsigned char x,unsigned char y,char *str,unsigned char Inv)
{
	char *position;
	unsigned char i;
	unsigned short OffSet;
	//str must be in "0123456789."
	if(((str[0] >= 0x30) && (str[0] <= 0x39)) || (str[0] == '.'))
	{
		position = (char *)strstr((const char *)ASCII32X16INDEX,(const char *)str);
		if(position < ASCII32X16INDEX)
			return;
		OffSet = ((position - ASCII32X16INDEX) << 6);
		if(Inv)
		{
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y,~ASCII32X16LIB[OffSet++]);
			}
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y+1,~ASCII32X16LIB[OffSet++]);
			}
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y+3,~ASCII32X16LIB[OffSet++]);
			}
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y+4,~ASCII32X16LIB[OffSet++]);
			}
		}
		else
		{
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y,ASCII32X16LIB[OffSet++]);
			}
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y+1,ASCII32X16LIB[OffSet++]);
			}
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y+2,ASCII32X16LIB[OffSet++]);
			}
			for(i=0;i<16;i++)
			{
				LCMPutChar(x+i,y+3,ASCII32X16LIB[OffSet++]);
			}
		}
	}		
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMPrintfOneChinese
** @DRIVER Description:			LCM display one ascii char
** @DRIVER Input para:			x:column addr;y:row addr;Inv:0-normal,1-Inversedd
*********************************************************************************************************/
void LCMPrintfOneChinese16x16(unsigned char x,unsigned char y,char *str,unsigned char Inv)
{
	char *OffSet;
	unsigned char i;
	unsigned int AddOffSet;
	OffSet = (char *)strstr((const char *)CHINESE16X16INDEX,str);
	if(OffSet >= CHINESE16X16INDEX)
		AddOffSet = ((OffSet - CHINESE16X16INDEX) << 4);
	else
		return;
	if(Inv)
	{
		for(i=0;i<16;i++)
		{
			LCMPutChar(x+i,y,~CHINESE16X16LIB[AddOffSet++]);
		}
		for(i=0;i<16;i++)
		{
			LCMPutChar(x+i,y+1,~CHINESE16X16LIB[AddOffSet++]);
		}
	}
	else
	{
		for(i=0;i<16;i++)
		{
			LCMPutChar(x+i,y,CHINESE16X16LIB[AddOffSet++]);
		}
		for(i=0;i<16;i++)
		{
			LCMPutChar(x+i,y+1,CHINESE16X16LIB[AddOffSet++]);
		}
	}
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMPrintfBmp
** @DRIVER Description:			LCM display one bmp picture
** @DRIVER Input para:			x:column addr;y:with:bmp with;hight bmp hight
*********************************************************************************************************/
void LCMPrintfBmp(unsigned char x,unsigned char y,unsigned char with,unsigned char hight,char *str)
{
	unsigned char i;
	unsigned char Row;
	unsigned short Index;
	Index = 0x00;
	for(Row=0;Row<(hight/8);Row++)
	{
		for(i=0;i<with;i++)
		{
			LCMPutChar(x+i,y,str[Index++]);
		}
		y++;
	}		
}
/*********************************************************************************************************
** @DRIVER Function name:     	LCMBacklightControl
** @DRIVER Input para:     		cmd:1-open,0-close
** @DRIVER retrun para:     	None
*********************************************************************************************************/
void LCMBacklightControl(unsigned char cmd)
{
	LPC_GPIO2->FIODIR |= 1ul << 2;
	if(cmd)
		LPC_GPIO2->FIOSET |= 1ul << 2;
	else
		LPC_GPIO2->FIOCLR |= 1ul << 2;
}
/*********************************************************************************************************
** @Driver Function name:     	PutByteIntoSimSpiBus
** @Driver Input para:     		x:x coordinate(0~239);y:y coordinate(line,0~9);ch:data
** @Driver retrun para:     	None
*********************************************************************************************************/
static void LCMPutChar(unsigned char x,unsigned char y,const unsigned char ch)
{
	LCMSetXY(x,y);
	PutDataIntoLcmController(ch);		
}
/*********************************************************************************************************
** @Driver Function name:     	PutCmdIntoLcmController
** @Driver Input para:     		cmd:cmd
** @Driver retrun para:     	None
*********************************************************************************************************/
static void PutCmdIntoLcmController(unsigned char cmd)
{
	LCM_nCS_SET();
	LCM_CMD_CLR();
	PutByteIntoSimSpiBus(cmd);
	LCM_CMD_SET();
	LCM_nCS_CLR();
}
/*********************************************************************************************************
** @Driver Function name:     	PutDataIntoLcmController
** @Driver Input para:     		data:data
** @Driver retrun para:     	None
*********************************************************************************************************/
static void PutDataIntoLcmController(unsigned char data)
{
	LCM_nCS_SET();
	LCM_CMD_SET();
	PutByteIntoSimSpiBus(data);
	LCM_CMD_CLR();
	LCM_nCS_CLR();
}
/*********************************************************************************************************
** @Driver Function name:     	PutByteIntoSimSpiBus
** @Driver Input para:     		byte:data
** @Driver retrun para:     	None
*********************************************************************************************************/
static void PutByteIntoSimSpiBus(unsigned char byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		LCM_SCLK_CLR();
		if((byte & 0x80) == 0)
		{
			LCM_MOSI_CLR();
		}
		else
		{
			LCM_MOSI_SET();
		} 
		LCM_SCLK_SET();
		byte = byte<<1;
	}
}
/**************************************End Of File*******************************************************/
