/**********************************************************************************************************
*	项目名称 ：热熔胶机程序                                
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : 
*	说    明 : TR100D程序
*	修改记录 :
*	版本号  		日期         作者      说明
*	v1.00    2020-02-18     陈春跃	  第一次编写，在三塔制氧机v1.03版的基础上修改；

*	Copyright (C), 2018-2019, 长沙启科电子 
*
**********************************************************************************************************/
#include "typedef.h"


#include "sys.h"
//#include "delay.h"  
#include "usart.h"   
//#include "led.h"
//#include "lcd.h"
//#include "key.h"  
//#include "usmart.h"  
#include "sram.h"   
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
//#include "fontupd.h"
//#include "text.h"	
//#include "piclib.h"	 
#include "usbh_usr.h" 
#include <string.h>
//ALIENTEK 探索者STM32F407开发板 实验53
//USB U盘 实验-库函数版本 
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

/*******变量定义*****/
//FIL fil;
FATFS *fatfs;
FRESULT res;
UINT bww;
//char buf[100];
//用户测试主程序
//返回值:0,正常
//       1,有问题
u8 USH_User_App(void)
{ 
}
/**********************************************************************************************************
*	函 数 名: Default_Param
*	功能说明: 默认参数
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void Default_Param(void)
{
	uint8_t i;
	int32_t a32;	
	uint32_t b32,c32,d32;
	
//	AlarmRecovery_HI = ((int32_t)HldReg[RECOVERY1] << 16) + HldReg[RECOVERY2]; //报警恢复标志
//	AlarmRecovery_LO = ((int32_t)HldReg[RECOVERY3] << 16) + HldReg[RECOVERY4]; 	
//	
//	totalruntime = ((INT32U)HldReg[TOTAL_RUN_TIME_HI] << 16) + HldReg[TOTAL_RUN_TIME_LO];//总运行时间　
//	if (totalruntime > 2000000000 || HldReg[TOTAL_RUN_TIME_HI] >= 0xF000) 
//		{totalruntime = 0;HldReg[TOTAL_RUN_TIME_HI]=0;HldReg[TOTAL_RUN_TIME_LO]=0;}
//	else {}
//	HldReg[RUN_TIME_HOUR] = 0;	HldReg[RUN_TIME_MINUTE] = 0;//本次运行时间
//	
//	HldReg[PRIVILEGE_LEVEL_INPUT] = 1;//密码等级输入
//	b32 = ((INT32U)HldReg[LOGIN_PASSWORD1_HI] << 16) + HldReg[LOGIN_PASSWORD1_LO];//等级1密码
//	c32 = ((INT32U)HldReg[LOGIN_PASSWORD2_HI] << 16) + HldReg[LOGIN_PASSWORD2_LO];//等级2密码
//	d32 = ((INT32U)HldReg[LOGIN_PASSWORD3_HI] << 16) + HldReg[LOGIN_PASSWORD3_LO];//等级3密码
//	if (b32 == 0) HldReg[LOGIN_PASSWORD1_LO] = 10000;//
//	if (c32 == 0) HldReg[LOGIN_PASSWORD2_LO] = 10086;//
//	if (d32 == 0) HldReg[LOGIN_PASSWORD3_LO] = 6722;//		
//		
//	HldReg[ENTERPRISE_NAME] = 0;//企业名称
//	if (HldReg[LOCK_YEAR] > 100) HldReg[LOCK_YEAR] = 0;//锁机年
//	if (HldReg[LOCK_MONTH] > 12) HldReg[LOCK_MONTH] = 0;	
//	if (HldReg[LOCK_DAY] > 31) HldReg[LOCK_DAY] = 0;		
//	if (HldReg[LOCK_EN] > 2) HldReg[LOCK_EN] = 0;//停机保护使能
//	HldReg[SCREENSAVER_TIME_TEMP] = HldReg[SCREENSAVER_TIME];//屏保时间，单位：秒	

//	for (i=0; i<10; i++) SlaveHldReg[i] = HldReg[i];//上电将上次运行的参数赋值勤
//	if (HldReg[NUMBER_OF_VALUES_USED] == 0 || HldReg[NUMBER_OF_VALUES_USED] > 24) HldReg[NUMBER_OF_VALUES_USED] = 24;//阀门使用数量	
////	if (HldReg[UNIT_STATE] > 2) HldReg[UNIT_STATE] = 0;//机组运行状态	0停止，1运行		从站不需断电保持	
//	HldReg[UNIT_STATE] = 0;//从站不需断电保持
//	if (HldReg[VALUE_STEP] > 100) HldReg[VALUE_STEP] = 0;//阀动作顺序	
//	SlaveHldReg[SLAVE_NUMBER] = HldReg[SLAVE_NUMBER_C];//从机站号，上电读出来上次保存的值
//	HldReg[ANALOG_TYPE] = 0;//模拟量输入信号类型,0-4~20mA,1-0~5V,2-0~10V
}
/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 主函数
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
int main(void)
{ 
	u16 i;//,j;
	vu32 usDelayTm;
	static uint16_t e16[3];
	SCB->VTOR = FLASH_BASE | 0x10000;//IAP地址偏移量		
//	char str[10];
//	char r = 20;
//	INT32U var_tmp;
	for(usDelayTm = 100000; usDelayTm >0; usDelayTm--);
	DriverInit();//驱动初始化
	for(i = 0; i < 5000; i++);
	Read_eepromData();
	
	
	
	InitComm0Data();//初始化通信数据
	//Default_Param();//参数黙认值
	Timer_1S[DELAY_ACT] = 4;//上电几秒后不再发送设置数据
	Timer_1S[BATTERY_PULL_OUT_DELAY] = 8;//延时判断电池是否拔出


//	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2，// 2bit(4preemption):2bit(4sub)
//	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
//	LED_Init();				//初始化与LED连接的硬件接口
//	KEY_Init();				//按键
//  	LCD_Init();				//初始化LCD 
//	W25QXX_Init();			//SPI FLASH初始化
//	HldReg[900] = W25QXX_TYPE;
//	my_mem_init(SRAMIN);	//初始化内部内存池	
// 	exfuns_init();			//为fatfs相关变量申请内存 
//	piclib_init();			//初始化画图
//  	f_mount(fs[0],"0:",1); 	//挂载SD卡  
//  	res = f_mount(fatfs,"1:",1); 	//挂载FLASH 

//	HldReg[901] = res;
//	if(res==0x0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
//	{
//		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
//		if(res==0)
//		{
//			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
//		}
//		Timer_1MS[WAIT_TIMER] = 100;//Delay_Ms(1000);                              //延时1000ms
//		while(Timer_1MS[WAIT_TIMER]);
//	}
//	else {}//FLASH挂载成功
  	res = f_mount(fs[2],"2:",1); 	//挂载U盘
//	//初始化USB主机
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	IWDG_Enable();       //使能看门狗	
	while(Timer_1S[DELAY_ACT] >3)
	{
		if(HldReg[SET_STATRPAGE] ==2) 
		{
			switch(HldReg[SYS_TYPE])
			{
				case LY_TGL_16M:
					HldReg[CHANGE_PAGE] = 5;
					break;
				case LY_TGL18M:
					HldReg[CHANGE_PAGE] = 6;
					break;
				case LY_TGL20M:
					HldReg[CHANGE_PAGE] = 7;
					break;
				case LY_TDL5M:
					HldReg[CHANGE_PAGE] = 11;
					break;
			}
		}
		else HldReg[CHANGE_PAGE] = 1;
        VgusMasterRun (&VgusModule0);
		VGUS_ComCtr();
		IWDG_ReloadCounter();//喂狗，
		Bsp_Control_10ms();//10ms执行1次		
		Bsp_Control_100ms();//100ms执行1次
		Bsp_Control_200ms();//200ms执行1次		
		Bsp_Control_1s();	//1s执行1次
	}
//	Beep_SetTime(1, 1000, 700);	
//-----------------------------------------------------------------------------	
	while(1)
	{
		IWDG_ReloadCounter();//喂狗，考虑到写U盘，看门狗时间定义为6S 。写U盘文件时有喂狗
//		if (HldReg[RUNDATA_TO_U_DISC] == 1 || HldReg[ALARMDATA_TO_U_DISC] == 1) //运行记录备份到U盘、报警记录备份到U盘
//		{
//			USBH_Process(&USB_OTG_Core, &USB_Host);
//		}
//		else Timer_1S[U_DISC_CONNECT] = 10;//U盘检测时间
	
		Bsp_Control_10ms();//10ms执行1次		
		Bsp_Control_100ms();//100ms执行1次
		Bsp_Control_200ms();//200ms执行1次		
		Bsp_Control_1s();	//1s执行1次

	//	if (Timer_1S[DELAY_ACT] <= 1) ModbusRTUMasterRun(&ModbusModule1);//MODBUS主站状态处理	
		//if (Timer_1S[DELAY_ACT] <= 1) EXTEND_ComModule();//扩展模块发送数据处理			
		if (Timer_1S[DELAY_ACT] <= 2) VgusMasterRun (&VgusModule0);//VGUS主站状态处理
		if (Timer_1S[DELAY_ACT] <= 2) VGUS_ComCtr();//触摸屏发送数据处理	
//		if (Timer_1S[DELAY_ACT] <= 2) EXTEND_ComModule11();//MODBUS从站状态处理	
//		if (Timer_1S[DELAY_ACT] <= 2) ModbusRTUMasterRun(&ModbusModule0);//MODBUS从站状态处理	
		if (Timer_1S[DELAY_ACT] <= 1) ModbusRTUSlaveRun(&ModbusModule0);//MODBUS主站状态处理	
//		if (Timer_1S[DELAY_ACT] <= 1) EXTEND_COMController();//扩展控制器发送数据处理			
		Run_Proc();//运行控制模块	
		
		DLockCtrl();

		Temp_Proc();

//		TM1638_Display();
//		TM1638_ReadKey();
//		LED_Display();
		//按键模块
//		Key_Proc();	
		Beep_Proc();
		
		Balance();
	}	
}










