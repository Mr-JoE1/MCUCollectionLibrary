
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
#include "fft_float.h"

#if defined(USE_FFT_TWD_LUT)
#if FFT_LOG2N == 4					//16-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
};

#elif FFT_LOG2N == 5				//32-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
  0.98078528,  -0.19509032,
};

#elif FFT_LOG2N == 6				//64-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
  0.98078528,  -0.19509032,
  0.99518473,  -0.09801714,
};

#elif FFT_LOG2N == 7				//128-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
  0.98078528,  -0.19509032,
  0.99518473,  -0.09801714,
  0.99879546,  -0.04906767,
};

#elif FFT_LOG2N == 8				//256-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
  0.98078528,  -0.19509032,
  0.99518473,  -0.09801714,
  0.99879546,  -0.04906767,
  0.99969882,  -0.02454123,
};

#elif FFT_LOG2N == 9				//512-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
  0.98078528,  -0.19509032,
  0.99518473,  -0.09801714,
  0.99879546,  -0.04906767,
  0.99969882,  -0.02454123,
  0.99992470,  -0.01227154,
};

#elif FFT_LOG2N == 10				//1024-point FFT
const FFT_TYPE lut_twd[2+FFT_LOG2N*2] = {
  0.00000000,   0.00000000,
 -1.00000000,   0.00000000,
  0.00000000,  -1.00000000,
  0.70710678,  -0.70710678,
  0.92387953,  -0.38268343,
  0.98078528,  -0.19509032,
  0.99518473,  -0.09801714,
  0.99879546,  -0.04906767,
  0.99969882,  -0.02454123,
  0.99992470,  -0.01227154,
  0.99998118,  -0.00613588,
};

#else
#warning "FFT twiddle factors not defined in lut_twd[]. Comment out USE_FFT_TWD_LUT"
#endif

#else											//USE_FFT_TWD_LUT not defined. precalculate via lut_cal()
FFT_TYPE lut_twd[2+FFT_LOG2N*2];				//to be precalculated by lut_cal()
#endif



#ifndef USE_FFT_TWD_LUT
//precalculated lut_twd array
//format: cosine, sine
//0..M_PI
//calculat the look-up table
//calculate the look-up tables
//n=FFT_N but only half of the cycle (FFT_N/2) is calculated
void lut_cal(void) {
    int m;
    FFT_TYPE tmp=M_PI*2/2/2;						//we start lei at 2*2

	//manually calculate the firts two (x2) points
    //printf("%12.8f, %12.8f,\n\r", lut_twd[0+0], lut_twd[0+0+1]);	//only used to produce the fft twiddle factor look-up tables
	lut_twd[2]=-1.0, lut_twd[2+1]= 0.0;
	//printf("%12.8f, %12.8f,\n\r", lut_twd[1+1], lut_twd[1+1+1]);	//only used to produce the fft twiddle factor look-up tables
	lut_twd[4]= 0.0, lut_twd[4+1]=-1.0;
    //printf("%12.8f, %12.8f,\n\r", lut_twd[2+2], lut_twd[2+2+1]);	//only used to produce the fft twiddle factor look-up tables
    for (m=3; m<=FFT_LOG2N; m+=1) {
    	tmp = tmp / 2;							//tmp = M_PI / lei;
        //lut_cos[i]=cos(-2*M_PI/n*i);
        //lut_sin[i]=sin(-2*M_PI/n*i);
        lut_twd[m+m]=cos(/*2*M_PI/n*i*/tmp);
        lut_twd[m+m+1]=-sin(/*2*M_PI/n*i*/tmp);
        //to calculate lut
        //printf("%12.8f, %12.8f,\n\r", lut_twd[m+m], lut_twd[m+m+1]);	//only used to produce the fft twiddle factor look-up tables
    }
}
#endif

/*******************************************************************
����ԭ�ͣ�struct compx EE(struct compx b1,struct compx b2)
�������ܣ��������������г˷�����
��������������������嶨��ĸ���a,b
���������a��b�ĳ˻��������������ʽ���
*******************************************************************/
compx EE(compx a, compx b) {
    compx c;
    c.real=a.real*b.real-a.imag*b.imag;
    c.imag=a.real*b.imag+a.imag*b.real;
    return(c);
}

/*****************************************************************
����ԭ�ͣ�void FFT(struct compx *xin,int N)
�������ܣ�������ĸ�������п��ٸ���Ҷ�任��FFT��
���������*xin�����ṹ������׵�ַָ�룬struct��
*****************************************************************/
void fft_float(int16_t FFTi_LOG2N, compx *xin) {
    int f,m,nv2,nm1,i,k,l,j=0;
    compx u,w,t;
    //int FFTi_N = 1ul << FFTi_LOG2N;

    nv2=FFT_N/2;                  //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
    nm1=FFT_N-1;
    for(i=0; i<nm1; i++) {
        if(i<j) {                  //���i<j,�����б�ַ
            t=xin[j];
            xin[j]=xin[i];
            xin[i]=t;
        }
        k=nv2;                    //��j����һ����λ��
        while(k<=j) {             //���k<=j,��ʾj�����λΪ1
            j=j-k;                 //�����λ���0
            k=k/2;                 //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
        }
        j=j+k;                   //��0��Ϊ1
    }

    {
        int le,lei,ip;                            //FFT����ˣ�ʹ�õ����������FFT����
        f=FFT_N;
        for(l=1; (f=f/2)!=1; l++)                //����l��ֵ����������μ���
            ;
        for(m=1; m<=l; m++) {                     // ���Ƶ��νἶ��
            //m��ʾ��m�����Σ�lΪ���μ�����l=log��2��N
            le=2<<(m-1);                            //le���ν���룬����m�����εĵ��ν����le��
            lei=le/2;                               //ͬһ���ν��вμ����������ľ���
            u.real=1.0;                             //uΪ���ν�����ϵ������ʼֵΪ1
            u.imag=0.0;
            w.real=cos(PI/lei);                     //wΪϵ���̣�����ǰϵ����ǰһ��ϵ������
            w.imag=-sin(PI/lei);
            //printf("m=%5d, lei=%5d\n\r", m, lei);
            for(j=0; j<=lei-1; j++) {               //���Ƽ��㲻ͬ�ֵ��νᣬ������ϵ����ͬ�ĵ��ν�
                for(i=j; i<=FFT_N-1; i=i+le) {        //����ͬһ���ν����㣬������ϵ����ͬ���ν�
                    ip=i+lei;                           //i��ip�ֱ��ʾ�μӵ�������������ڵ�
                    t=EE(xin[ip],u);                    //�������㣬�����ʽ
                    xin[ip].real=xin[i].real-t.real;
                    xin[ip].imag=xin[i].imag-t.imag;
                    xin[i].real=xin[i].real+t.real;
                    xin[i].imag=xin[i].imag+t.imag;
                }
                u=EE(u,w);                           //�ı�ϵ����������һ����������
            }
        }
    }

}

//the same calculation as above but in radix 2, but uses a look-up table: lut_twd[]
/*****************************************************************
����ԭ�ͣ�void FFT(struct compx *xin,int N)
�������ܣ�������ĸ�������п��ٸ���Ҷ�任��FFT��
���������*xin�����ṹ������׵�ַָ�룬struct��
*****************************************************************/
void fft_float2(int16_t FFTi_LOG2N, compx *xin) {
    int f,m,nv2,nm1,i,k,l,j=0;
    compx u,w,t;
    //int FFTi_N = 1ul << FFTi_LOG2N;

    nv2=FFT_N/2;                  //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
    nm1=FFT_N-1;
    for(i=0; i<nm1; i++) {
        if(i<j) {                  //���i<j,�����б�ַ
            t=xin[j];
            xin[j]=xin[i];
            xin[i]=t;
        }
        k=nv2;                    //��j����һ����λ��
        while(k<=j) {             //���k<=j,��ʾj�����λΪ1
            j=j-k;                 //�����λ���0
            k=k/2;                 //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
        }
        j=j+k;                   //��0��Ϊ1
    }

    {
        int le,lei,ip;                            //FFT����ˣ�ʹ�õ����������FFT����
        f=FFT_N;
        for(l=1; (f=f/2)!=1; l++)                //����l��ֵ����������μ���
            ;
        for(m=1; m<=l; m++) {                     // ���Ƶ��νἶ��
            //m��ʾ��m�����Σ�lΪ���μ�����l=log��2��N
            le=2<<(m-1);                            //le���ν���룬����m�����εĵ��ν����le��
            lei=le/2;                               //ͬһ���ν��вμ����������ľ���
            u.real=1.0;                             //uΪ���ν�����ϵ������ʼֵΪ1
            u.imag=0.0;
            w.real=lut_twd[m+m];					//w.real=cos(PI/lei);                     //wΪϵ���̣�����ǰϵ����ǰһ��ϵ������
            w.imag=lut_twd[m+m+1];					//w.imag=-sin(PI/lei);
            //printf("m=%5d, le=%5d, lei=%5d\n\r", m, le, lei);
            for(j=0; j<=lei-1; j++) {               //���Ƽ��㲻ͬ�ֵ��νᣬ������ϵ����ͬ�ĵ��ν�
                for(i=j; i<=FFT_N-1; i=i+le) {        //����ͬһ���ν����㣬������ϵ����ͬ���ν�
                    ip=i+lei;                           //i��ip�ֱ��ʾ�μӵ�������������ڵ�
                    t=EE(xin[ip],u);                    //�������㣬�����ʽ
                    xin[ip].real=xin[i].real-t.real;
                    xin[ip].imag=xin[i].imag-t.imag;
                    xin[i].real=xin[i].real+t.real;
                    xin[i].imag=xin[i].imag+t.imag;
                }
                u=EE(u,w);                           //�ı�ϵ����������һ����������
            }
        }
    }

}

/************************************************************
����ԭ�ͣ�void main()
�������ܣ�����FFT�任����ʾ����ʹ�÷���
�����������
�����������
************************************************************/
/*
int main(void) {
    int i;

    //generate the data
    for(i=0; i<FFT_N; i++) {                       	//���ṹ�帳ֵ
        s[i].real =	1.0+ \
        			2.0*cos(2*PI*1*(3.6/360+1.0*i/FFT_N))+ \
        			3.0*cos(2*PI*4*(1.8/360+1.0*i/FFT_N))+ \
        			4.0*cos(2*PI*6*(1.8/360+1.0*i/FFT_N))+ \
        			1.0e-1*rand()/RAND_MAX; 		//ʵ��Ϊ���Ҳ�FFT_N���������ֵΪ1
        s[i].imag=0;                                //�鲿Ϊ0
    }

	//print out the time-domain data
	printf("time-domain data...\n");
	for (i=0; i<FFT_N; i++)
		printf("i=%4d, real = %10.7f, imag = %10.7f, amp= %10.7f\n", i, s[i].real, s[i].imag, sqrt(s[i].real*s[i].real + s[i].imag*s[i].imag));

	//perform fft
    fft(s);                                        	//���п��ٸ���Ҷ�任

	//printf the fft data
	printf("post-FFT data...\n");
	for (i=0; i<FFT_N/2; i++)
		printf("i=%4d, real = %10.7f, imag = %10.7f, amp= %10.2f\n", i, s[i].real/FFT_N, s[i].imag/FFT_N, sqrt(s[i].real*s[i].real + s[i].imag*s[i].imag)/((i==0)?FFT_N:(FFT_N/2)));

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
