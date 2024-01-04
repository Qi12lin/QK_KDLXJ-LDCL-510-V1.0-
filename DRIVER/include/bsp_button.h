#include "typedef.h"

#ifndef _KEY_DRIVER_IO_H
#define _KEY_DRIVER_IO_H    (1)


//����
//��ֵ����
//����ֵԽС,���ȼ�Խ��

#ifndef _VK_KEY_TYPE
#define _VK_KEY_TYPE
extern vu32 keyValueTab;
//typedef enum
//{
//	VK_KEY1 	= 0x01,  //
//	VK_KEY2 	= 0x02,  //
//	VK_KEY3  	= 0x04,  //
//	VK_KEY4 	= 0x08,  //
//	VK_KEY5     = 0x10,  //
//	VK_KEY6 	= 0x20,  //
//	VK_KEY7  	= 0x40,  //
//	VK_KEY8		= 0x80,
//	VK_KEY9  	= 0x100,  //
//	VK_KEY10		= 0x200,
//	VK_ALL      = 0xff
//}VK_KeyType;
#define VK_KEY7 ((TM1638_KEY[0] &0x02) >>1)
#define VK_KEY1 ((TM1638_KEY[0] &0x04) >>2)
#define VK_KEY8 ((TM1638_KEY[0] &0x20) >>5)
#define VK_KEY2 ((TM1638_KEY[0] &0x40) >>6)
#define VK_KEY9 ((TM1638_KEY[1] &0x02) >>1)
#define VK_KEY3 ((TM1638_KEY[1] &0x04) >>2)
#define VK_KEY10 (keyValueTab &0x01)
#define VK_KEY4 ((keyValueTab &0x02)>>1)
#endif

#define KEY_NUM   7

//������������ɨ��ṹ
typedef struct
{
	vu32 key_msg;
	vu32 old_key_msg;					//֮ǰ�ļ���Ϣ
	vu32 cnt;							//������
}Key_Scan;

//������ʼ��
extern void keyInit(void);
extern void key_filter(void); //�����˲�����
extern u32 getKey(void);
extern u32 getKeyState(u32 vkKey);
extern u32 kbhit(void);
extern void key_Read(void);

#endif


