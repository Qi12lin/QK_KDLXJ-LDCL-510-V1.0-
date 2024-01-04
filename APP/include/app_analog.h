#include "typedef.h"

#ifndef _ANALOG_H
#define _ANLALG_H

#define ADC_SAMPLE_TIMES	32//ADC��������


extern uint16_t adjustmin[] ;//PT100У׼����Сֵ
extern uint16_t adjustmax[];//PT100У׼�����ֵ
extern uint16_t adjustdefault[] ;//PT100У׼��Ĭ��ֵ
extern uint16_t Ni120_adjustmin[];//Ni120У׼����Сֵ
extern uint16_t Ni120_adjustmax[];//Ni120У׼�����ֵ
extern uint16_t Ni120_adjustdefault[];//Ni120У׼��Ĭ��ֵ
extern uint16_t thermo_adjustmin[] ;//�ȵ�żУ׼����Сֵ
extern uint16_t thermo_adjustmax[];//�ȵ�żУ׼�����ֵ
extern uint16_t thermo_adjustdefault[] ;//�ȵ�żУ׼��Ĭ��ֵ
extern uint16_t J_thermo_adjustmin[] ;//J_�ȵ�żУ׼����Сֵ
extern uint16_t J_thermo_adjustmax[];//J_�ȵ�żУ׼�����ֵ
extern uint16_t J_thermo_adjustdefault[] ;//J_�ȵ�żУ׼��Ĭ��ֵ

extern void InitAnalogData(void);
extern void CAL_ALANOG(void); 
extern void TEMP_ADJUST(void);
extern void OverTemperatureProtection(void);
extern void SensorUnusual(void);
#endif

