/*
**************************************************************
		�ļ�����	app_comm.c
		��	 �ܣ�	ʵ�����������ͨ�ŵĹ��ܺ���������ͨ��
					Э�顣


**************************************************************
*/
#include "typedef.h"

vu8 tranStr[20]; //���ͻ�����
vu8 receStr[20];
u8 OldReceStr[20];
vu8 recePos;
vu8 sendPos = 0;

//��Ӧͨ�Ź��ϱ�־λ
vu8 commErrsignal;

vu8 LRC = 0; //У������

COMM_FLAG comFlag;
//�������ݸ���
//vu16 receData = 0;

//���й������жϵ����ת�Ķ�ʱ��
//TIME_TYPE MotorTimer;

//��վվ��
#define ADMIN_NO    (u8)0x01 
//��վվ��
#define SLAVE_NO    (u8)0x01



u16 commTm = 5000;        //ͨѶ�����ж϶�ʱ��
u16 comSenderTm =600;   //��ʱ���Ͷ�ʱ��
//��ʼ��ͨ������





extern vu8 g_ucState;

//200ms��ʱ����ʱ
void Counter_200ms(void)
{
	//200ms

	if(comSenderTm ==0)
	{
		Comm_Proc(1);	 		//ͨ�Ź���	
		//����һ������

		comSenderTm = SENDDELAYTIME;
		recePos = 0;
	}
	else
	{
		comSenderTm--;
	}
	if(commTm >0) commTm--;  
	else comFlag.comm_failure =1;  //ͨѶʧ�ܱ�־
}


//ͨ�Ź��̴���

void Comm_Proc(u8 _ucfSending)
{
	if(_ucfSending)
	{
		Comm_Send_Block((u8 *)tranStr, tranStr[1]+3);
    }
}

extern int send_start_flag;
//����һ������
void Comm_Send_Block(u8 * dat, u8 length)
{
//	u8* pStr = (u8*)dat;

	sendPos = 1;

	setSendData(); 
	//if(door_on_flag >0) door_on_flag--;
	USART_SendData(UART4, (uint16_t)(tranStr[0]));
  doorcommsendFlag =0;

}

//����һ������
void Comm_Rece_Block(void)
{
	//��ʱ����һ���ֽ����ݣ�����������ˣ�����һ��������ɱ�־
	vu8 dat;
	vu8 j,i;
	if( uart_Rece_Byte((u8 *)&dat) != 0)
	{
		receStr[recePos] = dat;
		recePos++;
		if(recePos >= 20) recePos = 0;
		if(recePos == receStr[1] + 3) // ���ݽ������
		{
			j = 0;
			for(i = 0; i < receStr[1] + 2; i++) //0~13
			{
				j += receStr[i];
			}
			LRC = ~j + 1;
			
			if(LRC == receStr[receStr[1]+2] && receStr[0] == ADMIN_NO)
			{
					//��ȡ���յ�������
				for(i = 0; i < receStr[1]; i++ ) OldReceStr[i] = receStr[i+2]; 
				j = 0;
				//���ý��յ�����
//						for (i=0;i<250;i++);          // delay
	
				

				commTm = COMMERRTIME;
				comSenderTm = SENDDELAYTIME;
				comFlag.comm_failure =0;
				if(COMCN_ERROR_ALARM == runParam.alarm)  
				{
					runParam.alarm =0;	
//				TM_200ms = TM_200MS_LEN;
				}
			
				Comm_Proc(1);
			}
			recePos = 0;   //����λ�ø�λ
		}
	 }
	 else
	 {
	 }
}

//���÷�������
void SetTransInc(void)
{
	//�ж��Ƿ񵽴�Ƶ��
	if(runFlag.arriveTargetFlag)
	{
		controlData.OUT0 |= 0x10; //���õ���ת��
		tranStr[6]  = 0;
		tranStr[7]  = 200;
		tranStr[8] =  0;
		tranStr[9] =  200; //����ֵ
	}
	else
	{
		controlData.OUT0 &= 0xef; //����û�е���ת��

		if(controlData.waitTimeToTFreq >= 1)
		{
			//�ȶ�״̬��������
			tranStr[6]  = ((controlData.TS_FreqInc& 0xff00) >> 8);
			tranStr[7]  = (controlData.TS_FreqInc & 0x00ff);
			tranStr[8] =  ((controlData.TS_FreqInc& 0xff00) >> 8);
			tranStr[9] =  (controlData.TS_FreqInc & 0x00ff);
		}
		else
		{
			tranStr[6]  =  ((allFreq.upFreqInc & 0xff00) >> 8);
			tranStr[7]  = (allFreq.upFreqInc & 0x00ff);
			tranStr[8] =  ((allFreq.downFreqInc & 0xff00) >> 8);
			tranStr[9] =  (allFreq.downFreqInc & 0x00ff);
		}
	} 
}

//�������ݵ�����   ��Ƶ��
void setSendData(void)   
{
	u8 i;
	vu8 j;
//	vu16 LCRC0 = 0;
	
	
	
	//����ת��״̬��������
	if(HldReg[MOTOR_TYPE] == QK_VFD)
	{
		if(controlData.stop_mode == 2) controlData.TxStopMode = 1;  //����ֱ��ֱ���ƶ�ͣ��
		else if(controlData.stop_mode == 3) controlData.TxStopMode = 1;//���е�1HZ ,ֱ���ƶ�ͣ��
		else if(controlData.stop_mode == 4) controlData.TxStopMode = 0;//���е�1HZ ,����ͣ��
		else {}
		
		SetTransInc();
		
		tranStr[0] = SLAVE_NO;	//01վ��
		tranStr[1] = 12;	 //���͵����ݵĳ���
		tranStr[2] = controlData.OUT0;    //��־λ
		tranStr[3]= 0x10 + controlData.TxStopMode;  //ͣ����ʽ,0x10��һȦ������
		tranStr[4] = ((controlData.PWM0h_Freq & 0xff00) >> 8); //
		tranStr[5]=  (controlData.PWM0h_Freq & 0x00ff);//���͵�Ŀ��Ƶ��
		tranStr[10] = (MOTOR_BASE_FREQ & 0x00ff); //��Ƶ��8λ
		tranStr[11] = RATE_CURRENT; //����
		tranStr[12]   = (MOTOR_BASE_FREQ & 0xff00) >> 8;//��Ƶ��8λ
		
		tranStr[13]   = 0x00; //����λ
	}	
	else if(HldReg[MOTOR_TYPE] == QK_WS)
	{
		tranStr[0] = SLAVE_NO;	//01վ��
		tranStr[1] = 9;	 //���͵����ݵĳ���
		tranStr[2] = controlData.OUT0;    //��־λ
		tranStr[3]=  HldReg[ROTOR];                          //ת�Ӻ�
		tranStr[4] = ((controlData.PWM0h_Freq & 0xff00) >> 8); //
		tranStr[5]=  (controlData.PWM0h_Freq & 0x00ff);//���͵�Ŀ��Ƶ��
		if(HldReg[SYS_TYPE] > LOW_TYPE)  tranStr[6] = 0xAF;   //����
		else tranStr[6] = 0xA0;  
		tranStr[7] = HldReg[DEC];                   //���ٵ�
		tranStr[8]   = (Speed_max[HldReg[ROTOR]] & 0xff00) >>8; //���ת��
		tranStr[9]   = Speed_max[HldReg[ROTOR]] &0x00ff;  
		tranStr[10]   = 28;
	}
	else //�������ݵ�����  ת�Ӱ�ӱ�Ƶ��
	{
		tranStr[0] = SLAVE_NO;	//01վ��
		tranStr[1] = 10;	 //���͵����ݵĳ���
		tranStr[2] = controlData.OUT0;    //��־λ

		tranStr[3] = ((set2 & 0xff00) >> 8); //
		tranStr[4]=  (set2 & 0x00ff);//ת��
		tranStr[5] = ((HldReg[ACC] ) &0x0f) |(((HldReg[DEC])<<4) & 0xf0 ); //�����ٵ�λ  
		
		tranStr[6]   = (Speed_max[1] & 0xff00) >>8; //���ת�� ϵͳ�����ת��
		tranStr[7]   = Speed_max[1] &0x00ff;  
		tranStr[8]   = HldReg[SET_TEMPERATURE]/10 +80;
		tranStr[9]   = HldReg[ROTOR];
		tranStr[10]   = (Speed_max[HldReg[ROTOR]] & 0xff00) >>8; //���ת��  ����ת�ӵ����ת��
		tranStr[11]   = Speed_max[HldReg[ROTOR]] &0x00ff; 
	}
		//��У��ֵ
		j = 0;
		for(i = 0; i < tranStr[1] + 2; i++)
		{
			j +=tranStr[i];
		}
	
		tranStr[tranStr[1] + 2] = ~j + 1;
	
}





