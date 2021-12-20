#include "const.h"
#include "complex.h"

extern double CNR;

void noise_generator(Complex(*noise));
double uniform01();

void awgn(Complex(*input_signal), Complex(*output_signal))
{
    Complex noise[OFDM_N];
    noise_generator(noise);

    for (int i=0; i<OFDM_N; i++){
    	//printf("i= %d, ����awgnǰ���ź�%lf + %lf j\n", i, input_signal[i].real, input_signal[i].image);
        output_signal[i] = ComplexAdd(input_signal[i], noise[i]);
        //printf("i= %d, ����awgn֮����ź�%lf + %lf j\n", i, output_signal[i].real, output_signal[i].image);
    }
}

void noise_generator(Complex(*noise))
{
	int n;
	double u1, u2, num;

	num = pow(10.0, (-CNR/10));

	for(n=0; n<OFDM_N; n++){

		u1 = uniform01();
		u2 = uniform01();

		noise[n].real = sqrt(-num*log(u1)) * cos(2.0*PI*u2);
		noise[n].image = sqrt(-num*log(u1)) * sin(2.0*PI*u2);
	}
}

double uniform01()
{
	double u;
	u = (double)rand()/RAND_MAX;
	while(u == 0.0 ){    //rand()��ȡֵ��Χ��[0 RAND_MAX]  u1Ϊ0ʱlog������������Ե���ȷ��Ҫ��ߵ�ʱ������������
		u = (double)rand()/RAND_MAX;
//		������ȷ�Ⱦ͹��ˣ���������Ϊ0�ĸ���ʵ��̫С��ǿ��֢����д�ɵݹ�u=uniform01()
	}
	return u;
}
