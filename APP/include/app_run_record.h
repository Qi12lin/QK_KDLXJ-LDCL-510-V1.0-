#include "typedef.h"

#ifndef _APP_RUN_RECORD_H
#define _APP_RUN_RECORD_H (1)

#define	RUN_RECORD_NUM		2400		//记录多少条信息
#define RUNRECORD_FIRST_ADDR	2400//历史数据记录起始地址（铁电）
//前20个断电保持区域数据------------------------------------------------------------

//报警记录相关参数-----------------------------------------------------------	
#define RUN_OFFSET		17	//每条记录所需的存储器
#define RUN_TYPE_OFFSET	11	//每条记录的数据种类

#define RUN_YEAR		0	 //报警发生年
#define RUN_MONTH		1	 //报警发生月
#define RUN_DAY			2	 //报警发生日
#define RUN_HOUR		3	 //报警发生时
#define RUN_MINUTE		4	 //报警发生分
#define RUN_ANAlOG1		5	 //压力
#define RUN_ANAlOG2		6	 //浓度
#define RUN_ANAlOG3		7	 //流量
#define RUN_ANAlOG4		8	 //温度
#define RUN_ANAlOG5H	9	 //累计
#define RUN_ANAlOG5L	10	 //
//报警记录参数结束-----------------------------------------------------------
extern uint32_t totalruntime;//,totalruntime_old;


extern void RunTimeRecord(void);

extern void Run_Record(void);
extern void MainTain(void);
#endif


