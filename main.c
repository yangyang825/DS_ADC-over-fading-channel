#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "const.h"
#include "complex.h"

double CNR;
int Eb_N0;
//	loop: 重复次数； Eb_N0：1bit含量的信号的能量 与 噪音密度的比值
int transmitted_bit[BITN], received_bit[BITN];

Complex modulated_signal[POINT_N], transmitted_signal[OFDM_N + GI];
Complex received_signal[OFDM_N];
Complex H[OFDM_N];

Complex estimated_signal[OFDM_N];

double SSE = 0;
double MSE = 0;

int main()
{
	srand((unsigned)time(NULL));//rand函数必备，生成随机数
	
	for(Eb_N0=4; Eb_N0<13; Eb_N0++)
	{
		CNR = (double)Eb_N0 + 3.0;
		/* 每个OFDM SYMBOL之前都要传一个packet来估计信道 */	
		estimateH(H); 
//		for(int i=0;i<OFDM_N;i++){
//			printf("%d, H = %lf+%lfj\n",i, H[i].real, H[i].image);
//		}
		for(int loop=0; loop<LOOPN; loop++)
		{
			transmitter(transmitted_bit, modulated_signal);
			oversampling_GI(modulated_signal, transmitted_signal);
			awgn(transmitted_signal,transmitted_signal);
			freSel_fading(transmitted_signal, transmitted_signal);
			removeGI(transmitted_signal,received_signal);
			ADC(received_signal, estimated_signal);
//			for (int i = 0; i < OFDM_N; i++)
//			{
//				printf("i= %d, ADC前的信号的实部%lf, ADC后信号%lf \n", i, received_signal[i].real, estimated_signal[i]);
//				SSE += pow((estimated_signal[i] - received_signal[i].real), 2);
//			}
			FFT_demodulation(estimated_signal, H, received_bit);
			ber(loop, transmitted_bit, received_bit);
		 }		
		MSE = SSE / (OFDM_N* LOOPN);
		//printf("MSE %10.8f\n", MSE);
	}
    return 0;
}

