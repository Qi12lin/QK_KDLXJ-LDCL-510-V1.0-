/**********************************************************************************************************
*	��Ŀ���� �����۽�������                                
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : 
*	˵    �� : TR100D����
*	�޸ļ�¼ :
*	�汾��  		����         ����      ˵��
*	v1.00    2020-02-18     �´�Ծ	  ��һ�α�д��������������v1.03��Ļ������޸ģ�

*	Copyright (C), 2018-2019, ��ɳ���Ƶ��� 
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
//ALIENTEK ̽����STM32F407������ ʵ��53
//USB U�� ʵ��-�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

/*******��������*****/
//FIL fil;
FATFS *fatfs;
FRESULT res;
UINT bww;
//char buf[100];
//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)
{ 
}
/**********************************************************************************************************
*	�� �� ��: Default_Param
*	����˵��: Ĭ�ϲ���
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/
void Default_Param(void)
{
	uint8_t i;
	int32_t a32;	
	uint32_t b32,c32,d32;
	
//	AlarmRecovery_HI = ((int32_t)HldReg[RECOVERY1] << 16) + HldReg[RECOVERY2]; //�����ָ���־
//	AlarmRecovery_LO = ((int32_t)HldReg[RECOVERY3] << 16) + HldReg[RECOVERY4]; 	
//	
//	totalruntime = ((INT32U)HldReg[TOTAL_RUN_TIME_HI] << 16) + HldReg[TOTAL_RUN_TIME_LO];//������ʱ�䡡
//	if (totalruntime > 2000000000 || HldReg[TOTAL_RUN_TIME_HI] >= 0xF000) 
//		{totalruntime = 0;HldReg[TOTAL_RUN_TIME_HI]=0;HldReg[TOTAL_RUN_TIME_LO]=0;}
//	else {}
//	HldReg[RUN_TIME_HOUR] = 0;	HldReg[RUN_TIME_MINUTE] = 0;//��������ʱ��
//	
//	HldReg[PRIVILEGE_LEVEL_INPUT] = 1;//����ȼ�����
//	b32 = ((INT32U)HldReg[LOGIN_PASSWORD1_HI] << 16) + HldReg[LOGIN_PASSWORD1_LO];//�ȼ�1����
//	c32 = ((INT32U)HldReg[LOGIN_PASSWORD2_HI] << 16) + HldReg[LOGIN_PASSWORD2_LO];//�ȼ�2����
//	d32 = ((INT32U)HldReg[LOGIN_PASSWORD3_HI] << 16) + HldReg[LOGIN_PASSWORD3_LO];//�ȼ�3����
//	if (b32 == 0) HldReg[LOGIN_PASSWORD1_LO] = 10000;//
//	if (c32 == 0) HldReg[LOGIN_PASSWORD2_LO] = 10086;//
//	if (d32 == 0) HldReg[LOGIN_PASSWORD3_LO] = 6722;//		
//		
//	HldReg[ENTERPRISE_NAME] = 0;//��ҵ����
//	if (HldReg[LOCK_YEAR] > 100) HldReg[LOCK_YEAR] = 0;//������
//	if (HldReg[LOCK_MONTH] > 12) HldReg[LOCK_MONTH] = 0;	
//	if (HldReg[LOCK_DAY] > 31) HldReg[LOCK_DAY] = 0;		
//	if (HldReg[LOCK_EN] > 2) HldReg[LOCK_EN] = 0;//ͣ������ʹ��
//	HldReg[SCREENSAVER_TIME_TEMP] = HldReg[SCREENSAVER_TIME];//����ʱ�䣬��λ����	

//	for (i=0; i<10; i++) SlaveHldReg[i] = HldReg[i];//�ϵ罫�ϴ����еĲ�����ֵ��
//	if (HldReg[NUMBER_OF_VALUES_USED] == 0 || HldReg[NUMBER_OF_VALUES_USED] > 24) HldReg[NUMBER_OF_VALUES_USED] = 24;//����ʹ������	
////	if (HldReg[UNIT_STATE] > 2) HldReg[UNIT_STATE] = 0;//��������״̬	0ֹͣ��1����		��վ����ϵ籣��	
//	HldReg[UNIT_STATE] = 0;//��վ����ϵ籣��
//	if (HldReg[VALUE_STEP] > 100) HldReg[VALUE_STEP] = 0;//������˳��	
//	SlaveHldReg[SLAVE_NUMBER] = HldReg[SLAVE_NUMBER_C];//�ӻ�վ�ţ��ϵ�������ϴα����ֵ
//	HldReg[ANALOG_TYPE] = 0;//ģ���������ź�����,0-4~20mA,1-0~5V,2-0~10V
}
/**********************************************************************************************************
*	�� �� ��: main
*	����˵��: ������
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/
int main(void)
{ 
	u16 i;//,j;
	vu32 usDelayTm;
	static uint16_t e16[3];
	SCB->VTOR = FLASH_BASE | 0x10000;//IAP��ַƫ����		
//	char str[10];
//	char r = 20;
//	INT32U var_tmp;
	for(usDelayTm = 100000; usDelayTm >0; usDelayTm--);
	DriverInit();//������ʼ��
	for(i = 0; i < 5000; i++);
	Read_eepromData();
	
	
	
	InitComm0Data();//��ʼ��ͨ������
	//Default_Param();//�����a��ֵ
	Timer_1S[DELAY_ACT] = 4;//�ϵ缸����ٷ�����������
	Timer_1S[BATTERY_PULL_OUT_DELAY] = 8;//��ʱ�жϵ���Ƿ�γ�


//	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2��// 2bit(4preemption):2bit(4sub)
//	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
//	LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
//	KEY_Init();				//����
//  	LCD_Init();				//��ʼ��LCD 
//	W25QXX_Init();			//SPI FLASH��ʼ��
//	HldReg[900] = W25QXX_TYPE;
//	my_mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
// 	exfuns_init();			//Ϊfatfs��ر��������ڴ� 
//	piclib_init();			//��ʼ����ͼ
//  	f_mount(fs[0],"0:",1); 	//����SD��  
//  	res = f_mount(fatfs,"1:",1); 	//����FLASH 

//	HldReg[901] = res;
//	if(res==0x0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
//	{
//		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
//		if(res==0)
//		{
//			f_setlabel((const TCHAR *)"1:ALIENTEK");	//����Flash���̵�����Ϊ��ALIENTEK
//		}
//		Timer_1MS[WAIT_TIMER] = 100;//Delay_Ms(1000);                              //��ʱ1000ms
//		while(Timer_1MS[WAIT_TIMER]);
//	}
//	else {}//FLASH���سɹ�
  	res = f_mount(fs[2],"2:",1); 	//����U��
//	//��ʼ��USB����
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	IWDG_Enable();       //ʹ�ܿ��Ź�	
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
		IWDG_ReloadCounter();//ι����
		Bsp_Control_10ms();//10msִ��1��		
		Bsp_Control_100ms();//100msִ��1��
		Bsp_Control_200ms();//200msִ��1��		
		Bsp_Control_1s();	//1sִ��1��
	}
//	Beep_SetTime(1, 1000, 700);	
//-----------------------------------------------------------------------------	
	while(1)
	{
		IWDG_ReloadCounter();//ι�������ǵ�дU�̣����Ź�ʱ�䶨��Ϊ6S ��дU���ļ�ʱ��ι��
//		if (HldReg[RUNDATA_TO_U_DISC] == 1 || HldReg[ALARMDATA_TO_U_DISC] == 1) //���м�¼���ݵ�U�̡�������¼���ݵ�U��
//		{
//			USBH_Process(&USB_OTG_Core, &USB_Host);
//		}
//		else Timer_1S[U_DISC_CONNECT] = 10;//U�̼��ʱ��
	
		Bsp_Control_10ms();//10msִ��1��		
		Bsp_Control_100ms();//100msִ��1��
		Bsp_Control_200ms();//200msִ��1��		
		Bsp_Control_1s();	//1sִ��1��

	//	if (Timer_1S[DELAY_ACT] <= 1) ModbusRTUMasterRun(&ModbusModule1);//MODBUS��վ״̬����	
		//if (Timer_1S[DELAY_ACT] <= 1) EXTEND_ComModule();//��չģ�鷢�����ݴ���			
		if (Timer_1S[DELAY_ACT] <= 2) VgusMasterRun (&VgusModule0);//VGUS��վ״̬����
		if (Timer_1S[DELAY_ACT] <= 2) VGUS_ComCtr();//�������������ݴ���	
//		if (Timer_1S[DELAY_ACT] <= 2) EXTEND_ComModule11();//MODBUS��վ״̬����	
//		if (Timer_1S[DELAY_ACT] <= 2) ModbusRTUMasterRun(&ModbusModule0);//MODBUS��վ״̬����	
		if (Timer_1S[DELAY_ACT] <= 1) ModbusRTUSlaveRun(&ModbusModule0);//MODBUS��վ״̬����	
//		if (Timer_1S[DELAY_ACT] <= 1) EXTEND_COMController();//��չ�������������ݴ���			
		Run_Proc();//���п���ģ��	
		
		DLockCtrl();

		Temp_Proc();

//		TM1638_Display();
//		TM1638_ReadKey();
//		LED_Display();
		//����ģ��
//		Key_Proc();	
		Beep_Proc();
		
		Balance();
	}	
}










