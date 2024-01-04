

#ifndef _LOCK_PROC_H
#define _LOCK_PROC_H

//门锁的类型
typedef enum
{
	DRAG_LOCK_TYPE = 0x01,   //牵引锁
	ELEC_LOCK_TYPE = 0x02,    //电磁锁
	DZ_LOCK_TYPE   = 0X03,     //电子锁
	MAX_LOCK_TYPE = 0x03,
	ZJ_LOCK_TYPE
	
	
}LOCKTYPE;

#define u8 unsigned char
//开门锁信息位
extern u8 OnDrSgn,doorcommsendFlag;


//获取保存的门锁的类型
extern void GetMemLockType(void);
//保存门锁的类型
extern void SaveLockType(void);
//门锁控制过程
extern void StartLock_Proc(u8 fOn);
//等待门锁开关量断开
extern void WaitToStopLock(void);
//门锁控制
extern void DLockCtrl(void);
extern void DLockCtrl2IOWay(void);
#endif


