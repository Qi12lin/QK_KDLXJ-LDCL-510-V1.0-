#include "typedef.h"

#ifndef _APP_RUN_RECORD_H
#define _APP_RUN_RECORD_H (1)

#define	RUN_RECORD_NUM		2400		//��¼��������Ϣ
#define RUNRECORD_FIRST_ADDR	2400//��ʷ���ݼ�¼��ʼ��ַ�����磩
//ǰ20���ϵ籣����������------------------------------------------------------------

//������¼��ز���-----------------------------------------------------------	
#define RUN_OFFSET		17	//ÿ����¼����Ĵ洢��
#define RUN_TYPE_OFFSET	11	//ÿ����¼����������

#define RUN_YEAR		0	 //����������
#define RUN_MONTH		1	 //����������
#define RUN_DAY			2	 //����������
#define RUN_HOUR		3	 //��������ʱ
#define RUN_MINUTE		4	 //����������
#define RUN_ANAlOG1		5	 //ѹ��
#define RUN_ANAlOG2		6	 //Ũ��
#define RUN_ANAlOG3		7	 //����
#define RUN_ANAlOG4		8	 //�¶�
#define RUN_ANAlOG5H	9	 //�ۼ�
#define RUN_ANAlOG5L	10	 //
//������¼��������-----------------------------------------------------------
extern uint32_t totalruntime;//,totalruntime_old;


extern void RunTimeRecord(void);

extern void Run_Record(void);
extern void MainTain(void);
#endif


