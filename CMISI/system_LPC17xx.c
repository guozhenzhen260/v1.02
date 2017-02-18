/********************************************************************************************************************************
 * @file:    system_LPC17xx.c
 * @purpose: CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *           for the NXP LPC17xx Device Series 
 * @version: V1.6
 * @date:    12th February 2010
 * @modify:  07th Oct 2016 by chen
 *-------------------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2008 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ********************************************************************************************************************************/

#include <stdint.h>
#include "LPC17xx.h"

#define XTAL        	12000000	//外部晶体振荡频率-------12MHz
#define FCCLK       	96000000	//CPU核心频率------------96MHz
#define FCCO        	288000000	//锁相环频率------------288MHz
#define FPCLK       	24000000	//外设时钟频率-----------24MHz
#define PLL_NVALUE  	1
#define PLL_MVALUE  	12

/*********************************************************************************************************************************
** @BSP Function name:     	SystemInit -- config system clock,pll,flash
** @BSP Input para:     	None
** @BSP retrun para:     	None
*********************************************************************************************************************************/
void SystemInit(void)
{
	LPC_SC->PCLKSEL0 = 0x00000000;
	LPC_SC->PCLKSEL1 = 0x00000000;
	LPC_SC->CLKOUTCFG = 0;
    if ((LPC_SC->PLL0STAT >> 24) == 1) 
	{
        LPC_SC->PLL0CON = 1;
        LPC_SC->PLL0FEED = 0xAA;
        LPC_SC->PLL0FEED = 0x55;
    }
    LPC_SC->PLL0CON = 0;
    LPC_SC->PLL0FEED = 0xAA;
    LPC_SC->PLL0FEED = 0x55;
    while(LPC_SC->PLL0STAT & (3 << 24));
	LPC_SC->SCS = (1UL << 5);							
	while((LPC_SC->SCS & (1<<6)) == 0);
	LPC_SC->CLKSRCSEL = 0x01;
    LPC_SC->PLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));    
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;    
    LPC_SC->PLL0CON   = 1;
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;
    while((LPC_SC->PLL0STAT & (1ul << 24)) == 0);
    LPC_SC->CCLKCFG = (FCCO / FCCLK) - 1;   
    while(((LPC_SC->PLL0STAT & (1ul << 26)) == 0));
    while(((LPC_SC->PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1)) && (((LPC_SC->PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1)));
    LPC_SC->PLL0CON  = 3;
    LPC_SC->PLL0FEED = 0xAA;
    LPC_SC->PLL0FEED = 0x55;                                
    while((LPC_SC->PLL0STAT & (1ul << 25)) == 0);
	LPC_SC->FLASHCFG = (((0x05ul << 12) & (~(0x003f))) | 0x003a);
	LPC_SC->PCLKSEL0  = 0x00;
	LPC_SC->PCLKSEL1  = 0x00;
}
/**************************************************End Of File*******************************************************************/
