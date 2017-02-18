/****************************************Copyright (c)****************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info---------------------------------------------------------------------
 ** @Filename:			I2C.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *********************************************************************************************************************/
#include "..\config.h"
/*********************************************************************************************************************/
#define ONE_BYTE_SUBA   1
#define TWO_BYTE_SUBA   2
#define X_ADD_8_SUBA    3
#define I2C0_OK			1
#define I2C0_ERR		0
/*********************************************************************************************************************/
volatile unsigned char I2C0_sla;		//器件从地址
volatile unsigned int  I2C0_suba;		//内部子地址
volatile unsigned char I2C0_suba_num;	//内部子地址字节数
volatile unsigned char *I2C0_buf;		//数据缓冲地址
volatile unsigned int  I2C0_num;		//发送或接受字节数
volatile unsigned char I2C0_end;		//1--总线操作结束;0--未结束
volatile unsigned char I2C0_suba_en;	//0--子地址已经处理或者不需要子地址;1--读取操作;2--写操作
/*********************************************************************************************************************/
static unsigned char Wait_I2C0_End(unsigned int dly);
/**********************************************************************************************************************
** @DRIVER Function name:     	InitTimer
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
** @DRIVER note:				None
**********************************************************************************************************************/
void I2C0Init(void)
{
	LPC_SC->PCONP |= PCI2C0_FUNC << 7;
	LPC_I2C0->I2SCLH 	= (12000000/200000+1)/2; 
	LPC_I2C0->I2SCLL 	= (12000000/200000)/2;
	LPC_I2C0->I2CONCLR 	= 0x2C;
	LPC_I2C0->I2CONSET 	= 0x40; 
	NVIC_EnableIRQ(I2C0_IRQn);
	NVIC_SetPriority(I2C0_IRQn,3);
}
/**********************************************************************************************************************
** @DRIVER Function name:     	Wait_I2C0_End
** @DRIVER Input para:     		dly:wait time
** @DRIVER retrun para:     	None
** @DRIVER note:				0x00:err;0x01:ok
**********************************************************************************************************************/
static unsigned char Wait_I2C0_End(unsigned int dly)
{  
	unsigned int i,j;
    if(I2C0_end == 1)
		return 0x01;
	for(j=0;j<dly;j++)
	{
	    for(i=0;i<80000;i++)
	    {
	        if(I2C0_end == 1)
				return 0x01;
	    }
	}
    return 0x00;
}
/**********************************************************************************************************************
** @DRIVER Function name:     	I2C0RdNByte
** @DRIVER Input para:     		sla:Device addr;suba_type:sub add type;s:data;num:len of data
** @DRIVER retrun para:     	None
** @DRIVER note:				0x00:err;0x01:ok
**********************************************************************************************************************/
unsigned char I2C0RdNByte(unsigned char sla,unsigned int suba_type,unsigned int suba,unsigned char *s,unsigned int num)
{
    if(num > 0)                                         
    {                                                   
        if(suba_type == 1)
        {                                               
            I2C0_sla = sla + 1;                 
            I2C0_suba = suba;                    
            I2C0_suba_num = 1;                       
        }
        if(suba_type == 2)
        {                                               
            I2C0_sla = sla + 1;                 
            I2C0_suba = suba;                    
            I2C0_suba_num = 2;                       
        }
        if(suba_type == 3)
        {                                               
            I2C0_sla = sla + ((suba >> 7 )& 0x0e) + 1;
            I2C0_suba = suba & 0x0ff;            
            I2C0_suba_num = 1;                       
        }
        I2C0_buf = s;                               
        I2C0_num = num;                             
        I2C0_suba_en = 1;                               
        I2C0_end = 0;
		LPC_I2C0->I2CONCLR = (1 << 2)|(1 << 3)|(1 << 5);//AA | SI | START            
		LPC_I2C0->I2CONSET = (1 << 5)|(1 << 6);//START | I2EN            
        return(Wait_I2C0_End(100));
    }
    return 0x00;
}
/**********************************************************************************************************************
** @DRIVER Function name:     	I2C0WrNByte
** @DRIVER Input para:     		sla:Device addr;suba_type:sub add type;s:data;num:len of data
** @DRIVER retrun para:     	None
** @DRIVER note:				0x00:err;0x01:ok
**********************************************************************************************************************/
unsigned char I2C0WrNByte(unsigned char sla,unsigned char suba_type,unsigned int suba,unsigned char *s,unsigned int num)
{
    if(num > 0) 
    {                                                       
        if(suba_type == 1)
        {                                                   
            I2C0_sla = sla;                         
            I2C0_suba = suba;                        
            I2C0_suba_num = 1;                           
        }
        if(suba_type == 2)
        {                                                   
            I2C0_sla = sla;                         
            I2C0_suba = suba;                        
            I2C0_suba_num = 2;                           
        }
        if(suba_type == 3)
        {                                                   
            I2C0_sla = sla + ((suba >> 7 )& 0x0e);     
            I2C0_suba = suba & 0x0ff;                
            I2C0_suba_num = 1;                           
        }
        I2C0_buf = s;                                   
        I2C0_num = num;                                 
        I2C0_suba_en = 2;                                   
        I2C0_end = 0;
		LPC_I2C0->I2CONCLR = (1 << 2)|(1 << 3)|(1 << 5);                                          
		LPC_I2C0->I2CONSET = (1 << 5)|(1 << 6);                                        
        return(Wait_I2C0_End(100));					
    }
    return 0x00;
}
/**********************************************************************************************************************
** @DRIVER Function name:     	I2C0_IRQHandler
** @DRIVER Input para:     		None
** @DRIVER retrun para:     	None
**********************************************************************************************************************/
void I2C0_IRQHandler(void)
{
    switch(LPC_I2C0->I2STAT & 0xF8)
    {
		case 0x08:
			if(I2C0_suba_en == 1)			
				LPC_I2C0->I2DAT = I2C0_sla & 0xFE;  
			else                            
				LPC_I2C0->I2DAT = I2C0_sla;         
			LPC_I2C0->I2CONCLR = (1 << 3) | (1 << 5);  	
			break;
        case 0x10:
			LPC_I2C0->I2DAT = I2C0_sla;				                                        
			LPC_I2C0->I2CONCLR = 0x28;    			
			break;
        case 0x18:	
        case 0x28:
			if(I2C0_suba_en == 0)			
			{
				if(I2C0_num > 0)
				{ 
					LPC_I2C0->I2DAT = *I2C0_buf++;
					LPC_I2C0->I2CONCLR = 0x28;   
					I2C0_num--;
				}
				else                     
				{                        
					LPC_I2C0->I2CONSET = (1<<4);
					LPC_I2C0->I2CONCLR = 0x28;  
					I2C0_end = 1;
				}
			}
			if(I2C0_suba_en == 1)       
			{
				if(I2C0_suba_num == 2)
				{
					LPC_I2C0->I2DAT = ((I2C0_suba >> 8) & 0xff);
					LPC_I2C0->I2CONCLR = 0x28; 
					I2C0_suba_num--;
					break;
				}
				if(I2C0_suba_num == 1)
				{
					LPC_I2C0->I2DAT = (I2C0_suba & 0xff);
					LPC_I2C0->I2CONCLR = 0x28; 
					I2C0_suba_num--;
					break;
				}
				if(I2C0_suba_num == 0)
				{
					LPC_I2C0->I2CONCLR = 0x08;
					LPC_I2C0->I2CONSET = 0x20;
					I2C0_suba_en = 0;
					break;
				}
			}
			if(I2C0_suba_en == 2)
			{
				if(I2C0_suba_num > 0)
				{    
					if (I2C0_suba_num == 2)
					{    
						LPC_I2C0->I2DAT = ((I2C0_suba >> 8) & 0xff);
						LPC_I2C0->I2CONCLR = 0x28;
						I2C0_suba_num--;
						break;
					}
					if (I2C0_suba_num == 1)
					{   
						LPC_I2C0->I2DAT    = (I2C0_suba & 0xff);
						LPC_I2C0->I2CONCLR = 0x28;
						I2C0_suba_num--;
						I2C0_suba_en  = 0;
						break;
					}
				}
			}
			break;
        case 0x40:
			if (I2C0_num <= 1)                      
				LPC_I2C0->I2CONCLR = 1 << 2;        
			else					
				LPC_I2C0->I2CONSET = 1 << 2;         
				LPC_I2C0->I2CONCLR = 0x28;               
			break;
        case 0x20:                                   
        case 0x30: 
        case 0x38:
        case 0x48:
			LPC_I2C0->I2CONCLR = 0x28;
			I2C0_end = 0xFF;
			break;
        case 0x50:
			*I2C0_buf++ = LPC_I2C0->I2DAT;
			I2C0_num--;
			if(I2C0_num == 1)
				LPC_I2C0->I2CONCLR = 0x2C;
			else
			{
				LPC_I2C0->I2CONSET = 0x04;             
				LPC_I2C0->I2CONCLR = 0x28;
			}
			break;
        case 0x58:
			*I2C0_buf++ = LPC_I2C0->I2DAT;
			LPC_I2C0->I2CONSET = 0x10;
			LPC_I2C0->I2CONCLR = 0x28;
			I2C0_end = 1;
			break;
        default:
			break;
    }
}
/**************************************End Of File***************************************************************/
