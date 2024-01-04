#ifndef _TYPEDEF_H
#define _TYPEDEF_H
//类型宏定义

#ifndef _NULL_H
#define _NULL_H
#define NULL 0
#endif

#define uchar unsigned char 
#define uint32 unsigned int   
#define WORD_LO(xxx)  	( (INT16U)(xxx) & 0xFF) 
#define WORD_HI(xxx)  	( (INT16U)(xxx) >> 8  )
#define BIT(i)	(1 << (i))
             
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

//客户定义
////#define SUZHOUOUSHIDA    		1	//0苏州欧仕达－活塞泵，气缸上升下降
////#define FIVE_HOSE				1	//5管5枪
//#define SHENZHENJIAMEIGAOKE   1	//2深圳嘉美高科
//#define SUZHOUOUSHIDA_PUR    	1	//3苏州欧仕达－PUR
//#define HANGZHOUYIHE_TIANDA	1	//4杭州亿赫－天达物流	开机和保温受上位机控制
//#define QUANZHOUJINGWEI    	1	//5泉州精威			5管5枪，主缸温度到达一定值后启动管枪，具有保温输出，西门子V20变频器
//#define JIANGMEWEILIAO    	1	//6江门维立奥		具有Hz/rpm选择
//#define SHENZHENFENGJUN_F1    1	//7深圳锋骏		具有自动泵胶功能，运行设定时间到后自动停止，三管无枪一辅热。	**非标
//#define   DONGGUANLILE_F1    	1	//8东莞立乐		一条管三把枪，枪的温度等管的温度上升到一定时再加热。	**非标
//#define   JUNDINGDA    		1	//9俊鼎达		管枪可选，单泵双泵可选
//#define   HUANGXING    		1	//10皇信，断电报警设置绝对值，具备报警使能选项，不需要马达控制，传感器异常时关闭使能
//#define   XINGWEI    			1	//11新威，大部分跟俊鼎达一样
//#define   XINGYUE    			1	//12东莞鑫阅，具有双活塞泵操作
//#define   XINGYUE_F1    		1	//13东莞鑫阅，2个枪组，4路点胶控制。
//#define SHENZHENFENGJUN_F2    1	//14深圳锋骏		无加热、双泵、双枪。	**非标
//#define QUANZHOUJUNSHENG	    1	//15泉州俊生   泵操作具备使能功能
//#define SHENZHENXUANSIRUI	    1	//16轩思睿	优化匀热时间功能。上电后温度在就绪区，匀热时间变成1分钟。
//#define JIANGMEWEILIAO_F2    	1	//17江门维立奥	五管五枪三泵，固定为rpm。采用启科变频器，0-10V模式先读到屏上来，五段可设，再发频率信号给变频器
#define SUZHOUOUSHIDA_F1    	1	//18苏州欧仕达－齿轮泵，具备外控开机功能
//高级密码定义
#define JIAMEIGAOKE_PASS    47791491 	//嘉美高科
/***********驱动层文件***********************/

//库文件
#include "stm32f4xx.h"		/* 如果要用ST的固件库，必须包含这个文件 */

//自定义
#include "bsp_driver.h"			//驱动初始化函数
#include "bsp_gpio.h"			//通用输入输出
#include "bsp_i2c_gpio.h"		//I2C总线驱动
#include "bsp_24xx02.h"			//24xx02 eeprom的驱动
#include "bsp_timer.h"			//定时器配置驱动
#include "bsp_uart.h"			//串口IO配置驱动
#include "bsp_mcp3208.h"
#include "MODBUS_RTU.H"
#include "UART_VGUS.H"
#include "w25qxx.h"
#include "bsp_adc.h"
#include "bsp_led.h"
#include "bsp_button.h"

#include "rtc.h"
//#include "PID.H"
/************驱动层文件结束******************/


/***********应用层文件***********************/
#include "app_comm.h"
#include "app_ControlComm.h"
#include "app_start_proc.h"

#include "app_analog.h"
#include "app_run_record.h"
#include "app_alarm_record.h"
#include "app_beep.h"
#include "app_versionSelectFunc.h"
#include "app_user.h"
#include "app_lock_proc.h"

#include "app_btn_scan.h"	//按键扫描
#include "app_btn_proc.h"	//按键处理

#include "app_temp.h"
/************应用层文件结束******************/

/************不平衡检测******************/
//mpu6050文件
#include "mpu6050.h"
#include "mpuiic.h"
#include "unbalance.h"
/************不平衡检测结束******************/

//#include "ff.h"	
//#include "app_image.h"	
/************字体文件************************/
//#include "digit.h"


#endif
