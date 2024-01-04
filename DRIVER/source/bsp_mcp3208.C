#include "typedef.h"
/**********************************************************************************************************
*	函 数 名: MCP3208_Init  
*	功能说明: MCP3208初始化
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/

void MCP3208_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIO的时钟
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;//PA1,SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5;//PA1,SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 ;//PA0-MISO，输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮动输入模式	GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	MCP3208_CS_ON();
	MCP3208_CLK_OFF();
}
/**********************************************************************************************************
*	函 数 名: MCP3208_delay
*	功能说明: 延时		F103主频72M，F407主频168M
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void MCP3208_delay(u16 _iN)
{
	u16 i;
	
	for(i =_iN*3;i>0;i--);
}
/**********************************************************************************************************
*	函 数 名: mcp3208_SendByte
*	功能说明: 读数据
*	形    参：reg
*	返 回 值: data
**********************************************************************************************************/
u8 mcp3208_SendByte(u8 reg)
{
	u8 data=0;
	u8 i=0;	
	for(i=8; i>0; i--)
	{
		if((reg&0x80) == 0x80) MCP3208_DOUT_ON();//高位在前
		else MCP3208_DOUT_OFF();		
		reg <<= 1;	
		data <<= 1;			
		if(MCP3208_DIN_READ() == 1) data|=0x01;//if ((PIND & 0x80) == 0x80)	data|=0x80;
		else data &= 0xFE;		
		MCP3208_CLK_ON();//clk=1;
		MCP3208_delay(10);
		MCP3208_CLK_OFF();//clk=0;
		MCP3208_delay(10);			
	}
	return(data);
}
/**********************************************************************************************************
*	函 数 名: MCP3208_SAMPLE  
*	功能说明: 读取一个通道AD
*	形    参：channels
*	返 回 值: r_data,AD转换结果
**********************************************************************************************************/
INT16U MCP3208_SAMPLE(uint8_t channels)
{
	INT16U r_data=0;
	uint8_t adch;
			  
	MCP3208_CS_OFF();//cs=0;
	MCP3208_delay(20);	
	if(channels >= 3)//4通道以下发7
	{
		adch=channels-4;	
		r_data=mcp3208_SendByte(0x07);			
		r_data=((INT16U)mcp3208_SendByte(adch<<6)&0x0F)<<8;	 //高4位
		r_data+=mcp3208_SendByte(0xFF);							 //低8位
	}
    else 
	{
		adch=channels;
		r_data=mcp3208_SendByte(0x06);	
		r_data=((INT16U)mcp3208_SendByte(adch<<6)&0x0F)<<8;	 //高4位
		r_data+=mcp3208_SendByte(0xFF);							 //低8位
	} 
	MCP3208_CS_ON();//cs=1;		
	return r_data;
}
//-------------------------------------------------- 

