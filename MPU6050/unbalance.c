/****************************************
*       �ļ�����app_balance.c
*       ��  �ܣ����Ļ���ƽ��״̬���
*
*****************************************/

#include "typedef.h"
#include "unbalance.h"
#include <math.h>  // �����һ����������ѧ���ͷ�ļ�
#include <stdlib.h>
#include <stdbool.h>


GET_VALU getvalue;

#define AMPLITUDE_DATA_LENGTH 100  

uint8_t balanCount = 0; 

// ���һ��ȫ�ֱ�������ʾ�Ƿ���в�ƽ����
int isBalanceCheckActive = 0;
u8 FUW =0;
u8 RXTX;//��ƽ�⴫�������

float amplitudeData[AMPLITUDE_DATA_LENGTH]; 
void loop(void)
{
    // �������ٶȳ���
    float gravity = 9.81;
    
    // ���ڴ洢������ٶȺ�����ı���
    float accelX;
    float accelY;
    float accelZ;
    float amplitude;

    // ����ѭ���������������;�ֵ����
    static int amplitudeIndex = 0;
    float amplitudeMean = 0.0;
    
    // ���ڼ��㷽��ͱ�׼��ı���
    float sum = 0.0;
    float variance = 0.0;
    float BPH_value;

    int i;
	

    // ��MPU6050��������ȡ���ٶȺ�����������
    RXTX = MPU_Get_Accelerometer(&runParam.ACCEL_X, &runParam.ACCEL_Y, &runParam.ACCEL_Z);
    MPU_Get_Gyroscope(&runParam.GYRO_X, &runParam.GYRO_Y, &runParam.GYRO_Z);
		if(RXTX == 1)
		{
			if(HldReg[BPH_SET] != 1)
			{
			SetAlarmFlag(IMBALANCE_ERROR_ALARM);//��ƽ����ϱ���
			}
			else{}
			FUW+=1;
		}
		
					//��������
		if(RXTX == 0 && FUW>1)
		{
			FUW=0;
			MPU_Init();// ��ʼ��MPU6050
			IWDG_Enable();
		}

    // ��ԭʼ���ٶ����ݽ��е�λת��
    accelX = runParam.ACCEL_X * (gravity / 16384.0);
    accelY = runParam.ACCEL_Y * (gravity / 16384.0);
    accelZ = runParam.ACCEL_Z * (gravity / 16384.0);

    // ����������ٶȵ����
    amplitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

    // ���������1000�����洢��runParam.amplitude��
    runParam.amplitude = (u16)(amplitude * 1000);

    // ��������ݴ洢��ѭ��������amplitudeData�У�����������
    amplitudeData[amplitudeIndex] = amplitude;
    amplitudeIndex = (amplitudeIndex + 1) % AMPLITUDE_DATA_LENGTH;

    // ����ѭ��������amplitudeData���������ݵĺ�
    for (i = 0; i < AMPLITUDE_DATA_LENGTH; i++)
    {
        sum += amplitudeData[i];
    }

    // ���������ֵ
    amplitudeMean = sum / AMPLITUDE_DATA_LENGTH;

    // ���㷽��
    for (i = 0; i < AMPLITUDE_DATA_LENGTH; i++)
    {
        variance += pow(amplitudeData[i] - amplitudeMean, 2);
    }
    
    // �����׼��
//    variance /= AMPLITUDE_DATA_LENGTH;
    BPH_value = sqrt(variance);

    // �������ֵ�ͱ�׼�����1000�����洢��runParam��
    runParam.amplitudeMean = (u16)(amplitudeMean * 1000);
    runParam.BPH_value = (u16)(variance * 1000);
}

float gx_mean;
float gx_std;
float MAX_BPH;

void Balance(void)
{
		float ax_std;	
  	Balance_Init();
    loop();
		
	  if(runFlag.curRunFlag)isBalanceCheckActive=1;
      else isBalanceCheckActive=0;	

	
		if (isBalanceCheckActive) {
			if (MAX_BPH < runParam.BPH_value) {
						MAX_BPH = runParam.BPH_value;
				}
			
//			if(runParam.SpeedRCF<9000){
//				if (gx_mean < runParam.BPH_value) {
//						gx_mean = runParam.BPH_value;
//				}
//			}
//				if(runParam.SpeedRCF>9000)
//				{
//				 		if (gx_std < runParam.BPH_value) {
//						    gx_std = runParam.BPH_value;
//				   }
//				}
		}
		
		
		if(isBalanceCheckActive && runParam.SpeedRCF <= 10)
		{
			MAX_BPH=0;
//		  gx_mean=0;
//			gx_std=0;
		}
		
    runParam.MAX_BPH = MAX_BPH;
    
    runParam.gx_mean = gx_mean;
		runParam.gx_std=gx_std;
}


#define SPEED_THRESHOLD 2100
#define IMBALANCE_THRESHOLD_1 10
#define IMBALANCE_THRESHOLD_2 20
#define IMBALANCE_THRESHOLD_3 30

/**
 * @brief Rotor_Parameter 
 * @param a - ��ֵ1
 * @param b - ��ֵ2
 * @param c - ��ֵ3
 * @param d - ��ֵ4
 * @return void
 */
void Rotor_Parameter(int a, int b, int c, int d)
{
    if (runParam.SpeedRCF < SPEED_THRESHOLD)
    {
        if (runParam.BPH_value >= a)
        {
            balanCount++;
            if (balanCount >= IMBALANCE_THRESHOLD_1)
            {
				     SetAlarmFlag(IMBALANCE_ALARM);//��ƽ�ⱨ��
            }
        }
    }
    else if (runParam.SpeedRCF < c)
    {
        if (runParam.BPH_value >= b)
        {
				SetAlarmFlag(IMBALANCE_ALARM);//��ƽ�ⱨ��
        }
    }
    else
    {
        if (runParam.BPH_value >= d)
        {
				  SetAlarmFlag(IMBALANCE_ALARM);//��ƽ�ⱨ��
        }
    }
}


#define SAVE_RUN_DATA_ADD  6000   
u8 BPH_ROTOR;   

void Balance_Storage(void)
{
	BPH_ROTOR= HldReg[ROTOR];
	getvalue.VALUE_LOW[0]=HldReg[VALUE_LOW];
	getvalue.VALUE_HIGH[0]=HldReg[VALUE_HIGH];
	
	if(getvalue.BPHROTOR)
	{
	  HldReg[VALUE_LOW] = getvalue.VALUE_LOW[BPH_ROTOR];
		getvalue.BPHROTOR=0;
	}		
	else {
		getvalue.VALUE_LOW[BPH_ROTOR]=HldReg[VALUE_LOW];
    getvalue.VALUE_HIGH[BPH_ROTOR]=HldReg[VALUE_HIGH];
	}
	
	 ee_WriteBytes((u8*)&getvalue, SAVE_RUN_DATA_ADD, sizeof(getvalue));//�ܼ�����
}



void Balance_rede(void)
{
		ee_ReadBytes((u8*)&getvalue, SAVE_RUN_DATA_ADD, sizeof(getvalue));//�ܼ�����
    HldReg[VALUE_LOW] = getvalue.VALUE_LOW[BPH_ROTOR];
    HldReg[VALUE_HIGH]= getvalue.VALUE_HIGH[BPH_ROTOR];
}


//��ƽ���⹦�ܲ��Դ���
void Balance_Init(void)
{
  if (isBalanceCheckActive)
	{
		if (runParam.BPH_value >= HldReg[VALUE_LOW])
			{
					balanCount++;
					if (balanCount >= IMBALANCE_THRESHOLD_1)
					{
					 SetAlarmFlag(IMBALANCE_ALARM);//��ƽ�ⱨ��
					}
			}

				
//		if(HldReg[ROTOR] == 1)
//		{
//		 Rotor_Parameter(3,5,9000,6);
//		}
//		if(HldReg[ROTOR] == 2)
//		{
//		 Rotor_Parameter(90,90,6000,90);
//		}
//		if(HldReg[ROTOR] == 3)
//		{
//		 Rotor_Parameter(3,7,6000,10);
//		}
		
	}
}



