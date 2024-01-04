#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

// 包含Kalman滤波器
#include "kalmen_filte.h"

#define gyro_raw_to_deg_s 0.0609756097561f // +-250°/s:131LSB/°/s   +-500°/s:65.5LSB/°/s   +-1000°/s:32.8LSB/°/s    +-2000°/s:16.4LSB/°/s
#define acc_raw_to_g 0.000244140625f        // +-2g : 16384LSB/g     +-4g : 8192LSB/g   +-8g : 4096LSB/g   +-16g : 2048LSB/g

// 加速度计校准参数
const float acc_param_k[3] = {0.9928, 1.0030, 0.9894};
const float acc_param_a[3] = {-0.0668, 0.0172, 0.0774};
const float gyro_offest[3] = {-0.96, 0.902, -1.05};

kalman_struct gyro_x_filter; // 陀螺仪X轴滤波器
kalman_struct gyro_y_filter; // 陀螺仪Y轴滤波器
kalman_struct gyro_z_filter; // 陀螺仪Z轴滤波器

kalman_struct accel_x_filter; // 加速度计X轴滤波器
kalman_struct accel_y_filter; // 加速度计Y轴滤波器
kalman_struct accel_z_filter; // 加速度计Z轴滤波器

// MPU6050初始化函数
u8 MPU_Init(void)
{
	u8 res;
  // 初始化IIC总线
	getvalue.OpenBPH = 1;
  MPU_IIC_Init();
  MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
  Delay_ms(100);
  MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
  MPU_Set_Gyro_Fsr(3);                   // 陀螺仪传感器,±2000dps
  MPU_Set_Accel_Fsr(0);                  // 加速度传感器,±2g
  MPU_Set_Rate(6);                       // 设置采样率50Hz
  MPU_Write_Byte(MPU_INT_EN_REG, 0X00);   // 关闭所有中断
  MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
  MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);   // 关闭FIFO
  MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效

  // 初始化陀螺仪滤波器
  kalman_init(&gyro_x_filter, 0, 1); // 初始值为0，初始误差协方差为1
  kalman_init(&gyro_y_filter, 0, 1);
  kalman_init(&gyro_z_filter, 0, 1);

  // 初始化加速度计滤波器
  kalman_init(&accel_x_filter, 0, 1);
  kalman_init(&accel_y_filter, 0, 1);
  kalman_init(&accel_z_filter, 0, 1);

  res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
  if (res == MPU_ADDR) // 器件ID正确
  {
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
    MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
    MPU_Set_Rate(6);                        // 设置采样率为50Hz
  }
  else
    return 1;
  return 0;
}

// 设置陀螺仪满量程范围
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
  return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3);
}

// 设置加速度传感器满量程范围
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
  return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3);
}

// 设置数字低通滤波器
u8 MPU_Set_LPF(u16 lpf)
{
  u8 data = 0;
  if (lpf >= 1000)
    data = 0;
  else if (lpf >= 188)
    data = 1;
  else if (lpf >= 98)
    data = 2;
  else if (lpf >= 42)
    data = 3;
  else if (lpf >= 10)
    data = 4;
  else if (lpf >= 0)
    data = 5;
  else
    data = 6;
  return MPU_Write_Byte(MPU_CFG_REG, data);
}

// 设置采样率
u8 MPU_Set_Rate(u16 rate)
{
  u8 data;
  if (rate > 1000) rate = 1000; // 限制采样率不超过1KHz
  if (rate < 4) rate = 4; // 限制采样率不低于4Hz
  data = 1000 / rate; // 计算采样率对应的配置值
  data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
  return MPU_Set_LPF(rate / 2); // 自动设置LPF为采样率的一半
}

// 获取温度值
short MPU_Get_Temperature(void)
{
  u8 buf[2];
  short raw;
  float temp;
  MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
  raw = ((u16)buf[0] << 8) | buf[1];
  temp = 36.53 + ((double)raw) / 340;
  return temp * 100;
}

// 获取陀螺仪数据
u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
  u8 buf[6], res;
  int16_t GyroData[3];
  res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
  if (res == 0)
  {
    GyroData[0] = ((u16)buf[0] << 8) | buf[1];
    GyroData[1] = ((u16)buf[2] << 8) | buf[3];
    GyroData[2] = ((u16)buf[4] << 8) | buf[5];

    *gx = ((float)GyroData[0]) * gyro_raw_to_deg_s;
    *gy = ((float)GyroData[1]) * gyro_raw_to_deg_s;
    *gz = ((float)GyroData[2]) * gyro_raw_to_deg_s;

    *gx -= gyro_offest[0];
    *gy -= gyro_offest[1];
    *gz -= gyro_offest[2];

    // 使用卡尔曼滤波器对数据进行滤波
    *gx = (short)kalman_filter(&gyro_x_filter, (float)*gx);
    *gy = (short)kalman_filter(&gyro_y_filter, (float)*gy);
    *gz = (short)kalman_filter(&gyro_z_filter, (float)*gz);
  }
  return res;
}

// 获取加速度计数据
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
  u8 buf[6], res;
  int16_t AccData[3];
  res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
  if (res == 0)
  {
    AccData[0] = ((u16)buf[0] << 8) | buf[1];
    AccData[1] = ((u16)buf[2] << 8) | buf[3];
    AccData[2] = ((u16)buf[4] << 8) | buf[5];

    *ax = ((((float)AccData[0]) * acc_raw_to_g) / 9.8) * 1000;
    *ay = ((((float)AccData[1]) * acc_raw_to_g) / 9.8) * 1000;
    *az = ((((float)AccData[2]) * acc_raw_to_g) / 9.8) * 1000;

    *ax = acc_param_k[0] * *ax + acc_param_a[0];
    *ay = acc_param_k[1] * *ay + acc_param_a[1];
    *az = acc_param_k[2] * *az + acc_param_a[2]; //校准

    // 使用卡尔曼滤波器对数据进行滤波
    *ax = (short)kalman_filter(&accel_x_filter, (float)*ax);
    *ay = (short)kalman_filter(&accel_y_filter, (float)*ay);
    *az = (short)kalman_filter(&accel_z_filter, (float)*az);
  }
  return res;
}
u8 ree;
// I2C写入数据
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
  u8 i;
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
  if (MPU_IIC_Wait_Ack()) // 等待应答
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Send_Byte(reg); // 写寄存器地址
  ree = MPU_IIC_Wait_Ack();     // 等待应答
  for (i = 0; i < len; i++)
  {
    MPU_IIC_Send_Byte(buf[i]); // 发送数据
    if (MPU_IIC_Wait_Ack())    // 等待ACK
    {
      MPU_IIC_Stop();
      return 1;
    }
  }
  MPU_IIC_Stop();
  return 0;
}

// I2C读取数据
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
  if (MPU_IIC_Wait_Ack()) // 等待应答
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Send_Byte(reg); // 写寄存器地址
  MPU_IIC_Wait_Ack();     // 等待应答
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((addr << 1) | 1); // 发送器件地址+读命令
  MPU_IIC_Wait_Ack();     // 等待应答
  while (len)
  {
    if (len == 1)
      *buf = MPU_IIC_Read_Byte(0); // 读数据,发送nACK
    else
      *buf = MPU_IIC_Read_Byte(1); // 读数据,发送ACK
    len--;
    buf++;
  }
  MPU_IIC_Stop(); // 产生一个停止条件
  return 0;
}

// I2C写入一个字节数据
u8 MPU_Write_Byte(u8 reg, u8 data)
{
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
  if (MPU_IIC_Wait_Ack()) // 等待应答
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Send_Byte(reg); // 写寄存器地址
  MPU_IIC_Wait_Ack();     // 等待应答
  MPU_IIC_Send_Byte(data); // 发送数据
  if (MPU_IIC_Wait_Ack()) // 等待ACK
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Stop();
  return 0;
}

// I2C读取一个字节数据
u8 MPU_Read_Byte(u8 reg)
{
  u8 res;
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
  MPU_IIC_Wait_Ack();                   // 等待应答
  MPU_IIC_Send_Byte(reg);               // 写寄存器地址
  MPU_IIC_Wait_Ack();                   // 等待应答
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((MPU_ADDR << 1) | 1); // 发送器件地址+读命令
  MPU_IIC_Wait_Ack();                   // 等待应答
  res = MPU_IIC_Read_Byte(0);           // 读取数据,发送nACK
  MPU_IIC_Stop();                      // 产生一个停止条件
  return res;
}
