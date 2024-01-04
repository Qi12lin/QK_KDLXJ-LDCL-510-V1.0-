#include "typedef.h"

#ifndef _APP_ALARM_RECORD_H
#define _APP_ALARM_RECORD_H (1)

#define	ALARM_RECORD_MAX_NUM		400
#define	ALARM_RECORD_NUM		30		//��¼����������
#define	ALARM_TYPE_NUM			9		//��������
extern uint16_t	AlarmRecord[ALARM_RECORD_MAX_NUM];

//ǰ20���ϵ籣����������------------------------------------------------------------
#define	NOW_ALARM_NO	    0	//��ַ0���浱ǰ�������
								//1-ALARM_TYPE_NUM�������治ͬ���౨������ʱ�ı�����ţ��Ա�ָ�ʱ���ҵ���Ӧ�ĵ�ַ
//������¼��ز���-----------------------------------------------------------	
#define ALARM_OFFSET		10	//ÿ����������Ĵ洢��
#define ALARM_YEAR			0	 //����������
#define ALARM_MONTH			1	 //����������
#define ALARM_DAY			2	 //����������
#define ALARM_HOUR			3	 //��������ʱ
#define ALARM_MINUTE		4	 //����������
#define ALARM_CONTENT		5	 //��������
#define WORK_ANAlOG1		6	 //ѹ��
#define WORK_ANAlOG2		7	 //Ũ��
#define WORK_ANAlOG3		8	 //����
#define WORK_ANAlOG4		9	 //�¶�
//������¼��������-----------------------------------------------------------

extern uint64_t alarm,AlarmRecovery;
extern uint32_t alarm_HI,alarm_LO,AlarmRecovery_HI,AlarmRecovery_LO;
extern uint8_t alarm_no,alarm_code_temp[10];
extern void Alarm_Code(void);
extern void Alarm_Record(void);

#endif


