#include "typedef.h"

#ifndef _APP_ALARM_RECORD_H
#define _APP_ALARM_RECORD_H (1)

#define	ALARM_RECORD_MAX_NUM		400
#define	ALARM_RECORD_NUM		30		//记录多少条报警
#define	ALARM_TYPE_NUM			9		//故障种类
extern uint16_t	AlarmRecord[ALARM_RECORD_MAX_NUM];

//前20个断电保持区域数据------------------------------------------------------------
#define	NOW_ALARM_NO	    0	//地址0保存当前报警序号
								//1-ALARM_TYPE_NUM用来保存不同种类报警发生时的报警序号，以便恢复时能找到对应的地址
//报警记录相关参数-----------------------------------------------------------	
#define ALARM_OFFSET		10	//每条报警所需的存储器
#define ALARM_YEAR			0	 //报警发生年
#define ALARM_MONTH			1	 //报警发生月
#define ALARM_DAY			2	 //报警发生日
#define ALARM_HOUR			3	 //报警发生时
#define ALARM_MINUTE		4	 //报警发生分
#define ALARM_CONTENT		5	 //报警内容
#define WORK_ANAlOG1		6	 //压力
#define WORK_ANAlOG2		7	 //浓度
#define WORK_ANAlOG3		8	 //流量
#define WORK_ANAlOG4		9	 //温度
//报警记录参数结束-----------------------------------------------------------

extern uint64_t alarm,AlarmRecovery;
extern uint32_t alarm_HI,alarm_LO,AlarmRecovery_HI,AlarmRecovery_LO;
extern uint8_t alarm_no,alarm_code_temp[10];
extern void Alarm_Code(void);
extern void Alarm_Record(void);

#endif


