#ifndef _TYPEDEF_H
#define _TYPEDEF_H
//���ͺ궨��

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

//�ͻ�����
////#define SUZHOUOUSHIDA    		1	//0����ŷ�˴�����ã����������½�
////#define FIVE_HOSE				1	//5��5ǹ
//#define SHENZHENJIAMEIGAOKE   1	//2���ڼ����߿�
//#define SUZHOUOUSHIDA_PUR    	1	//3����ŷ�˴PUR
//#define HANGZHOUYIHE_TIANDA	1	//4�����ںգ��������	�����ͱ�������λ������
//#define QUANZHOUJINGWEI    	1	//5Ȫ�ݾ���			5��5ǹ�������¶ȵ���һ��ֵ��������ǹ�����б��������������V20��Ƶ��
//#define JIANGMEWEILIAO    	1	//6����ά����		����Hz/rpmѡ��
//#define SHENZHENFENGJUN_F1    1	//7���ڷ濥		�����Զ��ý����ܣ������趨ʱ�䵽���Զ�ֹͣ��������ǹһ���ȡ�	**�Ǳ�
//#define   DONGGUANLILE_F1    	1	//8��ݸ����		һ��������ǹ��ǹ���¶ȵȹܵ��¶�������һ��ʱ�ټ��ȡ�	**�Ǳ�
//#define   JUNDINGDA    		1	//9������		��ǹ��ѡ������˫�ÿ�ѡ
//#define   HUANGXING    		1	//10���ţ��ϵ籨�����þ���ֵ���߱�����ʹ��ѡ�����Ҫ�����ƣ��������쳣ʱ�ر�ʹ��
//#define   XINGWEI    			1	//11�������󲿷ָ�������һ��
//#define   XINGYUE    			1	//12��ݸ���ģ�����˫�����ò���
//#define   XINGYUE_F1    		1	//13��ݸ���ģ�2��ǹ�飬4·�㽺���ơ�
//#define SHENZHENFENGJUN_F2    1	//14���ڷ濥		�޼��ȡ�˫�á�˫ǹ��	**�Ǳ�
//#define QUANZHOUJUNSHENG	    1	//15Ȫ�ݿ���   �ò����߱�ʹ�ܹ���
//#define SHENZHENXUANSIRUI	    1	//16��˼�	�Ż�����ʱ�书�ܡ��ϵ���¶��ھ�����������ʱ����1���ӡ�
//#define JIANGMEWEILIAO_F2    	1	//17����ά����	�����ǹ���ã��̶�Ϊrpm���������Ʊ�Ƶ����0-10Vģʽ�ȶ�������������ο��裬�ٷ�Ƶ���źŸ���Ƶ��
#define SUZHOUOUSHIDA_F1    	1	//18����ŷ�˴���ֱã��߱���ؿ�������
//�߼����붨��
#define JIAMEIGAOKE_PASS    47791491 	//�����߿�
/***********�������ļ�***********************/

//���ļ�
#include "stm32f4xx.h"		/* ���Ҫ��ST�Ĺ̼��⣬�����������ļ� */

//�Զ���
#include "bsp_driver.h"			//������ʼ������
#include "bsp_gpio.h"			//ͨ���������
#include "bsp_i2c_gpio.h"		//I2C��������
#include "bsp_24xx02.h"			//24xx02 eeprom������
#include "bsp_timer.h"			//��ʱ����������
#include "bsp_uart.h"			//����IO��������
#include "bsp_mcp3208.h"
#include "MODBUS_RTU.H"
#include "UART_VGUS.H"
#include "w25qxx.h"
#include "bsp_adc.h"
#include "bsp_led.h"
#include "bsp_button.h"

#include "rtc.h"
//#include "PID.H"
/************�������ļ�����******************/


/***********Ӧ�ò��ļ�***********************/
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

#include "app_btn_scan.h"	//����ɨ��
#include "app_btn_proc.h"	//��������

#include "app_temp.h"
/************Ӧ�ò��ļ�����******************/

/************��ƽ����******************/
//mpu6050�ļ�
#include "mpu6050.h"
#include "mpuiic.h"
#include "unbalance.h"
/************��ƽ�������******************/

//#include "ff.h"	
//#include "app_image.h"	
/************�����ļ�************************/
//#include "digit.h"


#endif
