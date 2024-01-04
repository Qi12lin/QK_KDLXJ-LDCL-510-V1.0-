#include "typedef.h"

#ifndef _BUTTON_PROC_H
#define _BUTTON_PROC_H

extern void SET_Proc(void);  //设置键的响应过程
extern void ADDKey_Proc(void);  //加键的响应过程
extern void SUBKey_Proc(void);	//减键的处理过程
extern void ENTERKey_Proc(void); //确定键的处理过程
extern void CUTKey_Proc(void);   //切换键的处理过程
extern void ROTORKey_Proc(void);  //加键的响应过程
extern void TIMEKey_Proc(void);	//减键的处理过程
extern void SPEEDKey_Proc(void); //确定键的处理过程
extern void TEMPKey_Proc(void);   //切换键的处理过程

extern void STOPKey_Proc(void);  //停止键的处理过程
extern void Comb1_Proc(void);//组合键1的过程处理
extern void OPENKey_Proc(void); //开门键过程

#endif
