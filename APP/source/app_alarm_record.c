/*
**************************************************************
		文件名：	app_alarm_record.c
		功	 能：实现报警记录。


**************************************************************
*/
#include "typedef.h"
//#include "app_alarm_record.H"
uint16_t AlarmRecord[ALARM_RECORD_MAX_NUM];// __attribute__((at(0X2000F500)));// ((section ("ER_RSV")));
/**********************************************************************************************************
*	函 数 名: Alarm_Code
*	功能说明: 报警标志
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
uint64_t alarm,AlarmRecovery;
uint32_t alarm_HI,alarm_LO,alarm_S2,AlarmRecovery_HI,AlarmRecovery_LO;
uint8_t alarm_no=0;
void Alarm_Code(void)
{
	
}
/**********************************************************************************************************
*	函 数 名: Alarm_Record
*	功能说明: 报警记录				
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void Alarm_Record(void)
{
	u8 i,k,l,m;
	uint16_t a16;
	uint32_t temp,temp1;

}




