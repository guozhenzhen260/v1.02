/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			config.h
 ** @brief:				add all head files 
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H


#define MDB_NON_RESPONSE_TIME	400		//MDB NON-RESPONSE timer define in section 10.4 of MDB V4.2

#include ".\APP\VENDSERVICE.H"
#include ".\APP\MAINTENANCE.H"
#include ".\APP\Android.H"
#include ".\DRIVERS\UART0.H"
#include ".\DRIVERS\UART3.H"
#include ".\DRIVERS\PINSELECT.H"
#include ".\DRIVERS\RTC.H"
#include ".\DRIVERS\LCM.H"
#include ".\DRIVERS\KEY.H"
#include ".\DRIVERS\TIMER.H"
#include ".\DRIVERS\BUZZER.H"
#include ".\DRIVERS\I2C.H"
#include ".\DRIVERS\MDB.H"
#include ".\DRIVERS\MOTOR.H"

#include ".\FreeRTOS\FreeRTOS.h"
#include ".\FreeRTOS\FreeRTOSConfig.h"
#include ".\FreeRtOS\include\task.h"
#include ".\FreeRTOS\include\list.H"
#include ".\FreeRTOS\include\queue.H"
#include ".\FreeRTOS\include\semphr.H"
#include ".\FreeRTOS\include\timers.H"
#include ".\CMISI\LPC17XX.H"

#include <stdint.h> 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <setjmp.h>
#include <rt_misc.h>
#include <stdarg.h>

#endif
/**************************************End Of File*******************************************************/
