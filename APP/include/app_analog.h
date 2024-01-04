#include "typedef.h"

#ifndef _ANALOG_H
#define _ANLALG_H

#define ADC_SAMPLE_TIMES	32//ADC采样次数


extern uint16_t adjustmin[] ;//PT100校准点最小值
extern uint16_t adjustmax[];//PT100校准点最大值
extern uint16_t adjustdefault[] ;//PT100校准点默认值
extern uint16_t Ni120_adjustmin[];//Ni120校准点最小值
extern uint16_t Ni120_adjustmax[];//Ni120校准点最大值
extern uint16_t Ni120_adjustdefault[];//Ni120校准点默认值
extern uint16_t thermo_adjustmin[] ;//热电偶校准点最小值
extern uint16_t thermo_adjustmax[];//热电偶校准点最大值
extern uint16_t thermo_adjustdefault[] ;//热电偶校准点默认值
extern uint16_t J_thermo_adjustmin[] ;//J_热电偶校准点最小值
extern uint16_t J_thermo_adjustmax[];//J_热电偶校准点最大值
extern uint16_t J_thermo_adjustdefault[] ;//J_热电偶校准点默认值

extern void InitAnalogData(void);
extern void CAL_ALANOG(void); 
extern void TEMP_ADJUST(void);
extern void OverTemperatureProtection(void);
extern void SensorUnusual(void);
#endif

