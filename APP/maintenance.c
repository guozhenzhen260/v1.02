/****************************************Copyright (c)****************************************************************************************************************************
**                      Fuzhou LinkMac Information Technology Co.,Ltd.
**                               http://www.linkmac.com.cn
**--------------File Info---------------------------------------------------------------------------------------------------------------------------------------------------------
** File name:           Maintenance.c
** Last modified Date:  2016-07-01
** Last Version:         
** Descriptions:        Uart0          
**--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2016-07-01
** Version:             V1.0 
** Descriptions:        The original version       
***********************************************************************************************************************************************************************************/
#include "..\API\API.H"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "Menu.h"
#include "MdbBillDeviceOperation.h"
#include "MdbCoinDeviceOperation.h"
#include "Log.h"
static void Login(void);
static void EnterMaintenance(void);
static void MaintenDevInfo(void);
static void MaintenDevConfig(void);
static void MaintenDevTest(void);
static void MaintenTradeConfig(void);
static void MaintenTradeLog(void);
static void TestBillValidator(void);
static void TestCoinValidator(void);
static void TestColumn(void);
/************************************************************************************************************************************************************************************
** @APP Function name:   Errcheck
** @APP Input para:      系统错误检测并生成错误代码
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static char *Errcheck()
{
	static char	strMoney[250]="";
	//纸币器	
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		if(MdbBillErr.Communicate)
			strcat(strMoney,"B1");
		if(MdbBillErr.moto)
			strcat(strMoney,"B2");
		if(MdbBillErr.sensor)
			strcat(strMoney,"B3");
		if(MdbBillErr.romchk)
			strcat(strMoney,"B4");	
		if(MdbBillErr.jam)
			strcat(strMoney,"B5");
		if(MdbBillErr.removeCash)
			strcat(strMoney,"B6");
		if(MdbBillErr.cashErr)
			strcat(strMoney,"B7");	
	}
	//硬币器
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		if(MdbCoinErr.Communicate)
			strcat(strMoney,"C1");
		if(MdbCoinErr.sensor)
			strcat(strMoney,"C2");
		if(MdbCoinErr.tubejam)
			strcat(strMoney,"C3");
		if(MdbCoinErr.romchk)
			strcat(strMoney,"C4");	
		if(MdbCoinErr.routing)
			strcat(strMoney,"C5");
		if(MdbCoinErr.jam)
			strcat(strMoney,"C6");
		if(MdbCoinErr.removeTube)
			strcat(strMoney,"C7");	
	}
	return strMoney;
}
/************************************************************************************************************************************************************************************
** @APP Function name:   PwdCheck/密码验证验证
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static uint8_t PwdCheck(void)
{
	unsigned char i,key[8],j,check,X,Y;
	API_LCM_ClearScreen();
	API_LCM_Printf(0,0,0,0,LoginUI.Prompt[VMCParam.Language]);
	API_LCM_DrawLine(0,2);
	API_LCM_Printf(16,3,0,0,LoginUI.Prompt[VMCParam.Language]);
	API_LCM_Printf(16,14,0,0,LoginUI.Exit[VMCParam.Language]);
	i = 0x00;
	X = 72;
	Y = 7;
	while(1)
	{
		key[i] = API_KEY_ReadKey();
		if(key[i])
		{
			if(key[i] == '>')
				break;
			if(key[i] == '<')
			{
				if(i < 5)
				{
					API_LCM_ClearArea(0,7,239,8);
					API_LCM_Printf(48,7,0,0,LoginUI.Error[VMCParam.Language]);
					vTaskDelay(100);
					API_LCM_ClearArea(0,7,239,8);
					X = 72;
					i = 0x00;
				}
				else
				{
					check = 0x00;
					for(j=0;j<6;j++)
					{
						if(key[j] == VMCParam.Password[j])
							check = 0x01;
						else
						{
							check = 0x00;
							break;
						}
					}
					if(check == 0x00)
					{
						API_LCM_ClearArea(0,7,239,8);
						API_LCM_Printf(48,7,0,0,LoginUI.Error[VMCParam.Language]);
						vTaskDelay(100);
						API_LCM_ClearArea(0,7,239,8);
						X = 72;
						i = 0x00;	
						return 0;
					}
					else
					{
						return 1;
					}
				}
			}
			else
			{
				if(i <= 5)
				{
					API_LCM_Printf(X,Y,0,0,"*");
					X += 16;
					i++;
				}
			}
		}
		vTaskDelay(10);
	}
	return 0;
}
/************************************************************************************************************************************************************************************
** @APP Function name:   Maintenance/维护任务
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
void Maintenance(void *pvParameters)
{
	vTaskDelay(200);
	pvParameters = pvParameters;
	while(1)
	{
		switch(VMCParam.VMCMode)
		{
			case VMC_MODE_VEND:
				vTaskDelay(200);
				break;
			case VMC_MODE_MAIN:
				Login();
				VMCParam.VMCMode = VMC_MODE_VEND;
				break;
			default:
				break;
		}
		vTaskDelay(20);
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   Login/登陆验证
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void Login(void)
{
	unsigned char i,key[8],j,check,X,Y;
	API_LCM_ClearScreen();
	API_LCM_Printf(0,0,0,0,LoginUI.Theme[VMCParam.Language]);
	API_LCM_DrawLine(0,2);
	API_LCM_Printf(16,3,0,0,LoginUI.Prompt[VMCParam.Language]);
	API_LCM_Printf(16,14,0,0,LoginUI.Exit[VMCParam.Language]);
	i = 0x00;
	X = 72;
	Y = 7;
	while(1)
	{
		key[i] = API_KEY_ReadKey();
		if(key[i])
		{
			if(key[i] == '>')
				break;
			if(key[i] == '<')
			{
				if(i < 5)
				{
					API_LCM_ClearArea(0,7,239,8);
					API_LCM_Printf(48,7,0,0,LoginUI.Error[VMCParam.Language]);
					vTaskDelay(100);
					API_LCM_ClearArea(0,7,239,8);
					X = 72;
					i = 0x00;
				}
				else
				{
					check = 0x00;
					for(j=0;j<6;j++)
					{
						if(key[j] == VMCParam.Password[j])
							check = 0x01;
						else
						{
							check = 0x00;
							break;
						}
					}
					if(check == 0x00)
					{
						API_LCM_ClearArea(0,7,239,8);
						API_LCM_Printf(48,7,0,0,LoginUI.Error[VMCParam.Language]);
						vTaskDelay(100);
						API_LCM_ClearArea(0,7,239,8);
						X = 72;
						i = 0x00;				
					}
					else
					{
						EnterMaintenance();
						break;
					}
				}
			}
			else
			{
				if(i <= 5)
				{
					API_LCM_Printf(X,Y,0,0,"*");
					X += 16;
					i++;
				}
			}
		}
		vTaskDelay(10);
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   EnterMaintenance/进入维护模式
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void EnterMaintenance(void)
{
	unsigned char key,Exit = 0x00,Next = 0x00;
	void (*SubMenu)(void);
	while(1)
	{
		API_LCM_ClearScreen();
		API_LCM_Printf(0,0,0,0,MaintenUI.Theme[VMCParam.Language]);
		API_LCM_DrawLine(0,2);
		API_LCM_Printf(16,3,0,0,MaintenUI.DeviceInfo[VMCParam.Language]);
		API_LCM_Printf(16,5,0,0,MaintenUI.DeviceConfig[VMCParam.Language]);
		API_LCM_Printf(16,7,0,0,MaintenUI.DeviceTest[VMCParam.Language]);
		API_LCM_Printf(16,9,0,0,MaintenUI.TradeSet[VMCParam.Language]);
		API_LCM_Printf(16,11,0,0,MaintenUI.TradeLOG[VMCParam.Language]);
		API_LCM_Printf(0,14,0,0,MaintenUI.Exit[VMCParam.Language]);
		while(1)
		{
			Next = 0x00;
			key = API_KEY_ReadKey();
			if(key == '>')
			{
				Exit = 0x01;
				break;
			}
			else
			{
				switch(key)
				{
					case 'A':
						Next = 0x01;
						SubMenu = MaintenDevInfo;
						break;
					case 'B':
						Next = 0x01;
						SubMenu = MaintenDevConfig;
						break;
					case 'C':
						Next = 0x01;
						SubMenu = MaintenDevTest;
						break;
					case 'D':
						Next = 0x01;
						SubMenu = MaintenTradeConfig;
						break;
					case 'E':
						Next = 0x01;
						SubMenu = MaintenTradeLog;
						break;
					default:
						Next = 0x00;
						break;
				}
			}
			if(Next)
				break;
			vTaskDelay(10);
		}
		if(Exit)
			break;
		else
			(*SubMenu)();
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   MaintenDevInfo/设备信息
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void MaintenDevInfo(void)
{
	unsigned char i,Id[7];
	char    *pstr;
	char	strMoney[250];

	pstr = Errcheck();
	strcpy(strMoney, pstr);
	for(i=0;i<7;i++)
	{
		Id[i] = ((((VMCParam.VMCID[i])>>4)*10)+((VMCParam.VMCID[i])&0x0f));//BCD to HEX
	}
	API_LCM_ClearScreen();
	API_LCM_Printf(0,0,0,0,DevInfo.Theme[VMCParam.Language]);
	API_LCM_DrawLine(0,2);
	//软件版本
	API_LCM_Printf(16,3,0,0,DevInfo.Version[VMCParam.Language]);
	API_LCM_Printf((16 + (strlen(DevInfo.Version[VMCParam.Language])) * 8),3,0,0,SOFTWAREVERSION);
	//版本序列号
	API_LCM_Printf(16,5,0,0,DevInfo.DevID[VMCParam.Language]);
	API_LCM_Printf((16 + (strlen(DevInfo.DevID[VMCParam.Language])) * 8),5,0,0,"%d%d%d%d%d%d%d",Id[0],Id[1],Id[2],Id[3],Id[4],Id[5],Id[6]);
	//故障码
	API_LCM_Printf(16,7,0,0,DevInfo.DevList[VMCParam.Language]);
	API_LCM_Printf((16 + (strlen(DevInfo.DevList[VMCParam.Language])) * 8),7,0,0,"%s",strMoney);
	API_LCM_Printf(0,14,0,0,DevInfo.Exit[VMCParam.Language]);
	while(1)
	{
		if(API_KEY_ReadKey() == '>')
			break;
		vTaskDelay(20);
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   MaintenDevConfig/设备配置
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void MaintenDevConfig(void)
{
	unsigned char key,Exit = 0x00,Back = 0x00,Lev,i;
	unsigned short Year;
	unsigned char Mon,Day,Hour,Min;
	while(1)
	{
		Back = 0x00;
		API_LCM_ClearScreen();
		API_LCM_Printf(0,0,0,0,DevConfig.Theme[VMCParam.Language]);
		API_LCM_DrawLine(0,2);
		API_LCM_Printf(16,3,0,0,DevConfig.Bill[VMCParam.Language],VMCParam.MdbBillDeviceEAB);
		API_LCM_Printf(16,5,0,0,DevConfig.Coin[VMCParam.Language],VMCParam.MdbCoinDeviceEAB);
		API_LCM_Printf(16,7,0,0,DevConfig.Cashless[VMCParam.Language],VMCParam.MdbCashlessDeviceEAB);
		API_LCM_Printf(16,9,0,0,DevConfig.Lan[VMCParam.Language],VMCParam.Language);
		API_LCM_Printf(128,9,0,0,DevConfig.Rtc[VMCParam.Language],VMCParam.Language);
		API_LCM_Printf(16,12,0,0,DevConfig.Note[VMCParam.Language]);
		API_LCM_Printf(0,14,0,0,DevConfig.Exit[VMCParam.Language]);
		while(1)
		{
			key = API_KEY_ReadKey();
			if(key == '>')
			{
				Exit = 0x01;
				break;
			}
			else
			{
				switch(key)
				{
					case 'A':
						API_LCM_Printf(16,3,0,1,DevConfig.Bill[VMCParam.Language],VMCParam.MdbBillDeviceEAB);
						while(1)
						{
							key = API_KEY_ReadKey();
							if((key == '<') || (key == '>'))
							{
								Back = 0x01;
								if(key == '<')
									LoadNewVmcParam();
								break;
							}
							else
							{
								switch(key)
								{
									case '0':
										VMCParam.MdbBillDeviceEAB = 0x00;
										API_LCM_Printf(16,3,0,1,DevConfig.Bill[VMCParam.Language],VMCParam.MdbBillDeviceEAB);
										break;
									case '1':
										VMCParam.MdbBillDeviceEAB = 0x01;
										API_LCM_Printf(16,3,0,1,DevConfig.Bill[VMCParam.Language],VMCParam.MdbBillDeviceEAB);
										break;
									default:
										break;
								}
							}
							vTaskDelay(20);
						}
						break;
					case 'B':
						API_LCM_Printf(16,5,0,1,DevConfig.Coin[VMCParam.Language],VMCParam.MdbCoinDeviceEAB);
						while(1)
						{
							key = API_KEY_ReadKey();
							if((key == '<') || (key == '>'))
							{
								Back = 0x01;
								if(key == '<')
									LoadNewVmcParam();
								break;
							}
							else
							{
								switch(key)
								{
									case '0':
										VMCParam.MdbCoinDeviceEAB = 0x00;
										API_LCM_Printf(16,5,0,1,DevConfig.Coin[VMCParam.Language],VMCParam.MdbCoinDeviceEAB);
										break;
									case '1':
										VMCParam.MdbCoinDeviceEAB = 0x01;
										API_LCM_Printf(16,5,0,1,DevConfig.Coin[VMCParam.Language],VMCParam.MdbCoinDeviceEAB);
										break;
									default:
										break;
								}
							}
							vTaskDelay(20);
						}
						break;
					case 'C':
						API_LCM_Printf(16,7,0,1,DevConfig.Cashless[VMCParam.Language],VMCParam.MdbCashlessDeviceEAB);
						while(1)
						{
							key = API_KEY_ReadKey();
							if((key == '<') || (key == '>'))
							{
								Back = 0x01;
								if(key == '<')
									LoadNewVmcParam();
								break;
							}
							else
							{
								switch(key)
								{
									case '0':
										VMCParam.MdbCashlessDeviceEAB = 0x00;
										API_LCM_Printf(16,7,0,1,DevConfig.Cashless[VMCParam.Language],VMCParam.MdbCashlessDeviceEAB);
										break;
									case '1':
										VMCParam.MdbCashlessDeviceEAB = 0x01;
										API_LCM_Printf(16,7,0,1,DevConfig.Cashless[VMCParam.Language],VMCParam.MdbCashlessDeviceEAB);
										break;
									default:
										break;
								}
							}
							vTaskDelay(20);
						}
						break;
					case 'D'://lan
						API_LCM_Printf(16,9,0,1,DevConfig.Lan[VMCParam.Language],VMCParam.Language);
						while(1)
						{
							key = API_KEY_ReadKey();
							if((key == '<') || (key == '>'))
							{
								Back = 0x01;
								if(key == '<')
									LoadNewVmcParam();
								break;
							}
							else
							{
								switch(key)
								{
									case '0':
										VMCParam.Language = 0x00;
										API_LCM_Printf(16,9,0,1,DevConfig.Lan[VMCParam.Language],VMCParam.Language);
										break;
									case '1':
										VMCParam.Language = 0x01;
										API_LCM_Printf(16,9,0,1,DevConfig.Lan[VMCParam.Language],VMCParam.Language);
										break;
									default:
										break;
								}
							}
							vTaskDelay(20);
						}
						break;
					case 'E'://RTC
						while(1)
						{
							Lev = 0x00;
							API_LCM_ClearScreen();
							API_LCM_Printf(0,0,0,0,DevConfig.RtcTheme[VMCParam.Language]);
							API_LCM_DrawLine(0,2);
							API_RTC_Read((void *)Rtc);
							API_LCM_Printf(16,3,0,0,DevConfig.RtcYear[VMCParam.Language],((Rtc[5] << 8) + Rtc[6]));
							API_LCM_Printf(16,5,0,0,DevConfig.RtcMon[VMCParam.Language],Rtc[4]);
							API_LCM_Printf(16,7,0,0,DevConfig.RtcDay[VMCParam.Language],Rtc[3]);
							API_LCM_Printf(16,9,0,0,DevConfig.RtcHour[VMCParam.Language],Rtc[2]);
							API_LCM_Printf(16,11,0,0,DevConfig.RtcMin[VMCParam.Language],Rtc[1]);
							API_LCM_Printf(0,14,0,0,DevConfig.Exit[VMCParam.Language]);
							while(1)
							{
								key = API_KEY_ReadKey();
								if((key == '<') || (key == '>'))
								{
									Back = 0x01;
									if(key == '<')
										LoadNewVmcParam();
									break;
								}
								else
								{
									switch(key)
									{
										case 'A':
											API_LCM_Printf(16,3,0,1,DevConfig.RtcYear[VMCParam.Language],((Rtc[5] << 8) + Rtc[6]));
											i = 0x00;
											Year = 0x00;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														Rtc[5] = (Year >> 8);
														Rtc[6] = Year;
														API_RTC_Write((void *)Rtc);
													}
													Lev = 0x01;
													break;
												}
												else
												{
													if((key >= '0') && (key <= '9'))
													{
														if(i < 0x03)
														{
															if(i == 0x00)
																Year = 0x00;
															API_LCM_ClearArea(0,3,239,4);
															Year = Year * 10 + (key - 0x30);
															API_LCM_Printf(16,3,0,1,DevConfig.RtcYear[VMCParam.Language],Year);
															i++;
														}
														else
														{
															API_LCM_ClearArea(0,3,239,4);
															Year = Year * 10 + (key - 0x30);
															API_LCM_Printf(16,3,0,1,DevConfig.RtcYear[VMCParam.Language],Year);
															i = 0x00;
														}
													}
												}
												vTaskDelay(20);
											}
											break;
										case 'B':
											API_LCM_ClearArea(0,5,239,6);
											Mon = 0x00;
											API_LCM_Printf(16,5,0,1,DevConfig.RtcMon[VMCParam.Language], Rtc[4]);
											i = 0x00;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if((Mon <= 12) && (Mon >= 1))
														{
															Rtc[4] = Mon;
															API_RTC_Write((void *)Rtc);
														}
													}
													Lev = 0x01;
													break;
												}
												else
												{
													if((key >= '0') && (key <= '9'))
													{
														if(i == 0x00)
														{
															Mon = 0x00;
															API_LCM_ClearArea(0,5,239,6);
															Mon = (key - 0x30);
															API_LCM_Printf(16,5,0,1,DevConfig.RtcMon[VMCParam.Language],Mon);
															i++;
														}
														else
														{
															API_LCM_ClearArea(0,5,239,6);
															Mon = Mon * 10 + (key - 0x30);
															API_LCM_Printf(16,5,0,1,DevConfig.RtcMon[VMCParam.Language],Mon);
															i = 0x00;
														}
													}
												}
												vTaskDelay(20);
											}									
											break;
										case 'C':
											API_LCM_ClearArea(0,7,239,8);
											Day = 0x00;
											API_LCM_Printf(16,7,0,1,DevConfig.RtcDay[VMCParam.Language],Rtc[3]);
											i = 0x00;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if((Day <= 31) && (Day >= 1))
														{
															Rtc[3] = Day;
															API_RTC_Write((void *)Rtc);
														}
													}
													Lev = 0x01;
													break;
												}
												else
												{
													if((key >= '0') && (key <= '9'))
													{
														if(i == 0x00)
														{
															Day = 0x00;
															API_LCM_ClearArea(0,7,239,8);
															Day = (key - 0x30);
															API_LCM_Printf(16,7,0,1,DevConfig.RtcDay[VMCParam.Language],Day);
															i++;
														}
														else
														{
															API_LCM_ClearArea(0,7,239,8);
															Day = Day * 10 + (key - 0x30);
															API_LCM_Printf(16,7,0,1,DevConfig.RtcDay[VMCParam.Language],Day);
															i = 0x00;
														}
													}
												}
												vTaskDelay(20);
											}									
											break;
										case 'D':
											API_LCM_ClearArea(0,9,239,10);
											Hour = 0x00;
											API_LCM_Printf(16,9,0,1,DevConfig.RtcHour[VMCParam.Language],Rtc[2]);
											i = 0x00;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if((Hour <= 23) && (Hour >= 1))
														{
															Rtc[2] = Hour;
															API_RTC_Write((void *)Rtc);
														}
													}
													Lev = 0x01;
													break;
												}
												else
												{
													if((key >= '0') && (key <= '9'))
													{
														if(i == 0x00)
														{
															Hour = 0x00;
															API_LCM_ClearArea(0,9,239,10);
															Hour = (key - 0x30);
															API_LCM_Printf(16,9,0,1,DevConfig.RtcHour[VMCParam.Language],Hour);
															i++;
														}
														else
														{
															API_LCM_ClearArea(0,9,239,10);
															Hour = Hour * 10 + (key - 0x30);
															API_LCM_Printf(16,9,0,1,DevConfig.RtcHour[VMCParam.Language],Hour);
															i = 0x00;
														}
													}
												}
												vTaskDelay(20);
											}									
											break;
										case 'E':
											API_LCM_ClearArea(0,11,239,12);
											Min = 0x00;
											API_LCM_Printf(16,11,0,1,DevConfig.RtcMin[VMCParam.Language],Rtc[1]);
											i = 0x00;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(Min <= 59)
														{
															Rtc[1] = Min;
															API_RTC_Write((void *)Rtc);
														}
													}
													Lev = 0x01;
													break;
												}
												else
												{
													if((key >= '0') && (key <= '9'))
													{
														if(i == 0x00)
														{
															Min = 0x00;
															API_LCM_ClearArea(0,11,239,12);
															Min = (key - 0x30);
															API_LCM_Printf(16,11,0,1,DevConfig.RtcMin[VMCParam.Language],Min);
															i++;
														}
														else
														{
															API_LCM_ClearArea(0,11,239,12);
															Min = Min * 10 + (key - 0x30);
															API_LCM_Printf(16,11,0,1,DevConfig.RtcMin[VMCParam.Language],Min);
															i = 0x00;
														}
													}
												}
												vTaskDelay(20);
											}									
											break;							
										default:
											break;
									}
								}
								vTaskDelay(20);
								if(Lev == 0x01)
									break;
							}
							if(Back == 0x01)
								break;
						}
						break;
					default:
						break;
				}					
			}
			if(Back == 0x01)
				break;
			vTaskDelay(20);
		}
		if(Exit)
			break;
		vTaskDelay(20);
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   MaintenDevTest/设备测试
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void MaintenDevTest(void)
{
	unsigned char key,Exit = 0x00,Back = 0x00;
	while(1)
	{
		Back = 0x00;
		API_LCM_ClearScreen();
		API_LCM_Printf(0,0,0,0,DevTest.Theme[VMCParam.Language]);
		API_LCM_DrawLine(0,2);
		API_LCM_Printf(16,3,0,0,DevTest.TBill[VMCParam.Language]);
		API_LCM_Printf(16,5,0,0,DevTest.TCoin[VMCParam.Language]);
		API_LCM_Printf(16,7,0,0,DevTest.TCashless[VMCParam.Language]);
		API_LCM_Printf(16,9,0,0,DevTest.TGdsChnl[VMCParam.Language]);
		API_LCM_Printf(0,14,0,0,DevTest.Exit[VMCParam.Language]);
		while(1)
		{
			key = API_KEY_ReadKey();
			if(key == '>')
			{
				Exit = 0x01;
				break;
			}
			else
			{
				switch(key)
				{
					case 'A':
						//纸币器测试
						TestBillValidator();	
						API_LCM_ClearScreen();
						API_LCM_Printf(0,0,0,0,DevTest.Theme[VMCParam.Language]);
						API_LCM_DrawLine(0,2);
						API_LCM_Printf(16,3,0,0,DevTest.TBill[VMCParam.Language]);
						API_LCM_Printf(16,5,0,0,DevTest.TCoin[VMCParam.Language]);
						API_LCM_Printf(16,7,0,0,DevTest.TCashless[VMCParam.Language]);
						API_LCM_Printf(16,9,0,0,DevTest.TGdsChnl[VMCParam.Language]);
						API_LCM_Printf(0,14,0,0,DevTest.Exit[VMCParam.Language]);
						break;
					case 'B':
						//硬币器测试
						TestCoinValidator();
						API_LCM_ClearScreen();
						API_LCM_Printf(0,0,0,0,DevTest.Theme[VMCParam.Language]);
						API_LCM_DrawLine(0,2);
						API_LCM_Printf(16,3,0,0,DevTest.TBill[VMCParam.Language]);
						API_LCM_Printf(16,5,0,0,DevTest.TCoin[VMCParam.Language]);
						API_LCM_Printf(16,7,0,0,DevTest.TCashless[VMCParam.Language]);
						API_LCM_Printf(16,9,0,0,DevTest.TGdsChnl[VMCParam.Language]);
						API_LCM_Printf(0,14,0,0,DevTest.Exit[VMCParam.Language]);
						break;
					case 'C':
						//读卡器测试
						TestCoinValidator();
						API_LCM_ClearScreen();
						API_LCM_Printf(0,0,0,0,DevTest.Theme[VMCParam.Language]);
						API_LCM_DrawLine(0,2);
						API_LCM_Printf(16,3,0,0,DevTest.TBill[VMCParam.Language]);
						API_LCM_Printf(16,5,0,0,DevTest.TCoin[VMCParam.Language]);
						API_LCM_Printf(16,7,0,0,DevTest.TCashless[VMCParam.Language]);
						API_LCM_Printf(16,9,0,0,DevTest.TGdsChnl[VMCParam.Language]);
						API_LCM_Printf(0,14,0,0,DevTest.Exit[VMCParam.Language]);
						break;
					case 'D':
						//货道测试
						TestColumn();
						API_LCM_ClearScreen();
						API_LCM_Printf(0,0,0,0,DevTest.Theme[VMCParam.Language]);
						API_LCM_DrawLine(0,2);
						API_LCM_Printf(16,3,0,0,DevTest.TBill[VMCParam.Language]);
						API_LCM_Printf(16,5,0,0,DevTest.TCoin[VMCParam.Language]);
						API_LCM_Printf(16,7,0,0,DevTest.TCashless[VMCParam.Language]);
						API_LCM_Printf(16,9,0,0,DevTest.TGdsChnl[VMCParam.Language]);
						API_LCM_Printf(0,14,0,0,DevTest.Exit[VMCParam.Language]);
						break;
					default:
						break;
				}					
			}
			if(Back == 0x01)
				break;
			vTaskDelay(20);
		}
		if(Exit)
			break;
		vTaskDelay(20);
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   TestBillValidator/纸币器测试
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void TestBillValidator(void)
{
	unsigned char key;
	uint8_t  billOptBack = 0;
	uint32_t InValue = 0,billEscrow=0;
	uint8_t Billtype = 0;
	uint32_t SumValue = 0;
	char    *pstr;
	char	strMoney[10];
	
	API_LCM_Printf(0,0,0,0,BillTest.Theme[VMCParam.Language]);
	API_LCM_ClearArea(0,3,239,15);
	API_LCM_Printf(0,14,0,0,BillTest.Exit[VMCParam.Language]);
	API_LCM_Printf((120 - (strlen(BillTest.Bill[VMCParam.Language]) /2 ) * 8),3,0,0,BillTest.Bill[VMCParam.Language]);
	BillDevProcess(&InValue,&Billtype,MBOX_BILLENABLEDEV,&billOptBack);
	while(1)
	{
		vTaskDelay(20);
		BillDevProcess(&InValue,&Billtype,0,&billOptBack);
		if(InValue>0)
		{
			Trace("\r\n APP>>Billvalue=%d\r\n",InValue);
			BillDevProcess(&billEscrow,&Billtype,MBOX_BILLESCROW,&billOptBack);
			if(billOptBack==2)
			{
				Trace("\r\n TaskEscrowSuccess=%d",InValue);			
				SumValue += InValue;
				pstr = PrintfMoney(SumValue);
				strcpy(strMoney, pstr);
				API_LCM_Printf(0,6,0,0,BillTest.BillMsg6[VMCParam.Language],strMoney);
				InValue=0;
				billOptBack=0;
			}
			else
			{
				Trace("\r\n TaskEscrowFail");
				InValue=0;
				billOptBack=0;
			}
		}
		key = API_KEY_ReadKey();
		if(key == '>')//取消按键
		{
			break;						
		}		
	}
	BillDevProcess(&InValue,&Billtype,MBOX_BILLDISABLEDEV,&billOptBack);
}
/************************************************************************************************************************************************************************************
** @APP Function name:   TestBillValidator/硬币器测试
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void TestCoinValidator(void)
{
	unsigned char key;
	uint8_t  billOptBack = 0;
	uint32_t InValue = 0;
	uint32_t OutValue = 0;
	uint8_t Billtype = 0;
	uint32_t SumValue = 0;
	char    *pstr;
	char	strMoney[10];
	API_LCM_Printf(0,0,0,0,CoinTest.Theme[VMCParam.Language]);
	API_LCM_ClearArea(0,3,239,15);
	API_LCM_Printf(0,14,0,0,CoinTest.Exit[VMCParam.Language]);
	API_LCM_Printf((120 - (strlen(CoinTest.Coin[VMCParam.Language]) /2 ) * 8),3,0,0,CoinTest.Coin[VMCParam.Language]);
	MdbCoinTypeEanbleOrDisable(1,1);
	while(1)
	{
		vTaskDelay(20);
		CoinDevProcess(&InValue,&Billtype,&billOptBack);
		if(InValue>0)
		{
			Trace("\r\n APP>>Coinvalue=%d\r\n",InValue);
			SumValue += InValue;
			pstr = PrintfMoney(SumValue);
			strcpy(strMoney, pstr);
			API_LCM_Printf(0,6,0,0,CoinTest.CoinMsg6[VMCParam.Language],strMoney);
			InValue=0;
			billOptBack=0;
		}
		key = API_KEY_ReadKey();
		
		if(key == '>')//取消按键
		{
			break;						
		}
		else
		{
			switch(key)
			{
				case '1':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0]),&OutValue);					
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);					
					break;
				case '2':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*2,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;
				case '3':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*3,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;
				case '4':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*4,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;
				case '5':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*5,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;
				case '6':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*6,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;
				case '7':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*7,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;
				case '8':
					ChangePayoutProcessLevel3(MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[0])*8,&OutValue);	
					Trace("\r\n APP>>Coinpay=%d\r\n",OutValue);	
					break;	
					
			}
			if(OutValue>0)
			{
				pstr = PrintfMoney(OutValue);
				strcpy(strMoney, pstr);
				API_LCM_Printf(0,6,0,0,CoinTest.CoinMsg7[VMCParam.Language],strMoney);
				OutValue=0;
			}
		}
	}
	MdbCoinTypeEanbleOrDisable(0,1);
}
/************************************************************************************************************************************************************************************
** @APP Function name:   TestColumn/货道测试
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void TestColumn(void)
{
	unsigned char layerOK = 0x00,dataOK = 0x00,key;
	char Layer,Channel;
	API_LCM_ClearScreen();
	API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumb[VMCParam.Language]);
	API_LCM_Printf(0,14,0,0,TradeConfigGC.Exit[VMCParam.Language]);							
	vTaskDelay(20);
	Layer = 0x00;
	Channel = 0x00;
	layerOK = 0x00;
	dataOK = 0x00;
	while(1)
	{
		key = API_KEY_ReadKey();
		if(key == '>')
		{
			break;
		}
		else
		{
			if(layerOK == 0x00)
			{
				if((key >= 'A') && (key <= 'F'))
				{
					layerOK = 0x01;
					Layer = key;
					API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumbL[VMCParam.Language],Layer);
				}
				else
					dataOK = 0x00;
			}
			else
			{
				if((key >= '1') && (key <= '8'))
				{
					Channel = key;
					API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumbLC[VMCParam.Language],Layer,(Channel - 0x30));
					dataOK = 0x01;
				}
				else
					dataOK = 0x00;
			}
		}
		if(dataOK == 0x01)
		{
			dataOK = 0x00;
			layerOK = 0x00;
			API_KEY_KeyboardCtrl(0x00);	
			API_VENDING_Vend(Layer,Channel);
			API_KEY_KeyboardCtrl(0x01);	
			Layer = 0x00;
			Channel = 0x00;
			API_LCM_ClearScreen();
			API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumb[VMCParam.Language]);
			API_LCM_Printf(0,14,0,0,TradeConfigGC.Exit[VMCParam.Language]);
		}
	}
		
}
/************************************************************************************************************************************************************************************
** @APP Function name:   MaintenTradeConfig/交易设置
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void MaintenTradeConfig(void)
{
	unsigned char key,Lev0 = 0x00,Lev1 = 0x00,Lev2 = 0x00,Lev3 = 0x01,Array;
	unsigned char layer = 0x00,channel = 0x00,layerOK = 0x00,MxCapat,dataOK = 0x00;
	unsigned char i,Pri[5],Index = 0x00,cNumb;
	unsigned short ConfigTemp;
	unsigned int Price = 0x00;
	while(1)//交易配置主界面
	{
		Lev0 = 0x00;
		API_LCM_ClearScreen();
		API_LCM_Printf(0,0,0,0,TradeConfig.Theme[VMCParam.Language]);
		API_LCM_DrawLine(0,2);
		API_LCM_Printf(15,3,0,0,TradeConfig.GdsChnl[VMCParam.Language]);
		API_LCM_Printf(15,5,0,0,TradeConfig.BillEscrow[VMCParam.Language]);
		API_LCM_Printf(15,7,0,0,TradeConfig.BillType[VMCParam.Language]);
		API_LCM_Printf(15,9,0,0,TradeConfig.CoinType[VMCParam.Language]);
		API_LCM_Printf(15,11,0,0,TradeConfig.AddGoods[VMCParam.Language]);
		API_LCM_Printf(0,14,0,0,TradeConfig.Exit[VMCParam.Language]);
		Lev1 = 0x00;
		while(1)
		{
			key = API_KEY_ReadKey();
			if(key == '>')
			{
				Lev0 = 0x01;
				break;
			}
			else
			{
				switch(key)
				{
					case 'A':	//货道配置界面,并等待输入货道号
						while(1)
						{
							Lev2 = 0x00;
							API_LCM_ClearScreen();
							API_LCM_Printf(0,0,0,0,TradeConfigGC.Theme[VMCParam.Language]);
							API_LCM_DrawLine(0,2);
							API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumb[VMCParam.Language]);
							API_LCM_Printf(0,14,0,0,TradeConfigGC.Exit[VMCParam.Language]);							
							vTaskDelay(20);
							layer = 0x00;
							channel = 0x00;
							layerOK = 0x00;
							dataOK = 0x00;
							while(1)//货道配置
							{
								key = API_KEY_ReadKey();
								if(key == '>')
								{
									Lev1 = 0x01;
									break;
								}
								else
								{
									if(layerOK == 0x00)
									{
										if((key >= 'A') && (key <= 'F'))
										{
											layerOK = 0x01;
											layer = key;
											API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumbL[VMCParam.Language],layer);
										}
										else
											dataOK = 0x00;
									}
									else
									{
										if((key >= '1') && (key <= '8'))
										{
											channel = key - 0x30;
											API_LCM_Printf(15,3,0,0,TradeConfigGC.GdNumbLC[VMCParam.Language],layer,channel);
											dataOK = 0x01;
										}
										else
											dataOK = 0x00;
									}
								}
								if(dataOK == 0x01)
								{
									while(1)
									{	
										dataOK = 0x00;
										Array = ((layer - 0x41)*8 + (channel - 1));
										API_LCM_ClearArea(0,3,239,4);
										API_LCM_Printf(15,3,0,0,TradeConfigGC.GdCoinfig[VMCParam.Language],layer,channel);
										API_LCM_ClearArea(0,6,239,9);
										API_LCM_Printf(15,6,0,0,TradeConfigGC.Enale[VMCParam.Language],VMCParam.GoodsChannelArray[Array]);
										API_LCM_Printf(15,8,0,0,TradeConfigGC.MaxCap[VMCParam.Language],VMCParam.GoodsMaxCapacity[Array]);
										API_LCM_Printf(15,10,0,0,TradeConfigGC.Price[VMCParam.Language],TradeParam.GoodsPrice[Array]/10,TradeParam.GoodsPrice[Array]%10);
										while(1)
										{
											vTaskDelay(20);
											Lev3 = 0x00;
											key = API_KEY_ReadKey();
											if(key == '>')
											{
												Lev2 = 0x01;
												break;
											}
											else
											{
												switch(key)
												{
													case 'A'://货道使能
														API_LCM_ClearArea(0,6,239,11);
														API_LCM_Printf(15,6,0,1,TradeConfigGC.Enale[VMCParam.Language],VMCParam.GoodsChannelArray[Array]);
														API_LCM_Printf(15,8,0,0,TradeConfigGC.MaxCap[VMCParam.Language],VMCParam.GoodsMaxCapacity[Array]);
														API_LCM_Printf(15,10,0,0,TradeConfigGC.Price[VMCParam.Language],TradeParam.GoodsPrice[Array]/10,TradeParam.GoodsPrice[Array]%10);
														while(1)
														{
															key = API_KEY_ReadKey();
															if((key == '<') || (key == '>'))
															{
																Lev3 = 0x01;
																if(key == '<')
																	LoadNewVmcParam();
																break;
															}
															else
															{
																switch(key)
																{
																	case '0':
																		VMCParam.GoodsChannelArray[Array] = 0x00;
																		API_LCM_Printf(15,6,0,1,TradeConfigGC.Enale[VMCParam.Language],VMCParam.GoodsChannelArray[Array]);
																		break;
																	case '1':
																		VMCParam.GoodsChannelArray[Array] = 0x01;
																		API_LCM_Printf(15,6,0,1,TradeConfigGC.Enale[VMCParam.Language],VMCParam.GoodsChannelArray[Array]);
																		break;
																	default:
																		break;
																}
															}
															vTaskDelay(20);
														}
														break;
													case 'B'://最大存货量
														MxCapat = VMCParam.GoodsMaxCapacity[Array];
														API_LCM_ClearArea(0,6,239,11);
														API_LCM_Printf(15,6,0,0,TradeConfigGC.Enale[VMCParam.Language],VMCParam.GoodsChannelArray[Array]);
														API_LCM_Printf(15,8,0,1,TradeConfigGC.MaxCap[VMCParam.Language],VMCParam.GoodsMaxCapacity[Array]);
														API_LCM_Printf(15,10,0,0,TradeConfigGC.Price[VMCParam.Language],TradeParam.GoodsPrice[Array]/10,TradeParam.GoodsPrice[Array]%10);
														while(1)
														{
															key = API_KEY_ReadKey();
															if((key == '<') || (key == '>'))
															{
																Lev3 = 0x01;
																if(key == '<')
																{
																	if(VMCParam.GoodsMaxCapacity[Array] != MxCapat)
																	{
																		VMCParam.GoodsMaxCapacity[Array] = MxCapat;
																		LoadNewVmcParam();
																	}
																}
																break;
															}
															else
															{
																if(dataOK == 0x00)
																{
																	if((key > '0') && (key <= '9'))
																	{
																		MxCapat = 0x00;
																		dataOK = 0x01;
																		MxCapat = (key - 0x30);
																		API_LCM_ClearArea(0,8,239,9);
																		API_LCM_Printf(15,8,0,1,TradeConfigGC.MaxCap[VMCParam.Language],MxCapat);	
																	}
																}
																else
																{
																	if((key >= '0') && (key <= '9'))
																	{
																		dataOK = 0x00;
																		MxCapat = MxCapat*10 + (key - 0x30);
																		API_LCM_ClearArea(0,8,239,9);
																		API_LCM_Printf(15,8,0,1,TradeConfigGC.MaxCap[VMCParam.Language],MxCapat);
																	}															
																}
															}
															vTaskDelay(20);
														}											
														break;
													case 'C'://货道价格
														API_LCM_ClearArea(0,6,239,11);
														API_LCM_Printf(15,6,0,0,TradeConfigGC.Enale[VMCParam.Language],VMCParam.GoodsChannelArray[Array]);
														API_LCM_Printf(15,8,0,0,TradeConfigGC.MaxCap[VMCParam.Language],VMCParam.GoodsMaxCapacity[Array]);
														API_LCM_Printf(15,10,0,1,TradeConfigGC.Price[VMCParam.Language],TradeParam.GoodsPrice[Array]/10,TradeParam.GoodsPrice[Array]%10);
														Index = 0x00;
														Price = 0x00;
														memset(Pri,0x00,5);
														vTaskDelay(20);
														while(1)
														{
															vTaskDelay(20);
															key = API_KEY_ReadKey();
															if((key == '>') || (key == '<'))
															{
																if(key == '<')
																{
																	if(TradeParam.GoodsPrice[Array] != Price)
																	{
																		TradeParam.GoodsPrice[Array] = Price;
																		LoadNewTradeParam();
																	}
																}
																Lev3 = 0x01;
																break;
															}
															else
															{
																if((key >= '0') && (key <= '9'))
																{
																	if(Index < 5)
																	{
																		for(i=0;i<4;i++)
																		{
																			Pri[4-i] = Pri[3-i];
																		}
																		Pri[0] = key - 0x30;
																		Index++;
																		Price = (Pri[4] * 10000 + Pri[3] * 1000 + Pri[2] * 100 + Pri[1] * 10 + Pri[0]);
																		API_LCM_ClearArea(0,10,239,11);
																		API_LCM_Printf(15,10,0,1,TradeConfigGC.Price[VMCParam.Language],Price/10,Price%10);
																	}
																	else
																	{
																		Price = 0x00;
																		for(i=0;i<4;i++)
																		{
																			Pri[i] = 0x00;
																		}
																		API_LCM_ClearArea(0,10,239,11);
																		API_LCM_Printf(15,10,0,1,TradeConfigGC.Price[VMCParam.Language],0,0);											
																		Index = 0x00;
																	}
																}
															}
														}
														break;
													default:
														break;
												}
											}
											if(Lev2 == 0x01)
												break;
											if(Lev3 == 0x01)
												break;
										}
										if(Lev2 == 0x01)
											break;
									}
								}
								if(Lev2 == 0x01)
									break;
							}
							if(Lev1 == 0x01)
								break;
						}
						break;
					case 'B':	//纸币暂存通道设置
						while(1)
						{
							Lev1 = 0x00;
							API_LCM_ClearScreen();
							API_LCM_Printf(0,0,0,0,TradeConfigBillEscrow.Theme[VMCParam.Language]);
							API_LCM_DrawLine(0,2);
							API_LCM_Printf(15,3,0,0,TradeConfigBillEscrow.BillChl[VMCParam.Language]);
							API_LCM_Printf(15,6,0,0,TradeConfigBillEscrow.Ch0[VMCParam.Language],((TradeParam.TypeBillEscrow >> 15) & 0x0001));
							API_LCM_Printf(110,6,0,0,TradeConfigBillEscrow.Ch1[VMCParam.Language],((TradeParam.TypeBillEscrow >> 14) & 0x0001));
							API_LCM_Printf(15,8,0,0,TradeConfigBillEscrow.Ch2[VMCParam.Language],((TradeParam.TypeBillEscrow >> 13) & 0x0001));
							API_LCM_Printf(110,8,0,0,TradeConfigBillEscrow.Ch3[VMCParam.Language],((TradeParam.TypeBillEscrow >> 12) & 0x0001));
							API_LCM_Printf(15,10,0,0,TradeConfigBillEscrow.Ch4[VMCParam.Language],((TradeParam.TypeBillEscrow >> 11) & 0x0001));
							API_LCM_Printf(110,10,0,0,TradeConfigBillEscrow.Ch5[VMCParam.Language],((TradeParam.TypeBillEscrow >> 10) & 0x0001));
							API_LCM_Printf(0,14,0,0,TradeConfigBillEscrow.Exit[VMCParam.Language]);
							Lev2 = 0x00;
							while(1)
							{
								key = API_KEY_ReadKey();
								if(key == '>')
								{
									Lev1 = 0x01;
									break;
								}
								else
								{
									switch(key)
									{
										case 'A':
											API_LCM_Printf(15,6,0,1,TradeConfigBillEscrow.Ch0[VMCParam.Language],((TradeParam.TypeBillEscrow >> 15) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEscrow;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEscrow)
														{
															TradeParam.TypeBillEscrow = ConfigTemp;
															TradeParam.TypeBillEscrow &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0x7FFF;
															API_LCM_Printf(15,6,0,1,TradeConfigBillEscrow.Ch0[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp = 0x8000;
															API_LCM_Printf(15,6,0,1,TradeConfigBillEscrow.Ch0[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}
											break;
										case 'B':
											API_LCM_Printf(110,6,0,1,TradeConfigBillEscrow.Ch1[VMCParam.Language],((TradeParam.TypeBillEscrow >> 14) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEscrow)
														{
															TradeParam.TypeBillEscrow = ConfigTemp;
															TradeParam.TypeBillEscrow &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xBFFF;
															API_LCM_Printf(110,6,0,1,TradeConfigBillEscrow.Ch1[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp = 0x4000;
															API_LCM_Printf(110,6,0,1,TradeConfigBillEscrow.Ch1[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'C':
											API_LCM_Printf(15,8,0,1,TradeConfigBillEscrow.Ch2[VMCParam.Language],((TradeParam.TypeBillEscrow >> 13) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEscrow;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEscrow)
														{
															TradeParam.TypeBillEscrow = ConfigTemp;
															TradeParam.TypeBillEscrow &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xDFFF;
															API_LCM_Printf(15,8,0,1,TradeConfigBillEscrow.Ch2[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp = 0x2000;
															API_LCM_Printf(15,8,0,1,TradeConfigBillEscrow.Ch2[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}
											break;
										case 'D':
											API_LCM_Printf(110,8,0,1,TradeConfigBillEscrow.Ch3[VMCParam.Language],((TradeParam.TypeBillEscrow >> 12) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEscrow;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEscrow)
														{
															TradeParam.TypeBillEscrow = ConfigTemp;
															TradeParam.TypeBillEscrow &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xEFFF;
															API_LCM_Printf(110,8,0,1,TradeConfigBillEscrow.Ch3[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp = 0x1000;
															API_LCM_Printf(110,8,0,1,TradeConfigBillEscrow.Ch3[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'E':
											API_LCM_Printf(15,10,0,1,TradeConfigBillEscrow.Ch4[VMCParam.Language],((TradeParam.TypeBillEscrow >> 11) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEscrow;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEscrow)
														{
															TradeParam.TypeBillEscrow = ConfigTemp;
															TradeParam.TypeBillEscrow &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xF7FF;
															API_LCM_Printf(15,10,0,1,TradeConfigBillEscrow.Ch4[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp = 0x0800;
															API_LCM_Printf(15,10,0,1,TradeConfigBillEscrow.Ch4[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'F':
											API_LCM_Printf(110,10,0,1,TradeConfigBillEscrow.Ch5[VMCParam.Language],((TradeParam.TypeBillEscrow >> 10) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEscrow;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEscrow = ConfigTemp;
															TradeParam.TypeBillEscrow &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xFBFF;
															API_LCM_Printf(110,10,0,1,TradeConfigBillEscrow.Ch5[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp = 0x0400;
															API_LCM_Printf(110,10,0,1,TradeConfigBillEscrow.Ch5[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										default:
											break;
									}
								}
								if(Lev2 == 0x01)
									break;
							}
							if(Lev1 == 0x01)
								break;
						}
						break;
					case 'C':	//纸币通道开关
						while(1)
						{
							Lev1 = 0x00;
							API_LCM_ClearScreen();
							API_LCM_Printf(0,0,0,0,TradeConfigBillChannel.Theme[VMCParam.Language]);
							API_LCM_DrawLine(0,2);
							API_LCM_Printf(15,3,0,0,TradeConfigBillChannel.BillChl[VMCParam.Language]);
							API_LCM_Printf(15,6,0,0,TradeConfigBillChannel.Ch0[VMCParam.Language],((TradeParam.TypeBillEnable >> 15) & 0x0001));
							API_LCM_Printf(110,6,0,0,TradeConfigBillChannel.Ch1[VMCParam.Language],((TradeParam.TypeBillEnable >> 14) & 0x0001));
							API_LCM_Printf(15,8,0,0,TradeConfigBillChannel.Ch2[VMCParam.Language],((TradeParam.TypeBillEnable >> 13) & 0x0001));
							API_LCM_Printf(110,8,0,0,TradeConfigBillChannel.Ch3[VMCParam.Language],((TradeParam.TypeBillEnable >> 12) & 0x0001));
							API_LCM_Printf(15,10,0,0,TradeConfigBillChannel.Ch4[VMCParam.Language],((TradeParam.TypeBillEnable >> 11) & 0x0001));
							API_LCM_Printf(110,10,0,0,TradeConfigBillChannel.Ch5[VMCParam.Language],((TradeParam.TypeBillEnable >> 10) & 0x0001));
							API_LCM_Printf(0,14,0,0,TradeConfigBillChannel.Exit[VMCParam.Language]);
							Lev2 = 0x00;
							while(1)
							{
								key = API_KEY_ReadKey();
								if(key == '>')
								{
									Lev1 = 0x01;
									break;
								}
								else
								{
									switch(key)
									{
										case 'A':
											API_LCM_Printf(15,6,0,1,TradeConfigBillChannel.Ch0[VMCParam.Language],((TradeParam.TypeBillEnable >> 15) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEnable = ConfigTemp;
															TradeParam.TypeBillEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0x7FFF;
															API_LCM_Printf(15,6,0,1,TradeConfigBillChannel.Ch0[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x8000;
															API_LCM_Printf(15,6,0,1,TradeConfigBillChannel.Ch0[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}
											break;
										case 'B':
											API_LCM_Printf(110,6,0,1,TradeConfigBillChannel.Ch1[VMCParam.Language],((TradeParam.TypeBillEnable >> 14) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEnable = ConfigTemp;
															TradeParam.TypeBillEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xBFFF;
															API_LCM_Printf(110,6,0,1,TradeConfigBillChannel.Ch1[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x4000;
															API_LCM_Printf(110,6,0,1,TradeConfigBillChannel.Ch1[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'C':
											API_LCM_Printf(15,8,0,1,TradeConfigBillChannel.Ch2[VMCParam.Language],((TradeParam.TypeBillEnable >> 13) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEnable = ConfigTemp;
															TradeParam.TypeBillEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xDFFF;
															API_LCM_Printf(15,8,0,1,TradeConfigBillChannel.Ch2[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x2000;
															API_LCM_Printf(15,8,0,1,TradeConfigBillChannel.Ch2[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}
											break;
										case 'D':
											API_LCM_Printf(110,8,0,1,TradeConfigBillChannel.Ch3[VMCParam.Language],((TradeParam.TypeBillEnable >> 12) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEnable = ConfigTemp;
															TradeParam.TypeBillEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xEFFF;
															API_LCM_Printf(110,8,0,1,TradeConfigBillChannel.Ch3[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x1000;
															API_LCM_Printf(110,8,0,1,TradeConfigBillChannel.Ch3[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'E':
											API_LCM_Printf(15,10,0,1,TradeConfigBillChannel.Ch4[VMCParam.Language],((TradeParam.TypeBillEnable >> 11) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEnable = ConfigTemp;
															TradeParam.TypeBillEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xF7FF;
															API_LCM_Printf(15,10,0,1,TradeConfigBillChannel.Ch4[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x0800;
															API_LCM_Printf(15,10,0,1,TradeConfigBillChannel.Ch4[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'F':
											API_LCM_Printf(110,10,0,1,TradeConfigBillChannel.Ch5[VMCParam.Language],((TradeParam.TypeBillEnable >> 10) & 0x0001));
											ConfigTemp = TradeParam.TypeBillEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeBillEnable)
														{
															TradeParam.TypeBillEnable = ConfigTemp;
															TradeParam.TypeBillEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xFBFF;
															API_LCM_Printf(110,10,0,1,TradeConfigBillChannel.Ch5[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x0400;
															API_LCM_Printf(110,10,0,1,TradeConfigBillChannel.Ch5[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										default:
											break;
									}
								}
								if(Lev2 == 0x01)
									break;
							}
							if(Lev1 == 0x01)
								break;
						}
						break;
					case 'D'://硬币通道开关
						while(1)
						{
							Lev1 = 0x00;
							API_LCM_ClearScreen();
							API_LCM_Printf(0,0,0,0,TradeConfigCoinChannel.Theme[VMCParam.Language]);
							API_LCM_DrawLine(0,2);
							API_LCM_Printf(15,3,0,0,TradeConfigCoinChannel.CoinChl[VMCParam.Language]);
							API_LCM_Printf(15,6,0,0,TradeConfigCoinChannel.Chl0[VMCParam.Language],((TradeParam.TypeCoinEnable >> 15) & 0x0001));
							API_LCM_Printf(15,8,0,0,TradeConfigCoinChannel.Chl1[VMCParam.Language],((TradeParam.TypeCoinEnable >> 14) & 0x0001));
							API_LCM_Printf(15,10,0,0,TradeConfigCoinChannel.Chl2[VMCParam.Language],((TradeParam.TypeCoinEnable >> 13) & 0x0001));
							API_LCM_Printf(0,14,0,0,TradeConfigCoinChannel.Exit[VMCParam.Language]);							
							vTaskDelay(20);
							Lev2 = 0x00;
							while(1)
							{
								key = API_KEY_ReadKey();
								if(key == '>')
								{
									Lev1 = 0x01;
									break;
								}
								else
								{
									switch(key)
									{
										case 'A':
											API_LCM_Printf(15,6,0,1,TradeConfigCoinChannel.Chl0[VMCParam.Language],((TradeParam.TypeCoinEnable >> 15) & 0x0001));
											ConfigTemp = TradeParam.TypeCoinEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeCoinEnable)
														{
															TradeParam.TypeCoinEnable = ConfigTemp;
															TradeParam.TypeCoinEnable &= 0xE000;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0x7000;
															API_LCM_Printf(15,6,0,1,TradeConfigCoinChannel.Chl0[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x8000;
															API_LCM_Printf(15,6,0,1,TradeConfigCoinChannel.Chl0[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}
											break;
										case 'B':
											API_LCM_Printf(15,8,0,1,TradeConfigCoinChannel.Chl1[VMCParam.Language],((TradeParam.TypeCoinEnable >> 14) & 0x0001));
											ConfigTemp = TradeParam.TypeCoinEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeCoinEnable)
														{
															TradeParam.TypeCoinEnable = ConfigTemp;
															TradeParam.TypeCoinEnable &= 0xFC00;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xB000;
															API_LCM_Printf(15,8,0,1,TradeConfigCoinChannel.Chl1[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x4000;
															API_LCM_Printf(15,8,0,1,TradeConfigCoinChannel.Chl1[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;
										case 'C':
											API_LCM_Printf(15,10,0,1,TradeConfigCoinChannel.Chl2[VMCParam.Language],((TradeParam.TypeCoinEnable >> 13) & 0x0001));
											ConfigTemp = TradeParam.TypeCoinEnable;
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == '<') || (key == '>'))
												{
													if(key == '<')
													{
														if(ConfigTemp != TradeParam.TypeCoinEnable)
														{
															TradeParam.TypeCoinEnable = ConfigTemp;
															TradeParam.TypeCoinEnable &= 0xE000;
															LoadNewTradeParam();
														}
													}
													Lev2 = 0x01;
													break;
												}
												else
												{
													switch(key)
													{
														case '0':
															ConfigTemp &= 0xD000;
															API_LCM_Printf(15,10,0,1,TradeConfigCoinChannel.Chl2[VMCParam.Language],0);
															break;
														case '1':
															ConfigTemp |= 0x2000;
															API_LCM_Printf(15,10,0,1,TradeConfigCoinChannel.Chl2[VMCParam.Language],1);
															break;
														default:
															break;
													}
												}
												vTaskDelay(20);
											}											
											break;	
										default:
											break;
									}
								}
								if(Lev2 == 0x01)
									break;
							}
							if(Lev1 == 0x01)
								break;
						}
						break;
					case 'E'://添货补货
						while(1)
						{
							Lev1 = 0x00;
							API_LCM_ClearScreen();
							API_LCM_Printf(0,0,0,0,TradeConfigAddGoods.Theme[VMCParam.Language]);
							API_LCM_DrawLine(0,2);
							API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.All[VMCParam.Language]);
							API_LCM_Printf(15,6,0,0,TradeConfigAddGoods.Layer[VMCParam.Language]);
							API_LCM_Printf(15,8,0,0,TradeConfigAddGoods.Channel[VMCParam.Language]);
							API_LCM_Printf(0,14,0,0,TradeConfigAddGoods.Exit[VMCParam.Language]);							
							vTaskDelay(20);
							Lev2 = 0x00;
							Lev3 = 0x00;							
							while(1)
							{
								key = API_KEY_ReadKey();
								if(key == '>')
								{
									Lev1 = 0x01;
									break;
								}
								else
								{
									switch(key)
									{
										case 'A'://全部添满
											API_LCM_ClearArea(0,4,239,9);
											API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.ALL_YON[VMCParam.Language]);
											API_LCM_Printf(31,7,0,0,TradeConfigAddGoods.CancelOrEnter[VMCParam.Language]);
											while(1)
											{
												key = API_KEY_ReadKey();
												if((key == 'A') || (key == '>'))
												{
													Lev2 = 0x01;
													break;
												}
												if(key == 'B')
												{
													//全部添满货道
													for(layer='A';layer<='F';layer++)
													{
														for(channel=1;channel<=8;channel++)
														{
															TradeParam.RemainGoods[(layer - 0x41)*8 + (channel - 1)] = VMCParam.GoodsMaxCapacity[(layer - 0x41)*8 + (channel - 1)];
														}
													}
													API_LCM_Printf(31,10,0,0,"添满货道成功!");
													LoadNewTradeParam();
													vTaskDelay(400);
													Lev2 = 0x01;
													break;
												}
												vTaskDelay(20);
											}
											break;
										case 'B'://按层添满
											API_LCM_ClearArea(0,4,239,9);
											API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterLayer[VMCParam.Language]);
											Lev3 = 0x00;
											layer = 0x00;
											while(1)
											{
												key = API_KEY_ReadKey();
												if(key == '>')
												{
													Lev2 = 0x01;
													break;
												}
												else
												{
													if((key >= 'A') && (key <= 'F'))
													{
														layer = key;
														API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterLayer0[VMCParam.Language],layer);
														while(1)
														{
															key = API_KEY_ReadKey();
															if(key == '>')
															{
																Lev2 = 0x01;
																break;
															}
															if(key == '<')
															{
																API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.AddLayerYON[VMCParam.Language],layer);
																API_LCM_Printf(31,7,0,0,TradeConfigAddGoods.CancelOrEnter[VMCParam.Language]);
																while(1)
																{
																	key = API_KEY_ReadKey();
																	if((key == 'A') || (key == '>'))
																	{
																		Lev2 = 0x01;
																		break;
																	}
																	if(key == 'B')
																	{
																		//填满该层
																		for(channel=1;channel<=8;channel++)
																		{
																			TradeParam.RemainGoods[(layer - 0x41)*8 + (channel - 1)] = VMCParam.GoodsMaxCapacity[(layer - 0x41)*8 + (channel - 1)];
																		}																		
																		API_LCM_Printf(31,10,0,0,"添满%C层成功!",layer);
																		LoadNewTradeParam();
																		vTaskDelay(400);
																		Lev2 = 0x01;
																		break;
																	}
																	vTaskDelay(20);																	
																}
															}
															if(Lev2 == 0x01)
																break;
															vTaskDelay(20);
														}
													}
												}
												vTaskDelay(20);
												if(Lev2 == 0x01)
													break;
											}
											break;
										case 'C'://按单个货道添满
											while(1)
											{
												API_LCM_ClearArea(0,4,239,9);
												API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterChannel[VMCParam.Language]);
												layer = 0x00;
												dataOK = 0x00;
												layerOK = 0x00;
												Lev2 = 0x00;
												Lev3 = 0x00;
												while(1)
												{
													key = API_KEY_ReadKey();
													if(key == '>')
													{
														Lev2 = 0x01;
														break;
													}
													else
													{
														if(layerOK == 0x00)
														{
															if((key >= 'A') && (key <= 'F'))
															{
																layerOK = 0x01;
																layer = key;
																API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterChannel0[VMCParam.Language],layer);
															}
															else
																dataOK = 0x00;
														}
														else
														{
															if((key >= '1') && (key <= '8'))
															{
																channel = key - 0x30;
																API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterChannel00[VMCParam.Language],layer,channel);
																dataOK = 0x01;
															}
															else
																dataOK = 0x00;
														}
													}
													if(dataOK == 0x01)
													{
														Lev3 = 0x00;
														API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterNumb[VMCParam.Language],layer,channel);
														API_LCM_Printf(15,8,0,0,TradeConfigAddGoods.CurrentNumb[VMCParam.Language],layer,channel,TradeParam.RemainGoods[(layer - 0x41)*8 + (channel - 1)]);
														i = 0x00;
														while(1)
														{
															key = API_KEY_ReadKey();
															if((key == '>') || (key == '<'))
															{
																if(key == '<')
																{
																	if(cNumb >= VMCParam.GoodsMaxCapacity[(layer - 0x41)*8 + (channel - 1)])
																	{
																		TradeParam.RemainGoods[(layer - 0x41)*8 + (channel - 1)] = VMCParam.GoodsMaxCapacity[(layer - 0x41)*8 + (channel - 1)];
																		API_LCM_Printf(15,8,0,0,TradeConfigAddGoods.NoteP[VMCParam.Language]);
																	}
																	else
																	{
																		TradeParam.RemainGoods[(layer - 0x41)*8 + (channel - 1)] = cNumb;
																		API_LCM_Printf(15,8,0,0,TradeConfigAddGoods.NoteP[VMCParam.Language]);
																	}
																	LoadNewTradeParam();
																	vTaskDelay(300);
																}
																Lev3 = 0x01;
																break;
															}
															else
															{
																if((key >= '0') && (key <= '9'))
																{
																	if(i == 0x00)
																	{
																		cNumb = key - 0x30;
																		API_LCM_ClearArea(0,4,239,5);
																		API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterNumb0[VMCParam.Language],layer,channel,cNumb);
																		i++;
																	}
																	else if(i == 0x01)
																	{
																		cNumb = (cNumb * 10) + (key - 0x30);
																		API_LCM_ClearArea(0,4,239,5);
																		API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterNumb0[VMCParam.Language],layer,channel,cNumb);
																		i++;
																	}
																	else
																	{
																		i = 0x00;
																		cNumb = key - 0x30;
																		API_LCM_ClearArea(0,4,239,5);
																		API_LCM_Printf(15,4,0,0,TradeConfigAddGoods.EnterNumb0[VMCParam.Language],layer,channel,cNumb);
																		i++;																	
																	}
																}
															}
															vTaskDelay(20);
														}
													}
													if(Lev3 == 0x01)
														break;
													vTaskDelay(20);
												}
												if(Lev2 == 0x01)											
													break;
											}
											break;
										default:
											break;
									}
								}
								vTaskDelay(20);
								if(Lev2 == 0x01)
									break;
							}
							if(Lev1 == 0x01)
								break;
						}
						break;
					default:
						break;
				}					
			}
			if(Lev1 == 0x01)
				break;
		}
		if(Lev0)
			break;
	}
}
/************************************************************************************************************************************************************************************
** @APP Function name:   MaintenTradeLog/交易记录
** @APP Input para:      None
** @APP Output para:     None
** @APP retrun para:     None
************************************************************************************************************************************************************************************/
static void MaintenTradeLog(void)
{
	uint16_t TotalCoinPayIn;
	unsigned char key;

	InitTotalLog();
	TotalCoinPayIn=(TotalTradeLog.TotalCoinPayIn + TotalTradeLog.TotalCashlessPayIn + TotalTradeLog.TotalBillPayIn);
	API_LCM_ClearScreen();
	API_LCM_Printf(0,0,0,0,TradeLog.Theme[VMCParam.Language]);
	API_LCM_DrawLine(0,2);
	API_LCM_Printf(0,3,0,0,TradeLog.Coin[VMCParam.Language],TotalTradeLog.TotalCoinPayIn/10,TotalTradeLog.TotalCoinPayIn%10,TotalTradeLog.TotalCoinPayout/10,TotalTradeLog.TotalCoinPayout%10);
	API_LCM_Printf(0,5,0,0,TradeLog.Bill[VMCParam.Language],TotalTradeLog.TotalBillPayIn/10,TotalTradeLog.TotalBillPayIn%10,0,0);
	API_LCM_Printf(0,7,0,0,TradeLog.Cashless[VMCParam.Language],TotalTradeLog.TotalCashlessPayIn/10,TotalTradeLog.TotalCashlessPayIn%10);
	API_LCM_Printf(0,9,0,0,TradeLog.TotalPay[VMCParam.Language],TotalCoinPayIn/10,TotalCoinPayIn%10,TotalTradeLog.TotalCoinPayout/10,TotalTradeLog.TotalCoinPayout%10);
	API_LCM_Printf(0,11,0,0,TradeLog.TtlTradNum[VMCParam.Language],TotalTradeLog.TotalSuccesNumber,TotalTradeLog.TotalErrorNumber);
	API_LCM_Printf(0,14,0,0,TradeLog.Exit[VMCParam.Language]);
	while(1)
	{
		key=API_KEY_ReadKey();
		if(key)
		{
			if(key == '>')
				break;
			else if(key == '<')
			{
				if(PwdCheck())
				{
					LogClearAPI();
					TotalCoinPayIn=(TotalTradeLog.TotalCoinPayIn + TotalTradeLog.TotalCashlessPayIn + TotalTradeLog.TotalBillPayIn);
					Trace("\r\nPwd success");
				}
				else
				{
					Trace("\r\nPwd false");
				}
				API_LCM_ClearScreen();
				vTaskDelay(2);
				API_LCM_Printf(0,0,0,0,TradeLog.Theme[VMCParam.Language]);
				API_LCM_DrawLine(0,2);
				API_LCM_Printf(0,3,0,0,TradeLog.Coin[VMCParam.Language],TotalTradeLog.TotalCoinPayIn/10,TotalTradeLog.TotalCoinPayIn%10,TotalTradeLog.TotalCoinPayout/10,TotalTradeLog.TotalCoinPayout%10);
				API_LCM_Printf(0,5,0,0,TradeLog.Bill[VMCParam.Language],TotalTradeLog.TotalBillPayIn/10,TotalTradeLog.TotalBillPayIn%10,0,0);
				API_LCM_Printf(0,7,0,0,TradeLog.Cashless[VMCParam.Language],TotalTradeLog.TotalCashlessPayIn/10,TotalTradeLog.TotalCashlessPayIn%10);
				API_LCM_Printf(0,9,0,0,TradeLog.TotalPay[VMCParam.Language],TotalCoinPayIn/10,TotalCoinPayIn%10,TotalTradeLog.TotalCoinPayout/10,TotalTradeLog.TotalCoinPayout%10);
				API_LCM_Printf(0,11,0,0,TradeLog.TtlTradNum[VMCParam.Language],TotalTradeLog.TotalSuccesNumber,TotalTradeLog.TotalErrorNumber);
				API_LCM_Printf(0,14,0,0,TradeLog.Exit[VMCParam.Language]);
			}
		}
		vTaskDelay(20);
	}
}
/**************************************End Of File***************************************************************************************/
