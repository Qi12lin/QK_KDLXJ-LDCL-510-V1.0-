/****************************************
*       文件名：app_balance.c
*       功  能：离心机不平衡状态检测
*
*****************************************/

#include "typedef.h"
#include "unbalance.h"
#include <math.h>  // 添加这一行来包含数学库的头文件
#include <stdlib.h>
#include <stdbool.h>


GET_VALU getvalue;

#define AMPLITUDE_DATA_LENGTH 100  

uint8_t balanCount = 0; 

// 添加一个全局变量来表示是否进行不平衡检测
int isBalanceCheckActive = 0;
u8 FUW =0;
u8 RXTX;//不平衡传感器检测

float amplitudeData[AMPLITUDE_DATA_LENGTH]; 
void loop(void)
{
    // 重力加速度常数
    float gravity = 9.81;
    
    // 用于存储三轴加速度和振幅的变量
    float accelX;
    float accelY;
    float accelZ;
    float amplitude;

    // 用于循环缓冲区的索引和均值计算
    static int amplitudeIndex = 0;
    float amplitudeMean = 0.0;
    
    // 用于计算方差和标准差的变量
    float sum = 0.0;
    float variance = 0.0;
    float BPH_value;

    int i;
	

    // 从MPU6050传感器获取加速度和陀螺仪数据
    RXTX = MPU_Get_Accelerometer(&runParam.ACCEL_X, &runParam.ACCEL_Y, &runParam.ACCEL_Z);
    MPU_Get_Gyroscope(&runParam.GYRO_X, &runParam.GYRO_Y, &runParam.GYRO_Z);
		if(RXTX == 1)
		{
			if(HldReg[BPH_SET] != 1)
			{
			SetAlarmFlag(IMBALANCE_ERROR_ALARM);//不平衡故障报警
			}
			else{}
			FUW+=1;
		}
		
					//重启功能
		if(RXTX == 0 && FUW>1)
		{
			FUW=0;
			MPU_Init();// 初始化MPU6050
			IWDG_Enable();
		}

    // 对原始加速度数据进行单位转换
    accelX = runParam.ACCEL_X * (gravity / 16384.0);
    accelY = runParam.ACCEL_Y * (gravity / 16384.0);
    accelZ = runParam.ACCEL_Z * (gravity / 16384.0);

    // 计算三轴加速度的振幅
    amplitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

    // 将振幅乘以1000，并存储在runParam.amplitude中
    runParam.amplitude = (u16)(amplitude * 1000);

    // 将振幅数据存储在循环缓冲区amplitudeData中，并更新索引
    amplitudeData[amplitudeIndex] = amplitude;
    amplitudeIndex = (amplitudeIndex + 1) % AMPLITUDE_DATA_LENGTH;

    // 计算循环缓冲区amplitudeData中所有数据的和
    for (i = 0; i < AMPLITUDE_DATA_LENGTH; i++)
    {
        sum += amplitudeData[i];
    }

    // 计算振幅均值
    amplitudeMean = sum / AMPLITUDE_DATA_LENGTH;

    // 计算方差
    for (i = 0; i < AMPLITUDE_DATA_LENGTH; i++)
    {
        variance += pow(amplitudeData[i] - amplitudeMean, 2);
    }
    
    // 计算标准差
//    variance /= AMPLITUDE_DATA_LENGTH;
    BPH_value = sqrt(variance);

    // 将振幅均值和标准差乘以1000，并存储在runParam中
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
 * @param a - 阈值1
 * @param b - 阈值2
 * @param c - 阈值3
 * @param d - 阈值4
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
				     SetAlarmFlag(IMBALANCE_ALARM);//不平衡报警
            }
        }
    }
    else if (runParam.SpeedRCF < c)
    {
        if (runParam.BPH_value >= b)
        {
				SetAlarmFlag(IMBALANCE_ALARM);//不平衡报警
        }
    }
    else
    {
        if (runParam.BPH_value >= d)
        {
				  SetAlarmFlag(IMBALANCE_ALARM);//不平衡报警
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
	
	 ee_WriteBytes((u8*)&getvalue, SAVE_RUN_DATA_ADD, sizeof(getvalue));//总计数器
}



void Balance_rede(void)
{
		ee_ReadBytes((u8*)&getvalue, SAVE_RUN_DATA_ADD, sizeof(getvalue));//总计数器
    HldReg[VALUE_LOW] = getvalue.VALUE_LOW[BPH_ROTOR];
    HldReg[VALUE_HIGH]= getvalue.VALUE_HIGH[BPH_ROTOR];
}


//不平衡检测功能测试代码
void Balance_Init(void)
{
  if (isBalanceCheckActive)
	{
		if (runParam.BPH_value >= HldReg[VALUE_LOW])
			{
					balanCount++;
					if (balanCount >= IMBALANCE_THRESHOLD_1)
					{
					 SetAlarmFlag(IMBALANCE_ALARM);//不平衡报警
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



