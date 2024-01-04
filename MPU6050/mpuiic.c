//��� MPU6050 ������ģ��� I2C ͨ����������

#include "mpuiic.h"
#include "delay.h"



#define MPU_RCC_PORT RCC_AHB1Periph_GPIOD

#define MPU_SCL_PIN GPIO_Pin_5
#define MPU_SDA_PIN GPIO_Pin_6
#define MPU_GPIO GPIOD

//#define MPU_RCC_PORT RCC_AHB1Periph_GPIOA

//#define MPU_SCL_PIN GPIO_Pin_10
//#define MPU_SDA_PIN GPIO_Pin_9
//#define MPU_GPIO GPIOA

//#if 1

//	#define I2C_SCL_1_MPU()  		GPIO_SetBits(MPU_GPIO, MPU_SCL_PIN)
//	#define I2C_SCL_0_MPU()  		GPIO_ResetBits(MPU_GPIO, MPU_SCL_PIN)

//	#define I2C_SDA_1_MPU()  		GPIO_SetBits(MPU_GPIO, MPU_SDA_PIN)
//	#define I2C_SDA_0_MPU()  		GPIO_ResetBits(MPU_GPIO, MPU_SDA_PIN)

//	#define I2C_SDA_READ_MPU()  GPIO_ReadInputDataBit(MPU_GPIO, MPU_SDA_PIN)

//#else

//	#define I2C_SCL_1_MPU()  		MPU_GPIO->BSRR = MPU_SCL_PIN				
//	#define I2C_SCL_0_MPU()  		MPU_GPIO->BRR = MPU_SCL_PIN			
//	
//	#define I2C_SDA_1_MPU()  		MPU_GPIO->BSRR = MPU_SDA_PIN		
//	#define I2C_SDA_0_MPU()  		MPU_GPIO->BRR = MPU_SDA_PIN		
//	
//	#define I2C_SDA_READ_MPU()  ((MPU_GPIO->IDR & MPU_SDA_PIN) != 0)

//#endif
	
//��ʱ����
//static void MPU_i2c_Delay(void)
//{
//	u8 i;
//	for (i = 0; i < 10; i++);
//}

 

void MPU_IIC_Delay(void)
{
	Delay_us(10);
}
//��ʼ��IIC
void MPU_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(MPU_RCC_PORT,ENABLE);//��ʹ������IO PORTBʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = MPU_SCL_PIN | MPU_SDA_PIN;  //GPIO_Pin_5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MPU_GPIO, &GPIO_InitStructure);
	
  GPIO_SetBits(MPU_GPIO,MPU_SCL_PIN|MPU_SDA_PIN);	
}


void MPU_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = MPU_SDA_PIN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MPU_GPIO, &GPIO_InitStructure);

}

void MPU_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = MPU_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MPU_GPIO, &GPIO_InitStructure);

}


/*
�� I2C ͨ���У���ʼ�ź����� SDA ���� SCL ��Ϊ�ߵ�ƽʱ��
�ɵ͵�ƽ��ߵ�ƽת����γɵġ����ת��Ĺ�����Ҫһ����ʱ�䣬
����� SCL ���ɵ͵�ƽ��ߵ�ƽת��֮����Ҫһ������ʱ��
�ȴ� SDA �ߵ�״̬�ȶ��������ٽ�����һ���Ĳ�����
������ܻᵼ�� I2C ���߳��ִ���
*/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_SCL=1;
	
	//��ʱ
	MPU_IIC_Delay();
	
	MPU_IIC_SDA=0;
	//��ʱ
	MPU_IIC_Delay();
	
	MPU_IIC_SCL=0;
}

void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;
	
	MPU_IIC_Delay();
	
	MPU_IIC_SCL=1;
	MPU_IIC_SDA=1;
	
	MPU_IIC_Delay();
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
/*
�ȴ� I2C �豸����Ӧ���źš������Ǹú����ķ�����
�� SDA �˿�����Ϊ����ģʽ
���ȣ��ں�����ͷ���� SDA �˿�����Ϊ����ģʽ���Ա��ڽ��� I2C �豸���͵�Ӧ���źš�
���͸ߵ�ƽ�� SDA �˿�
���ţ��� SDA �˿����ߣ����ҵ��� MPU_IIC_Delay() ��������һ������ʱ��
�� SCL �˿�����
�� SCL �˿����ߣ����ҵ��� MPU_IIC_Delay() ��������һ������ʱ��
�ȴ� I2C �豸����Ӧ���ź�
����һ�� while ѭ����ѭ�������ж� SDA �˿ڵ�״̬����� SDA �˿�ʼ�ձ���Ϊ�ߵ�ƽ��˵�� I2C �豸û����Ӧ����ʱ ucErrTime ��������һ�������������ֵ������ 250��˵�� I2C �豸û����Ӧ���͵��� MPU_IIC_Stop() ��������ֹͣ�źţ�Ȼ�󷵻�һ�������� 1��
�� SCL �˿�����
��� I2C �豸�ɹ�������Ӧ���źţ��ͽ� SCL �˿����ͣ��Ա��ڽ�����һ���Ĳ�����
������ȷ�� 0
��󣬷���һ����ȷ�� 0����ʾ�ȴ�Ӧ��ɹ���
*/

u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();      //SDA����Ϊ����  
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
	
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();	
 
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ��
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}					 				     

/*
������ I2C �豸����һ���ֽڵ����ݡ�

�� SDA �˿�����Ϊ���ģʽ
���ȣ��ں�����ͷ���� SDA �˿�����Ϊ���ģʽ���Ա����� I2C �豸�������ݡ�
�� SCL �˿�����
�� SCL �˿����ͣ��Ա��ڽ������ݷ��͵�׼��������

��������
ͨ��һ�� for ѭ������Ҫ���͵����ݵ�ÿһλ���η��ͳ�ȥ��
��ÿһ��ѭ���У������жϵ�ǰλ��ֵ�� 0 ���� 1������� 1���ͽ� SDA �˿����ߣ�
����ͽ� SDA �˿����ͣ�

Ȼ��Ҫ���͵�����������λ���Ա��ڷ�����һλ���ݡ�

�� SCL �˿�����
��ÿһ��ѭ���У��� SCL �˿����ߣ��Ա��� I2C �豸��ȡ��ǰλ�����ݡ�
��ʱ
��ÿһ��ѭ���У�����Ҫ����һ������ʱ����ȷ�� I2C �豸�ܹ���ȷ�ض�ȡ�����ݡ�
����ʹ���� MPU_i2c_Delay() ����������ʱ��
�� SCL �˿�����
��ÿһ��ѭ���У��� SCL �˿����ͣ��Ա��ڽ�����һλ���ݵķ��͡�

*/

void MPU_IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	MPU_SDA_OUT(); 	    
	MPU_IIC_SCL=0;
	for(t=0;t<8;t++)
	{
		MPU_IIC_SDA = (txd&0x80)>>7;
		txd<<=1; 
		MPU_IIC_Delay();	  
		MPU_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU_IIC_SCL=0;
		MPU_IIC_Delay();
	}	 
} 
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK

/*
���ڴ� I2C �豸��ȡһ���ֽڵ����ݡ������Ǹú����ķ�����

�� SDA �˿�����Ϊ����ģʽ
���ȣ��ں�����ͷ���� SDA �˿�����Ϊ����ģʽ���Ա��ڽ��� I2C �豸���͵����ݡ�

ѭ����ȡ����
ͨ��һ�� for ѭ������ I2C �豸���͵����ݵ�ÿһλ���ζ�ȡ������
��ÿһ��ѭ���У����Ƚ� SCL �˿����ͣ��Ա���׼���������ݣ�

Ȼ�󽫽��յ�������������λ���Ա��ڽ�����һλ���ݣ�

�����жϵ�ǰλ��ֵ�� 0 ���� 1������� 1���ͽ����յ������ݵ����λ����Ϊ 1��
���򲻽����κβ�������� SCL �˿����ߣ��Ա��� I2C �豸������һλ���ݡ�

���� ACK ���� NACK
�ڶ�ȡ������֮����Ҫ���� ack ������ֵ���������� ACK ���� NACK��
��� ack ����Ϊ 1���ͷ��� ACK����ʾ����������һ���ֽڵ����ݣ�
������ NACK����ʾֹͣ�������ݡ�

���ؽ��յ�������
��󣬽����յ���������Ϊ�����ķ���ֵ���ء�
*/

u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
    MPU_IIC_SCL=0; 
    MPU_IIC_Delay();
		MPU_IIC_SCL=1;
		
    receive<<=1;
        
		if(MPU_READ_SDA)
			receive++;   
		
		MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK   
    return receive;
}


























