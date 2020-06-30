#ifndef FFT_H_INCLUDED
#define FFT_H_INCLUDED

#include <math.h>					//sin()/cos()
#include <stdio.h>					//int16_t
#include <stdint.h>					//int16_t

/*********************************************************************
                         ���ٸ���Ҷ�任C����
������飺�˺�����ͨ�õĿ��ٸ���Ҷ�任C���Ժ�������ֲ��ǿ�����²��ֲ���
          ��Ӳ�����˺����������������ʽ��ʾһ������������Ϊ��Ȼ˳��ĸ�
          ��������ʵ���ǿ�����鲿Ϊ0�������Ϊ����FFT�任����Ȼ˳���
          ����
ʹ��˵����ʹ�ô˺���ֻ����ĺ궨��FFT_N��ֵ����ʵ�ֵ����ĸı䣬FFT_N��
          Ӧ��Ϊ2��N�η��������������ʱӦ�ں��油0
�������ã�FFT(s);
��    �ߣ���˧��
ʱ    �䣺2010-2-20
��    ����Ver1.0
�ο����ף�

**********************************************************************/
//fft configuration
#define USE_FFT_TWD_LUT														//if pre-calculated twiddle factors are used
#define FFT_LOG2N	8
#define FFT_N 		(1ul<<FFT_LOG2N)											//128	                                                  //���帣��Ҷ�任�ĵ���
#define FFT_TYPE	double
//end fft configuration

#define PI 			3.1415926535897932384626433832795028841971               //����Բ����ֵ

typedef struct {
    FFT_TYPE real,imag;
} compx;                                    //����һ�������ṹ
//struct compx s[FFT_N];                                              //FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����

//calculat the look-up table - used only if USE_FFT_TWD_LUT is commented out
void lut_cal(void);

/*****************************************************************
����ԭ�ͣ�void FFT(struct compx *xin,int N)
�������ܣ�������ĸ�������п��ٸ���Ҷ�任��FFT��
���������*xin�����ṹ������׵�ַָ�룬struct��
*****************************************************************/
void fft_float(int16_t FFTi_LOG2N, compx *xin);
void fft_float2(int16_t FFTi_LOG2N, compx *xin);

/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fft_float.h"

#define FFT_N			(1<<FFT_LOG2N)
#define FFT_LOG2N		5

compx s[FFT_N];

int main(void) {
    int i;
    int t0, t1;

    srand(time(NULL));
    //generate the data
    for(i=0; i<FFT_N; i++) {                       	//���ṹ�帳ֵ
        s[i].real =	1.0+ \
        			2.0*cos(2*PI*1*(3.6/360+1.0*i/FFT_N))+ \
        			3.0*cos(2*PI*4*(1.8/360+1.0*i/FFT_N))+ \
        			4.0*cos(2*PI*6*(1.8/360+1.0*i/FFT_N))+ \
        			0.51*(rand()/32768.0 - 0.5); 		//ʵ��Ϊ���Ҳ�FFT_N���������ֵΪ1
        s[i].imag=0;                                //�鲿Ϊ0
    }

	//print out the time-domain data
	printf("time-domain data...\n");
	for (i=0; i<FFT_N; i++)
		printf("i=%4d, real = %10.7f, imag = %10.7f, amp= %10.5f\n", i, s[i].real, s[i].imag, sqrt(s[i].real*s[i].real + s[i].imag*s[i].imag));

	//perform fft
    t0=clock();
    for (i=0; i<1; i++) fft_float(s, FFT_N);                                        	//���п��ٸ���Ҷ�任	11212 clicks / 100000
    t1=clock();
    printf("t0=%5d, t1=%5d, t1-t0=%5d\n\r", t0, t1, t1-t0);

	//printf the fft data
	//printf("post-FFT data...\n");
	for (i=0; i<FFT_N/2; i++)
		printf("i=%4d, real = %10.7f, imag = %10.7f, amp= %10.5f\n", i, s[i].real/FFT_N, s[i].imag/FFT_N, sqrt(s[i].real*s[i].real + s[i].imag*s[i].imag)/((i==0)?FFT_N:(FFT_N/2)));

    //for(i=0; i<FFT_N; i++)                         	//��任������ģֵ�����븴����ʵ������
       //s[i].real=sqrt(s[i].real*s[i].real+s[i].imag*s[i].imag);

	//printf the fft data
	//printf("mod-FFT data...\n");
	//for (i=0; i<FFT_N; i++)
		//printf("i=%4d, amp = %10.7f\n", i, s[i].real, s[i].imag);
    //while(1);
    return 0;
}
*/

#endif // FFT_H_INCLUDED
