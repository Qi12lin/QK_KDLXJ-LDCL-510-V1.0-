#include "typedef.h"

#ifndef _APP_CONTROLCOMM_H
#define _APP_CONTROLCOMM_H (1)


//ͨ�����ݵĸ�ʽ
//��ʾ�巢�͵����ݸ�ʽ
typedef  struct 
{
	vu8 stationNo;  //վ��
	vu8 datNum;     //���ݸ���,������վ�ţ������ݸ���
	vu8 stateFlag1;  //״̬��־
	vu8 wayFlag;    // 
	vu8 freqH;         //��ʾ�巢��Ƶ�ʸ�8λ
	vu8 freqL;         //��ʾ�巢��Ƶ�ʵ�8λ
	vu8 speedIncH;    //����������8λ
	vu8 speedIncL;    //����������8λ   
	vu8 speedDecH;    //����������8λ
	vu8 speedDecL;    //����������8λ 		
	vu8 motorBasefreq; //�����Ƶ��8λ
	vu8 motorRatedCur; //��������
	vu8 ReserveByte;   //Ԥ���ֽ�
	vu8 verifyValue;   //У��ֵ
}COMM_TEND_FORMAT;

//��ʾ����յ����ݸ�ʽ
typedef  struct
{
	u8 stationNo;  //վ��
	u8 datNum;     //���ݸ���
	u8 transState;  //��Ƶ����״̬��־
	u8 signalByte;  //�źţ�ģ����ϣ����źŵ�
	u8 motorSpeedH; //����ٶȵĸ�8λ
	u8 motorSpeedL; //����ٶȵĵ�8λ
	u8 transFreqH;  //��Ƶ����ǰ���Ƶ�ʸ�8λ
	u8 transFreqL;  //��Ƶ����ǰ���Ƶ�ʵ�8λ
	u8 transAlarmInfo; //��Ƶ��������Ϣ
	u8 ReserveByte;   //Ԥ���ֽ�
	u8 backRatedCur;  //�������������
	u8 ReserveByte2[2];   //Ԥ���ֽ�
	u8 verifyValue;   //У��ֵ
}COMM_RECE_FORMAT;


typedef  struct
{
		vu8 sendEnable:1; //����ʹ��λ
		vu8 receEnable:1; //����ʹ��λ
		vu8 comm_failure:1; //ͨ��ʧ�ܱ�־ 
		vu8 receComplete:1; //������ɱ�־
}COMM_FLAG;

extern COMM_FLAG comFlag;
//extern TIME_TYPE txTime; //�������ݶ�ʱ��

//
#define SENDDELAYTIME  300  //ͨѶ�����Ǽ����������ʱ��
#define COMMERRTIME 5000
extern u16 commTm;        //ͨѶ�����ж϶�ʱ��
extern u16 comSenderTm;   //��ʱ���Ͷ�ʱ��


//ͨ�ű�־λ
extern COMM_FLAG comFlag;
//��Ӧͨ�Ź��ϱ�־λ
extern vu8 commErrsignal;

extern vu8 tranStr[20]; //���ͻ�����
extern vu8 sendPos;
//���ջ�����
extern u8 OldReceStr[];
extern vu8 receStr[20];
extern vu8 LRC; //У������
extern u16	upFreqInc1,downFreqInc1;  //���ϼӼ�������

//����һ���ֽڵ�����
extern void Comm_Send_Byte(u8 byte);
//����һ���ֽڵ�����
extern u8 Comm_Rece_Byte(u8 *byte);
//����һ������
extern void Comm_Send_Block(u8 * dat, u8 length);
//����һ������
extern void Comm_Rece_Block(void);
//5s��ʱ����ʱ
extern void Counter_5s(void);
//200ms��ʱ����ʱ
extern void Counter_200ms(void);
//��ʼ��5s��200ms��ʱ��
extern void TM_5s_200ms_Init(void);
//�ڶ�ʱ���з���һ���ֽڵ�����
extern void SendByte(void);

//��ȡ����Ƶ��
extern u16 GetFKFreq(void);
//���÷�������
extern void SetTxDat(void);
//�������ݵ�����
extern void setSendData(void);
//��ʼ��ͨ������
extern void InitCommData(void);
//ͨ��ģ��
extern void CommModule(void);

//ͨ�Ź��̴���
//comFlag.sendEnable
extern void Comm_Proc(u8 _ucfSending);

#endif


