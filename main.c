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
Complex received_signal[OFDM_N + GI], estimated_signalAndGI[OFDM_N + GI];
Complex estimated_signal[OFDM_N];
Complex H[POINT_N];//信道估计-频域上的H
Complex h1[OFDM_N+GI]; //真实fading-时域上的h 
Complex h2[OFDM_N+GI]; 


double SSE = 0;
double MSE = 0;
double AverageBER = 0;
double ber_i = 0;

int index = 0;//delete
int main()
{
	srand((unsigned)time(NULL));//rand函数必备，生成随机数
	
	for(Eb_N0=20; Eb_N0<30; Eb_N0++){
		CNR = (double)Eb_N0 + 3.0;
    	for(int hloop=0; hloop<HLOOP; hloop++)
		{
			system("pause");
//    		generateTrueFading(h1, h2);
			for(int i = 0;i<OFDM_N+GI;i++){
				h1[i].real = 1;
				h1[i].image = 0;
				h2[i].real = 0.5;
				h2[i].image = 0;
			} 
    		estimateH(H,h1,h2);
    		for(int i=0;i<POINT_N;i++){
    			printf("H = %lf+%lf\n", H[i].real, H[i].image);
			} 
    		
//    		getConvolution(h1,h2,H);//理论H 

			for(int loop=0; loop<LOOPN; loop++)
			{
				transmitter(transmitted_bit, modulated_signal);
				oversampling_GI(modulated_signal, transmitted_signal);
//				awgn(transmitted_signal,transmitted_signal);
				freSel_fading(transmitted_signal, received_signal, h1, h2);//CHANNEL
				ADC(received_signal, estimated_signalAndGI);
				removeGI(estimated_signalAndGI, estimated_signal);
				
//				for(int i=0;i<POINT_N;i++){
//					printf("%d, modulated_signal = %lf+%lf\n", i, modulated_signal[i].real, modulated_signal[i].image);
//					printf("%d, estimated_signal = %lf+%lf\n", i, estimated_signal[i].real, estimated_signal[i].image);
//				} 
				FFT_demodulation(estimated_signal, H, received_bit);
				
				ber(loop, transmitted_bit, received_bit, &ber_i);
				system("pause");
			 }
			AverageBER += ber_i/HLOOP;
			ber_i = 0.0;
			printf("hloop=HLOOP-1时的平均BER: Eb/N0 = %f, AverageBER = %e\n", (CNR - 3.0), AverageBER);
//			MSE = SSE / (OFDM_N* LOOPN);
			//printf("MSE %10.8f\n", MSE);	
		}
		
	}
    return 0;
}

