#include "kalmen_filte.h"

/**
 *kalman_init - �������˲�����ʼ��
 *@kalman_lcw���������˲����ṹ��
 *@init_x���������ĳ�ʼֵ
 *@init_p������״̬����ֵ���ķ���ĳ�ʼֵ
 */
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p)
{
    kalman_lcw->x = init_x;//�������ĳ�ʼֵ��������ֵһ�������ֵ���������ǣ�
    kalman_lcw->p = init_p;//����״̬����ֵ���ķ���ĳ�ʼֵ
    kalman_lcw->A = 1;
    kalman_lcw->H = 1;
    kalman_lcw->q = 10e-2;//10e-6;//2e2;predict noise convariance Ԥ�⣨���̣��������� ʵ�鷢���޸����ֵ��Ӱ����������
    kalman_lcw->r = 4e2;//10e-5;//�������۲⣩���������������Ϊ�������Է����ǣ�
    //���������ǲ�����ͳ��һ��ʱ���ڵ�������������ݡ����ݻ������̬�ֲ���
    //��3��ԭ��ȡ��̬�ֲ���(3��)^2��Ϊr�ĳ�ʼ��ֵ
}

/**
 *kalman_filter - �������˲���
 *@kalman_lcw:�������ṹ��
 *@measure������ֵ
 *�����˲����ֵ
 */
float kalman_filter(kalman_struct *kalman_lcw, float measure)
{
    /* Predict */
    kalman_lcw->x = kalman_lcw->A * kalman_lcw->x;
    kalman_lcw->p = kalman_lcw->A * kalman_lcw->A * kalman_lcw->p + kalman_lcw->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement */
    kalman_lcw->gain = kalman_lcw->p * kalman_lcw->H / (kalman_lcw->p * kalman_lcw->H * kalman_lcw->H + kalman_lcw->r);
    kalman_lcw->x = kalman_lcw->x + kalman_lcw->gain * (measure - kalman_lcw->H * kalman_lcw->x);
    kalman_lcw->p = (1 - kalman_lcw->gain * kalman_lcw->H) * kalman_lcw->p;

    return kalman_lcw->x;
}



