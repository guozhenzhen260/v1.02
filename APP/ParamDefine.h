/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			ParamDefine.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _PARADEF_H
#define _PARADEF_H
/********************************************************************************************************
** @Define name:       APP Debug
********************************************************************************************************/
//#define  APP_DBG_MDB_BILL
//#define  APP_DBG_MDB_COIN
//#define  APP_DBG_FRAM
/********************************************************************************************************
** @Define name:       Vending Machine Parameter Define
********************************************************************************************************/
#define SOFTWAREVERSION		" V1.00"
#define MAX_MOTOR_ROW		6									//电机行数
#define MAX_MOTOR_COL		8									//电机列数
#define MAXARRAY			(MAX_MOTOR_ROW * MAX_MOTOR_COL)		//最大货道数量
#define VMC_MODE_VEND		0x01								//交易模式
#define VMC_MODE_MAIN		0x02								//维护模式
#define VMC_FRAM_ADDR		0xA0								//FRAM address
#define VMC_PARA_PAGE_S		0									//VMCParam start page
#define VMC_TDPA_PAGE_S		256									//TradeParam start page
#define VMC_TLOG_PAGE_S		512									//TotalTradeLog start page
#define VMC_SLOG_PAGE_S		1024								//TotalTradeLog start page
#define VMC_SLOG_PAGE_E		4096								//TotalTradeLog End page
/********************************************************************************************************
** @Define name:       VMCParam Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned char GoodsChannelArray[MAXARRAY];
	unsigned char GoodsMaxCapacity[MAXARRAY];
	unsigned char VMCID[7];
	unsigned char Password[6];
	unsigned char Language;
	unsigned char MdbBillDeviceEAB;
	unsigned char MdbCoinDeviceEAB;
	unsigned char MdbCashlessDeviceEAB;
	unsigned char Rtc[8];
	unsigned char VMCMode;
} VMCPARAM;
extern volatile VMCPARAM VMCParam;
/********************************************************************************************************
** @Define name:       Trade Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned short GoodsPrice[MAXARRAY];
	unsigned char  RemainGoods[MAXARRAY];
	unsigned short TypeBillEscrow;			//暂存通道
	unsigned short TypeBillEnable;			//纸币通道使能
	unsigned short TypeCoinEnable;			//硬币通道使能
	unsigned char  ServiceWhenNoChanger;
	unsigned char  ServiceWhenBillStackerIsFull;
} TRADEPARAM;
extern volatile TRADEPARAM TradeParam;
typedef struct
{
	unsigned int TotalCoinPayIn;
	unsigned int TotalCoinPayout;
	unsigned int TotalBillPayIn;
	unsigned int TotalBillPayOut;
	unsigned int TotalCashlessPayIn;
	unsigned int TotalVendNumber;
	unsigned int TotalSuccesNumber;
	unsigned int TotalErrorNumber;
	unsigned int SingleTradeLogPointer;
} TOTALTRADELOG;
extern volatile TOTALTRADELOG TotalTradeLog;
typedef struct
{
	unsigned char Time[7];
	unsigned char Err;
	unsigned int  CoinPayIn;
	unsigned int  BillPayIn;
	unsigned int  CoinPayout;
	unsigned char Channel;
} SINGLETRADELOG;
extern volatile SINGLETRADELOG SingleTradeLog;
/********************************************************************************************************
** @Define name:       Error Code Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned char FramErr;
	unsigned char TradeParamErr;
	unsigned char MdbErr;
	unsigned char MotorErr;
} SYSTEMERROR;
extern volatile SYSTEMERROR SysError;
/********************************************************************************************************
** @Define name:       MDB Device Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned char Oneline;
	unsigned char ManufacturerCode[3];
	unsigned char Level;
	unsigned char CurrencyCode[2];
	unsigned char ScalingFactor;
	unsigned char Decimal;
	unsigned char CoinTypeRouting[2];
	unsigned char CoinTypeCredit[16];
	unsigned char CoinTypeTubeStatus[16];
	unsigned char CoinTypePresentInTube[16];
	unsigned char Feature[4];
} MDBCOINACCEPTORCHANGER;
extern volatile MDBCOINACCEPTORCHANGER MDBCoinDevice;
typedef struct
{
	unsigned char Oneline;
	unsigned char ManufacturerCode[3];
	unsigned char Level;
	unsigned char CurrencyCode[2];
	unsigned int  ScalingFactor;
	unsigned char Decimal;
	unsigned int  StackerCapacity;
	unsigned char BillSecurityLevel[2];
	unsigned char Escrow;
	unsigned char BillTypeEscrowEnable[15];
	unsigned int  BillTypeCredit[15];
	unsigned char BillRecycSupport;
	unsigned char Level2Feature[4];
	unsigned char StackerStatus;
	unsigned int  BillInStacker;
} MDBBILLVALIDATORRECYCLER;
extern volatile MDBBILLVALIDATORRECYCLER MDBBillDevice;


/******************************************纸币器硬币器通讯结构体*****************************************************/
//纸币器收到纸币返回面值
#define MBOX_BILLMONEY					0x01
//禁能纸币器命令
#define MBOX_BILLDISABLEDEV				0x02
//使能纸币器命令
#define MBOX_BILLENABLEDEV				0x03
//重置纸币器命令
#define MBOX_BILLRESETDEV				0x04
//启动纸币器命令
#define MBOX_BILLINITDEV				0x05
//纸币器压抄命令
#define MBOX_BILLESCROW					0x06
//返回纸币器压抄成功命令
#define MBOX_BILLESCROWSUCC				0x07
//返回纸币器压抄失败命令
#define MBOX_BILLESCROWFAIL				0x08
//纸币器退币命令
#define MBOX_BILLRETURN					0x09
//返回纸币器退币成功命令
#define MBOX_BILLRETURNSUCC				0x0A
//返回纸币器退币失败命令
#define MBOX_BILLRETURNFAIL				0x0B
//返回纸币器参数命令
#define MBOX_BILLSETUPDEV				0x0C


//硬币器收到硬币返回面值
#define MBOX_COINMONEY					0x0D
//禁能硬币器命令
#define MBOX_COINDISABLEDEV				0x0E
//使能硬币器命令
#define MBOX_COINENABLEDEV				0x0F
//启动硬币器命令
#define MBOX_COININITDEV				0x10
//返回硬币器按下退币键退币请求命令
#define MBOX_COINRETURN					0x11
//返回硬币器参数命令
#define MBOX_COINSETUPDEV				0x12


//对于level3的mdb找零器找零命令
#define MBOX_MDBCHANGELEVEL3			0x13
//对于level2的mdb找零器找零命令
#define MBOX_MDBCHANGELEVEL2			0x14
//对于得到找零器余币量
#define MBOX_MDBCHANGETUBE				0x15
//对于hopper的找零器找零命令
#define MBOX_MDBCHANGEHOPPER			0x16
//找零器找零返回命令
#define MBOX_CHANGERBACKMONEY			0x17

typedef struct	
{
	unsigned char Communicate;			//1代表纸币器通讯不上
	unsigned char moto;					//1代表马达故障
	unsigned char sensor;				//1代表传感器故障
	unsigned char romchk;				//1代表rom出错
	unsigned char jam;					//1代表投币卡币
	unsigned char removeCash;			//1移除纸币钞箱
	unsigned char cashErr;				//1纸币钞箱故障，以及钞箱满
	unsigned char disable;				//1纸币因为各种原因被禁能了
	unsigned char recyErr;				//找零模块异常
}MDBBILLERROR;
extern MDBBILLERROR MdbBillErr;

typedef struct	
{
	unsigned char Communicate;			//1代表硬币器通讯不上
	unsigned char sensor;				//1代表传感器故障
	unsigned char tubejam;				//1代表出币口卡币
	unsigned char romchk;				//1代表rom出错
	unsigned char routing;				//1代表进币通道出错
	unsigned char jam;					//1代表投币卡币	
	unsigned char removeTube;			//1硬币斗移开
	//使用扩展指令检查
	unsigned char disable;				//1硬币因为各种原因被vmc禁能了
	unsigned char unknowError;			//非0硬币其他各种故障
	unsigned char unknowErrorLow;		//硬币其他各种故障的子状态码
	
}MDBCOINERROR;
extern MDBCOINERROR MdbCoinErr;

/********************************************************************************************************
** @Define name:       RTC Parameter Define
********************************************************************************************************/
extern volatile unsigned char Rtc[7];
/********************************************************************************************************
** @Define name:       CrcCheck Fuction
********************************************************************************************************/
extern void InitSysErrCode(void);
extern void InitVmcParam(void);
extern void LoadDefaultVmcParam(void);
extern void LoadNewVmcParam(void);
extern void InitTradeParam(void);
extern void LoadDefaultTradeParam(void);
extern void LoadNewTradeParam(void);
extern void InitTotalLog(void);
extern void LoadNewTotalLog(void);
extern void InitSingleLog(void);
extern void LoadNewSingleLog(void);
extern unsigned short CrcCheck(unsigned char *data,unsigned int len);
char *PrintfMoney(unsigned int dispnum);
#endif
/**************************************End Of File*******************************************************/
