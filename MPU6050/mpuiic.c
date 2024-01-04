//针对 MPU6050 陀螺仪模块的 I2C 通信驱动程序。

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
	
//延时代码
//static void MPU_i2c_Delay(void)
//{
//	u8 i;
//	for (i = 0; i < 10; i++);
//}

 

void MPU_IIC_Delay(void)
{
	Delay_us(10);
}
//初始化IIC
void MPU_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(MPU_RCC_PORT,ENABLE);//先使能外设IO PORTB时钟 
	
	GPIO_InitStructure.GPIO_Pin = MPU_SCL_PIN | MPU_SDA_PIN;  //GPIO_Pin_5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //设置为输出模式
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
在 I2C 通信中，起始信号是由 SDA 线在 SCL 线为高电平时，
由低电平向高电平转变而形成的。这个转变的过程需要一定的时间，
因此在 SCL 线由低电平向高电平转变之后，需要一定的延时，
等待 SDA 线的状态稳定下来后再进行下一步的操作，
否则可能会导致 I2C 总线出现错误。
*/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_SCL=1;
	
	//延时
	MPU_IIC_Delay();
	
	MPU_IIC_SDA=0;
	//延时
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

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
/*
等待 I2C 设备发送应答信号。以下是该函数的分析：
将 SDA 端口设置为输入模式
首先，在函数开头，将 SDA 端口设置为输入模式，以便于接收 I2C 设备发送的应答信号。
发送高电平到 SDA 端口
接着，将 SDA 端口拉高，并且调用 MPU_IIC_Delay() 函数进行一定的延时。
将 SCL 端口拉高
将 SCL 端口拉高，并且调用 MPU_IIC_Delay() 函数进行一定的延时。
等待 I2C 设备发送应答信号
进入一个 while 循环，循环体内判断 SDA 端口的状态，如果 SDA 端口始终保持为高电平，说明 I2C 设备没有响应，此时 ucErrTime 计数器加一。如果计数器的值超过了 250，说明 I2C 设备没有响应，就调用 MPU_IIC_Stop() 函数发送停止信号，然后返回一个错误码 1。
将 SCL 端口拉低
如果 I2C 设备成功发送了应答信号，就将 SCL 端口拉低，以便于进行下一步的操作。
返回正确码 0
最后，返回一个正确码 0，表示等待应答成功。
*/

u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();      //SDA设置为输入  
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
	MPU_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

//产生ACK应答
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
//不产生ACK应答		    
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
用于向 I2C 设备发送一个字节的数据。

将 SDA 端口设置为输出模式
首先，在函数开头，将 SDA 端口设置为输出模式，以便于向 I2C 设备发送数据。
将 SCL 端口拉低
将 SCL 端口拉低，以便于进行数据发送的准备工作。

发送数据
通过一个 for 循环，将要发送的数据的每一位依次发送出去。
在每一次循环中，首先判断当前位的值是 0 还是 1，如果是 1，就将 SDA 端口拉高，
否则就将 SDA 端口拉低；

然后将要发送的数据向左移位，以便于发送下一位数据。

将 SCL 端口拉高
在每一次循环中，将 SCL 端口拉高，以便于 I2C 设备读取当前位的数据。
延时
在每一次循环中，都需要进行一定的延时，以确保 I2C 设备能够正确地读取到数据。
这里使用了 MPU_i2c_Delay() 函数进行延时。
将 SCL 端口拉低
在每一次循环中，将 SCL 端口拉低，以便于进行下一位数据的发送。

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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK

/*
用于从 I2C 设备读取一个字节的数据。以下是该函数的分析：

将 SDA 端口设置为输入模式
首先，在函数开头，将 SDA 端口设置为输入模式，以便于接收 I2C 设备发送的数据。

循环读取数据
通过一个 for 循环，将 I2C 设备发送的数据的每一位依次读取出来。
在每一次循环中，首先将 SCL 端口拉低，以便于准备接收数据；

然后将接收到的数据向左移位，以便于接收下一位数据；

接着判断当前位的值是 0 还是 1，如果是 1，就将接收到的数据的最低位设置为 1，
否则不进行任何操作；最后将 SCL 端口拉高，以便于 I2C 设备发送下一位数据。

发送 ACK 或者 NACK
在读取完数据之后，需要根据 ack 参数的值来决定发送 ACK 还是 NACK。
如果 ack 参数为 1，就发送 ACK，表示继续接收下一个字节的数据，
否则发送 NACK，表示停止接收数据。

返回接收到的数据
最后，将接收到的数据作为函数的返回值返回。
*/

u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA设置为输入
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
        MPU_IIC_NAck();//发送nACK
    else
        MPU_IIC_Ack(); //发送ACK   
    return receive;
}


























