

#ifndef _LOCK_PROC_H
#define _LOCK_PROC_H

//����������
typedef enum
{
	DRAG_LOCK_TYPE = 0x01,   //ǣ����
	ELEC_LOCK_TYPE = 0x02,    //�����
	DZ_LOCK_TYPE   = 0X03,     //������
	MAX_LOCK_TYPE = 0x03,
	ZJ_LOCK_TYPE
	
	
}LOCKTYPE;

#define u8 unsigned char
//��������Ϣλ
extern u8 OnDrSgn,doorcommsendFlag;


//��ȡ���������������
extern void GetMemLockType(void);
//��������������
extern void SaveLockType(void);
//�������ƹ���
extern void StartLock_Proc(u8 fOn);
//�ȴ������������Ͽ�
extern void WaitToStopLock(void);
//��������
extern void DLockCtrl(void);
extern void DLockCtrl2IOWay(void);
#endif


