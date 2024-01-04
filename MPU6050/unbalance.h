

#ifndef _APP_BALANCE_H
#define _APP_BALANCE_H
#include "typedef.h"

#define  MAX_VALUE   25 
typedef enum
{
	
	START_NUMBER =0x31,
	VALUE_LOW =0X32,
	VALUE_HIGH,
	MAX_NUMBER,
}GET_VALUE;

typedef struct{
	
u16	VALUE_LOW[MAX_VALUE];
u16	VALUE_HIGH[MAX_VALUE];
u8 OpenBPH;
u8 BPHROTOR;//不平衡转子选择
}GET_VALU;

extern GET_VALU getvalue;

extern void Balance(void);
extern void Balance_Init(void);
extern void BalanceStart(void);
extern void BalanceCease(void);

extern int isBalanceCheckActive;
extern void Rotor_Parameter(int a, int b, int c,int d);

void Balance_rede(void);
void Balance_Storage(void);

#endif


