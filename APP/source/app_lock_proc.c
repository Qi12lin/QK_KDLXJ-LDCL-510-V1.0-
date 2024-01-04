#include "typedef.h"



//开门锁信息位
u8 OnDrSgn;
u8 doorcommsendFlag; //通讯发送标志

//门锁控制
void DLockCtrl(void)
{
	static vu8 GateBit = 0;
	//开机时，还原门到位信号	  
  	static vu8 DoorPlate = 2;
	u8 signal = 0;
	static u8 gate_lock_flag =0;
	
HldReg[LOCK_TYPE] = INIT_LOCK_TYPE;
	if(HldReg[LOCK_TYPE]  == DZ_LOCK_TYPE)
	{ 
		//电子锁
		//signal调整门锁霍尔到正确位置
		signal = (DoorPlate == 2 && DOOR_CLOSURE && GATE_LOCK && runParam.Speed < 20);
		if( (signal || OnDrSgn )&& !DOOR)
		{
			//启动关门信号
			DOOR_ON;
			OnDrSgn = 0;
			DoorPlate =0;
			GateBit = GATE_LOCK; //门锁未到位信号1
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
		//电磁锁
		if(OnDrSgn)
		{
			OnDrSgn = 0; 
			if(DOOROPENTIME2_1MS ==0)
			{
				doorcommsendFlag =1;
				DOOR_ON;//HldReg[DOOROPEN_TIME] =300;范围100-999
				if(HldReg[LOCK_TYPE]  == ELEC_LOCK_TYPE)     DOOROPENTIME1_1MS =100;// HldReg[DOOROPEN_TIME];
					
				else if(HldReg[LOCK_TYPE] == DRAG_LOCK_TYPE) DOOROPENTIME1_1MS = HldReg[DOOROPEN_TIME]*10;
				DOOROPENTIME2_1MS =4000;
			}
		}
		if(DOOROPENTIME1_1MS ==0 && doorcommsendFlag ==0)  //等待开门结束
		{
			DOOR_OFF;
		}    
	}
}
