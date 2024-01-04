#include "typedef.h"
/**********************************************************************************************************
*	�� �� ��: MCP3208_Init  
*	����˵��: MCP3208��ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/

void MCP3208_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIO��ʱ��
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;//PA1,SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5;//PA1,SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 ;//PA0-MISO������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������ģʽ	GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	MCP3208_CS_ON();
	MCP3208_CLK_OFF();
}
/**********************************************************************************************************
*	�� �� ��: MCP3208_delay
*	����˵��: ��ʱ		F103��Ƶ72M��F407��Ƶ168M
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/
void MCP3208_delay(u16 _iN)
{
	u16 i;
	
	for(i =_iN*3;i>0;i--);
}
/**********************************************************************************************************
*	�� �� ��: mcp3208_SendByte
*	����˵��: ������
*	��    �Σ�reg
*	�� �� ֵ: data
**********************************************************************************************************/
u8 mcp3208_SendByte(u8 reg)
{
	u8 data=0;
	u8 i=0;	
	for(i=8; i>0; i--)
	{
		if((reg&0x80) == 0x80) MCP3208_DOUT_ON();//��λ��ǰ
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
*	�� �� ��: MCP3208_SAMPLE  
*	����˵��: ��ȡһ��ͨ��AD
*	��    �Σ�channels
*	�� �� ֵ: r_data,ADת�����
**********************************************************************************************************/
INT16U MCP3208_SAMPLE(uint8_t channels)
{
	INT16U r_data=0;
	uint8_t adch;
			  
	MCP3208_CS_OFF();//cs=0;
	MCP3208_delay(20);	
	if(channels >= 3)//4ͨ�����·�7
	{
		adch=channels-4;	
		r_data=mcp3208_SendByte(0x07);			
		r_data=((INT16U)mcp3208_SendByte(adch<<6)&0x0F)<<8;	 //��4λ
		r_data+=mcp3208_SendByte(0xFF);							 //��8λ
	}
    else 
	{
		adch=channels;
		r_data=mcp3208_SendByte(0x06);	
		r_data=((INT16U)mcp3208_SendByte(adch<<6)&0x0F)<<8;	 //��4λ
		r_data+=mcp3208_SendByte(0xFF);							 //��8λ
	} 
	MCP3208_CS_ON();//cs=1;		
	return r_data;
}
//-------------------------------------------------- 
