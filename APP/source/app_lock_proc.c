#include "typedef.h"



//��������Ϣλ
u8 OnDrSgn;
u8 doorcommsendFlag; //ͨѶ���ͱ�־

//��������
void DLockCtrl(void)
{
	static vu8 GateBit = 0;
	//����ʱ����ԭ�ŵ�λ�ź�	  
  	static vu8 DoorPlate = 2;
	u8 signal = 0;
	static u8 gate_lock_flag =0;
	
HldReg[LOCK_TYPE] = INIT_LOCK_TYPE;
	if(HldReg[LOCK_TYPE]  == DZ_LOCK_TYPE)
	{ 
		//������
		//signal����������������ȷλ��
		signal = (DoorPlate == 2 && DOOR_CLOSURE && GATE_LOCK && runParam.Speed < 20);
		if( (signal || OnDrSgn )&& !DOOR)
		{
			//���������ź�
			DOOR_ON;
			OnDrSgn = 0;
			DoorPlate =0;
			GateBit = GATE_LOCK; //����δ��λ�ź�1
		}
		if(DOOR && GATE_LOCK==0 && GateBit != GATE_LOCK)
		{
			DOOROPENTIME1_1MS =52;gate_lock_flag =1;
		}
		if(DOOROPENTIME1_1MS == 0 && gate_lock_flag ==1)
		{
		    DOOR_OFF;
			DoorPlate = 0;gate_lock_flag =0;
		}
		
		GateBit = GATE_LOCK;
	}
	else
	{		
		//�����
		if(OnDrSgn)
		{
			OnDrSgn = 0; 
			if(DOOROPENTIME2_1MS ==0)
			{
				doorcommsendFlag =1;
				DOOR_ON;//HldReg[DOOROPEN_TIME] =300;��Χ100-999
				if(HldReg[LOCK_TYPE]  == ELEC_LOCK_TYPE)     DOOROPENTIME1_1MS =100;// HldReg[DOOROPEN_TIME];
					
				else if(HldReg[LOCK_TYPE] == DRAG_LOCK_TYPE) DOOROPENTIME1_1MS = HldReg[DOOROPEN_TIME]*10;
				DOOROPENTIME2_1MS =4000;
			}
		}
		if(DOOROPENTIME1_1MS ==0 && doorcommsendFlag ==0)  //�ȴ����Ž���
		{
			DOOR_OFF;
		}    
	}
}
