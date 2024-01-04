#ifndef  _KALMAN_FILTER_H
#define  _KALMAN_FILTER_H

//1d�������˲�
typedef struct {
    float x;  // state 
    float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q) 
    float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)   
    float q;  // process(predict) noise convariance Э����
    float r;  //������������
    float p;  // estimated error convariance �������Э����
    float gain;
}kalman_struct;

void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p);
float kalman_filter(kalman_struct *kalman_lcw, float z_measure);

#endif  /*_KALMAN_FILTER_H*/


