#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

// ����Kalman�˲���
#include "kalmen_filte.h"

#define gyro_raw_to_deg_s 0.0609756097561f // +-250��/s:131LSB/��/s   +-500��/s:65.5LSB/��/s   +-1000��/s:32.8LSB/��/s    +-2000��/s:16.4LSB/��/s
#define acc_raw_to_g 0.000244140625f        // +-2g : 16384LSB/g     +-4g : 8192LSB/g   +-8g : 4096LSB/g   +-16g : 2048LSB/g

// ���ٶȼ�У׼����
const float acc_param_k[3] = {0.9928, 1.0030, 0.9894};
const float acc_param_a[3] = {-0.0668, 0.0172, 0.0774};
const float gyro_offest[3] = {-0.96, 0.902, -1.05};

kalman_struct gyro_x_filter; // ������X���˲���
kalman_struct gyro_y_filter; // ������Y���˲���
kalman_struct gyro_z_filter; // ������Z���˲���

kalman_struct accel_x_filter; // ���ٶȼ�X���˲���
kalman_struct accel_y_filter; // ���ٶȼ�Y���˲���
kalman_struct accel_z_filter; // ���ٶȼ�Z���˲���

// MPU6050��ʼ������
u8 MPU_Init(void)
{
	u8 res;
  // ��ʼ��IIC����
	getvalue.OpenBPH = 1;
  MPU_IIC_Init();
  MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); // ��λMPU6050
  Delay_ms(100);
  MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); // ����MPU6050
  MPU_Set_Gyro_Fsr(3);                   // �����Ǵ�����,��2000dps
  MPU_Set_Accel_Fsr(0);                  // ���ٶȴ�����,��2g
  MPU_Set_Rate(6);                       // ���ò�����50Hz
  MPU_Write_Byte(MPU_INT_EN_REG, 0X00);   // �ر������ж�
  MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C��ģʽ�ر�
  MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);   // �ر�FIFO
  MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT���ŵ͵�ƽ��Ч

  // ��ʼ���������˲���
  kalman_init(&gyro_x_filter, 0, 1); // ��ʼֵΪ0����ʼ���Э����Ϊ1
  kalman_init(&gyro_y_filter, 0, 1);
  kalman_init(&gyro_z_filter, 0, 1);

  // ��ʼ�����ٶȼ��˲���
  kalman_init(&accel_x_filter, 0, 1);
  kalman_init(&accel_y_filter, 0, 1);
  kalman_init(&accel_z_filter, 0, 1);

  res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
  if (res == MPU_ADDR) // ����ID��ȷ
  {
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); // ����CLKSEL,PLL X��Ϊ�ο�
    MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // ���ٶ��������Ƕ�����
    MPU_Set_Rate(6);                        // ���ò�����Ϊ50Hz
  }
  else
    return 1;
  return 0;
}

// ���������������̷�Χ
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
  return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3);
}

// ���ü��ٶȴ����������̷�Χ
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
  return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3);
}

// �������ֵ�ͨ�˲���
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

// ���ò�����
u8 MPU_Set_Rate(u16 rate)
{
  u8 data;
  if (rate > 1000) rate = 1000; // ���Ʋ����ʲ�����1KHz
  if (rate < 4) rate = 4; // ���Ʋ����ʲ�����4Hz
  data = 1000 / rate; // ��������ʶ�Ӧ������ֵ
  data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); // �������ֵ�ͨ�˲���
  return MPU_Set_LPF(rate / 2); // �Զ�����LPFΪ�����ʵ�һ��
}

// ��ȡ�¶�ֵ
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

// ��ȡ����������
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

    // ʹ�ÿ������˲��������ݽ����˲�
    *gx = (short)kalman_filter(&gyro_x_filter, (float)*gx);
    *gy = (short)kalman_filter(&gyro_y_filter, (float)*gy);
    *gz = (short)kalman_filter(&gyro_z_filter, (float)*gz);
  }
  return res;
}

// ��ȡ���ٶȼ�����
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
    *az = acc_param_k[2] * *az + acc_param_a[2]; //У׼

    // ʹ�ÿ������˲��������ݽ����˲�
    *ax = (short)kalman_filter(&accel_x_filter, (float)*ax);
    *ay = (short)kalman_filter(&accel_y_filter, (float)*ay);
    *az = (short)kalman_filter(&accel_z_filter, (float)*az);
  }
  return res;
}
u8 ree;
// I2Cд������
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
  u8 i;
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((addr << 1) | 0); // ����������ַ+д����
  if (MPU_IIC_Wait_Ack()) // �ȴ�Ӧ��
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Send_Byte(reg); // д�Ĵ�����ַ
  ree = MPU_IIC_Wait_Ack();     // �ȴ�Ӧ��
  for (i = 0; i < len; i++)
  {
    MPU_IIC_Send_Byte(buf[i]); // ��������
    if (MPU_IIC_Wait_Ack())    // �ȴ�ACK
    {
      MPU_IIC_Stop();
      return 1;
    }
  }
  MPU_IIC_Stop();
  return 0;
}

// I2C��ȡ����
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((addr << 1) | 0); // ����������ַ+д����
  if (MPU_IIC_Wait_Ack()) // �ȴ�Ӧ��
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Send_Byte(reg); // д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();     // �ȴ�Ӧ��
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((addr << 1) | 1); // ����������ַ+������
  MPU_IIC_Wait_Ack();     // �ȴ�Ӧ��
  while (len)
  {
    if (len == 1)
      *buf = MPU_IIC_Read_Byte(0); // ������,����nACK
    else
      *buf = MPU_IIC_Read_Byte(1); // ������,����ACK
    len--;
    buf++;
  }
  MPU_IIC_Stop(); // ����һ��ֹͣ����
  return 0;
}

// I2Cд��һ���ֽ�����
u8 MPU_Write_Byte(u8 reg, u8 data)
{
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // ����������ַ+д����
  if (MPU_IIC_Wait_Ack()) // �ȴ�Ӧ��
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Send_Byte(reg); // д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();     // �ȴ�Ӧ��
  MPU_IIC_Send_Byte(data); // ��������
  if (MPU_IIC_Wait_Ack()) // �ȴ�ACK
  {
    MPU_IIC_Stop();
    return 1;
  }
  MPU_IIC_Stop();
  return 0;
}

// I2C��ȡһ���ֽ�����
u8 MPU_Read_Byte(u8 reg)
{
  u8 res;
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // ����������ַ+д����
  MPU_IIC_Wait_Ack();                   // �ȴ�Ӧ��
  MPU_IIC_Send_Byte(reg);               // д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();                   // �ȴ�Ӧ��
  MPU_IIC_Start();
  MPU_IIC_Send_Byte((MPU_ADDR << 1) | 1); // ����������ַ+������
  MPU_IIC_Wait_Ack();                   // �ȴ�Ӧ��
  res = MPU_IIC_Read_Byte(0);           // ��ȡ����,����nACK
  MPU_IIC_Stop();                      // ����һ��ֹͣ����
  return res;
}
