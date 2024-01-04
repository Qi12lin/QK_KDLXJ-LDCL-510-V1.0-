#include "typedef.h"

u8 g_ucfDispXg=0; //离心力转换
//状态指示灯LED端口
#define LED1_PORT	 GPIOB
#define LED2_PORT	 GPIOB

//状态指示灯的引脚
#define LED1_PIN	 GPIO_Pin_0
#define LED2_PIN	 GPIO_Pin_1

//状态指示灯的时钟
#define GPIO_CLK_LED1		 RCC_AHB1Periph_GPIOB
#define GPIO_CLK_LED2		 RCC_AHB1Periph_GPIOB

extern  signed int watch3_temp,watch4_temp;
u8 DOT,LED1_data,LED2_data;
//LEDiO的初始化
void InitLed(void)
{
	GPIO_InitTypeDef TypeDef;
 
	//LED GPIO时钟配置
 	RCC_APB2PeriphClockCmd(GPIO_CLK_LED1 | GPIO_CLK_LED2 , ENABLE);

	TypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	TypeDef.GPIO_Mode =	GPIO_Mode_OUT;

	GPIO_ResetBits(LED1_PORT, LED1_PIN);
//	GPIO_SetBits(LED2_PORT, LED2_PIN);

	//LED1 灯
	TypeDef.GPIO_Pin = LED1_PIN;
	GPIO_Init(LED1_PORT, &TypeDef);
	
	//LED2 灯
	TypeDef.GPIO_Pin = LED2_PIN;
	GPIO_Init(LED2_PORT, &TypeDef);
}

//设置灯的状态
void SetLedState(LED_DefType led, u8 fOn)
{
	switch(led)
	{
	case LED1:
		if(fOn)  LED1_data =1;//GPIO_ResetBits(LED1_PORT, LED1_PIN);
		else     LED1_data =0;//GPIO_SetBits(LED1_PORT, LED1_PIN);
	break;
	case LED2:
		if(fOn)   LED2_data =1;//GPIO_SetBits(LED2_PORT, LED2_PIN);
		else    LED2_data =0;//GPIO_ResetBits(LED2_PORT, LED2_PIN);
	break;
	default:
	break;
	}
}
//--------------------------------------------------------------------------------------------------
const u8 Tab_LED[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f, //"0"  "1"   "2"  "3"  "4"  "5"  "6"  "7"  "8"  "9" 
							0x77,0x7c,0x39,0x5e,0x79,0x71,0x80,0x40,0x49,0x38,0x00,0x73,0x76,0x6e,0x54, 0x78,//25
							//"A"  "b"  "C"  "d"  "E"  "F"  "."  "-"  "?" "L"  "??" "P"  "H"  "y"   "n"  "t"
							//10	11	12	  13   14   15   16   17    18   19   20   21   22   23    24   25}; 	
							0x50,0x5c,0x10,0x58,0x1C,0x3e,
							0x01,0x02,0x04,0x08,0x10,0x20}; //40 "N" 41 "V"
                      
                      				 		  
			 			    

u8 disp_LED[10] ={1,2,3,4,5,6,7,8,9,10,};

#define CLK_PORT GPIOB
#define CLK_PIN  GPIO_Pin_2



#define STB_PORT GPIOB
#define STB_PIN  GPIO_Pin_0


#define DIO_PORT GPIOE
#define DIO_PIN  GPIO_Pin_8



#define TM1638_CLK_ON  GPIO_SetBits(CLK_PORT,CLK_PIN)  
#define TM1638_CLK_OFF GPIO_ResetBits(CLK_PORT,CLK_PIN)  

#define TM1638_STB_ON  GPIO_SetBits(STB_PORT,STB_PIN)  
#define TM1638_STB_OFF GPIO_ResetBits(STB_PORT,STB_PIN)  


#define TM1638_DIO_ON  GPIO_SetBits(DIO_PORT,DIO_PIN)  
#define TM1638_DIO_OFF GPIO_ResetBits(DIO_PORT,DIO_PIN)  

#define  TM1638_DIO  GPIO_ReadInputDataBit(DIO_PORT, DIO_PIN)

//#define TM1628_DIO_OUT() DIO_PORT->DDR |=0x10
//#define TM1628_DIO_IN()  DIO_PORT->DDR &=0xEF
void TM1638_delay(u16 _iN)
{
	u16 i;
	
	for(i =0; i<_iN ;i++);

}
void TM1638_DIO_OUT(void)
{
	GPIO_InitTypeDef TypeDef;
	
	TypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	TypeDef.GPIO_Mode =	GPIO_Mode_OUT;

	TypeDef.GPIO_Pin = DIO_PIN;
	GPIO_Init(DIO_PORT, &TypeDef);

}
void TM1638_DIO_IN(void)
{
	GPIO_InitTypeDef TypeDef;
	
	TypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	TypeDef.GPIO_Mode =	GPIO_Mode_IN;

	TypeDef.GPIO_Pin = DIO_PIN;
	GPIO_Init(DIO_PORT, &TypeDef);

}

//???LED???
void TM1638_IO_Init(void)
{
		GPIO_InitTypeDef TypeDef;
 
	//LED GPIO时钟配置
 	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE , ENABLE);

	TypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	TypeDef.GPIO_Mode =	GPIO_Mode_OUT;

	TypeDef.GPIO_Pin = CLK_PIN;
	GPIO_Init(CLK_PORT, &TypeDef);

	TypeDef.GPIO_Pin = STB_PIN;
	GPIO_Init(STB_PORT, &TypeDef);

	TypeDef.GPIO_Pin = DIO_PIN;
	GPIO_Init(DIO_PORT, &TypeDef);	

		TM1638_STB_ON;
}

//------------------------------------------
// ?TM1628??8???,?????
//------------------------------------------						  
void TM1638_WriteByte(u8 Data)
{
	u8 i;
	for(i = 0; i < 8; i++)
	{
		if(Data & 0x01) TM1638_DIO_ON;
		else TM1638_DIO_OFF;
		
		TM1638_CLK_OFF;
		TM1638_delay(50);

		TM1638_CLK_ON;
	  TM1638_delay(50);
		Data = Data >> 1;
	}
}

//------------------------------------------
// ?TM1628????	    
//------------------------------------------			
void TM1638_WriteCommand(u8 Com)
{

		TM1638_STB_ON;
		TM1638_delay(30);

		TM1638_STB_OFF;
	TM1638_delay(10);
	TM1638_WriteByte(Com);
}
u8 TM1638_KEY[2] = {0,0};

void TM1638_Display(void)
{
	u8 i;
	u8 Display_DotTemp;
	u8 TM1638TAB[8];
	
	TM1638_WriteCommand(0x03); //??????,(0x03)7?10???,(0x00)4?13???
	TM1638_WriteCommand(0x40); //??????,???????1??
	TM1638_WriteCommand(0xc0); //??????,?00H??

	for(i = 0; i <8; i++) //??????
	{
		if(i==4 && g_ucfDispXg ==1)TM1638TAB[i] = Tab_LED[disp_LED[i]] | 0x80;
		else if(i==6 && DOT ==1)TM1638TAB[i] = Tab_LED[disp_LED[i]] | 0x80;
		else TM1638TAB[i] = Tab_LED[disp_LED[i]];//Tab_LED[LED[i]] | 0x80;
	}
	
	for(i = 0; i <8; i++) //??????
	{
		
//		if(i< 4)
//		{
//			if (i == 1 && DotBit == 3 )TM1628TAB[i] = Tab_LED[LED[i]] | 0x80;
//			else if (i == 2 && DotBit == 2 ) TM1628TAB[i] = Tab_LED[LED[i]] | 0x80;
//			else if (i == 3 && DotBit == 4 ) TM1628TAB[i] = Tab_LED[LED[i]] | 0x80;  
//			//else if (i == 2 && DotBit == 0x32) TM1628TAB[i] = Tab_LED[LED[i]] | 0x80;  
//			else 	TM1628TAB[i] = Tab_LED[LED[i]];
//			TM1628TAB[i] = Tab_LED[LED[i]];
//		}
//		else
//		{
//			TM1628TAB[i] = Tab_LED[LED[i]];
//		}
		//TM1628TAB[i] = 0xFF;
		TM1638_WriteByte((TM1638TAB[0]>>i&0x01) |(((TM1638TAB[1]>>i)&0x01)<<1)|(((TM1638TAB[2]>>i)&0x01)<<2)|
		        (((TM1638TAB[3]>>i)&0x01)<<3)|(((TM1638TAB[4]>>i)&0x01)<<4)|(((TM1638TAB[5]>>i)&0x01)<<5)|
						(((TM1638TAB[6]>>i)&0x01)<<6)|(((TM1638TAB[7]>>i)&0x01)<<7));
		if(i ==0 && LED1_data ==1)TM1638_WriteByte(0x01);   //运行指示灯
		else if(i ==1 && LED2_data ==1)TM1638_WriteByte(0x01); //停止指示灯
		else TM1638_WriteByte(0x00);
		
	}
	TM1638_WriteCommand(0x8C); //??????,??????????4/16

	TM1638_STB_ON;
}


//------------------------------------------
//??TM1628???? 
//------------------------------------------		
u8 key_data[2];

void TM1638_ReadKey(void)
{
	u8 i,j;
	TM1638_WriteCommand(0x42); //?????
	
	TM1638_DIO_IN();

	for(j = 0; j < 2; j++)//???????
	{
		for(i = 0; i < 8; i++)
		{	
			TM1638_KEY[j] = TM1638_KEY[j] >> 1;
			
			TM1638_CLK_OFF;
			TM1638_delay(50);
			
			TM1638_CLK_ON;
			TM1638_delay(2500);
			if(TM1638_DIO == 1) TM1638_KEY[j] = TM1638_KEY[j] | 0X80;
			else {}

			if (TM1638_KEY[j] == 0xff) 
				TM1638_KEY[j] = 0;//??????????????1?????
			else {}
		}
		key_data[j] =TM1638_KEY[j];
	}

		TM1638_DIO_OUT();
		
		TM1638_STB_ON;
}
//--------------------------------------
//LED显示数据万千百十个
void str_to_LED(u8 *_ucpDesStr, u8 _ucBitNum, u16 _uiData)
{
	u8 i;
	static u16 Data_temp;
	Data_temp = _uiData;
	for(i=0;i<_ucBitNum;i++)
	{
		_ucpDesStr[_ucBitNum-i-1] =Data_temp%10;
		Data_temp /=10;
	}
}
//-------------------
//温度数字显示
void Temp_to_LED(u8 *_ucpDesStr, int16_t _iData)
{
	u8 i;
	static int16_t Data_temp;
	//_iData =-123;
	Data_temp = _iData;
	if(_iData >=0)
	{
		DOT =1;
		for(i=0;i<3;i++)
		{
			_ucpDesStr[2-i] =Data_temp%10;
			Data_temp /=10;
		}
		if(_iData<100) _ucpDesStr[0] =20;
		
	}
	else 
	{
		Data_temp = 0-_iData;
		for(i=0;i<3;i++)
		{
			_ucpDesStr[2-i] =Data_temp%10;
			Data_temp /=10;
		}
		if((0-_iData)>100)
		{			
			_ucpDesStr[2] =_ucpDesStr[1];
			_ucpDesStr[1] =_ucpDesStr[0];
			_ucpDesStr[0] =17;
			DOT =0;
		}
		else 
		{
			_ucpDesStr[0] =17;
			DOT =1; 
		}
	}
}
//-----------------------------
//LED显示数据
//----------------------
extern vu16 t_RemainTime;
void LED_Display(void)
{
	u8 ucLen;

	//显示实际转速disp_LED

//	watch1 = 10000;//OldReceStr[6];
	
	if(g_ucfDispXg == 0)
	{
		//实时转速
		str_to_LED(disp_LED, 5, runParam.Speed);
		if(runParam.Speed<10000) disp_LED[0] =20;
		if(runParam.Speed<1000) disp_LED[1] =20;
		if(runParam.Speed<100) disp_LED[2] =20;
		if(runParam.Speed<10) disp_LED[3] =20;
	}
	else if(g_ucfDispXg == 1)
	{
		//实时离心力
		//速度转换为离心力
		
		str_to_LED(disp_LED, 5, runParam.RCF);
		if(runParam.RCF<10000) disp_LED[0] =20;
		if(runParam.RCF<1000) disp_LED[1] =20;
		if(runParam.RCF<100) disp_LED[2] =20;
		if(runParam.RCF<10) disp_LED[3] =20;
	}

	//显示时间
//	str_to_LED(&disp_LED[5], 2, t_RemainTime/60);
//	str_to_LED(&disp_LED[7], 2, t_RemainTime%60);
	//显示温度
	
	//runParam.temprature =9;
	if(runParam.temprature <=-260 || runParam.temprature>=1100)
	{
		disp_LED[5] = 17;
		disp_LED[6] = 17;
		disp_LED[7] = 17;
	//	disp_LED[12] = 17;
		
	}
	else Temp_to_LED(&disp_LED[5], runParam.temprature);
	
//	if(T_flicker >= flicker/2 && runFlag.curRunFlag == 1  && runFlag.arriveSpeedFlag ==1) DOT =0;
//	else DOT =1;
}
