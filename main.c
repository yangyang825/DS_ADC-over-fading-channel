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
Complex H[OFDM_N];//信道估计-频域上的H
//Complex *h1;//真实fading-时域上的h 
//Complex *h2;
//h1 = (Complex*)malloc(sizeof(Complex) * (OFDM_N+GI));
//h2 = (Complex*)malloc(sizeof(Complex) * (OFDM_N+GI)); 
Complex h1[OFDM_N+GI]; 
Complex h2[OFDM_N+GI]; 
Complex estimated_signal[OFDM_N];

double SSE = 0;
double MSE = 0;
double BER = 0;

int main()
{
	srand((unsigned)time(NULL));//rand函数必备，生成随机数
	
	for(Eb_N0=10; Eb_N0<13; Eb_N0++){
		CNR = (double)Eb_N0 + 3.0;
    	for(int hloop=0; hloop<HLOOP; hloop++)
		{
//			system("pause");
    		generateTrueFading(h1, h2);
    		estimateH(H,h1,h2);
//    		system("pause");
			for(int loop=0; loop<LOOPN; loop++)
			{
				transmitter(transmitted_bit, modulated_signal);
				oversampling_GI(modulated_signal, transmitted_signal);
	//			awgn(transmitted_signal,transmitted_signal);
				freSel_fading(transmitted_signal, transmitted_signal, h1, h2);//CHANNEL
				removeGI(transmitted_signal,received_signal);
				ADC(received_signal, estimated_signal);
				FFT_demodulation(estimated_signal, H, received_bit);
				ber(loop, transmitted_bit, received_bit);
//				system("pause");
			 }
//			 BER += ber_i/1000;
		
			MSE = SSE / (OFDM_N* LOOPN);
			//printf("MSE %10.8f\n", MSE);	
		}
		
	}
    return 0;
}

