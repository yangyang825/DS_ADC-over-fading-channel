#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "const.h"
#include "complex.h"

double CNR;
int Eb_N0;

int transmitted_bit[BITN], received_bit[BITN];
Complex QPSK_signal[POINT_N], OFDM_signal[OFDM_N], transmitted_signal[OFDM_N + GI];
Complex received_signal[OFDM_N + GI], estimated_signalAndGI[OFDM_N + GI];//variables in ADC
Complex estimated_signal[OFDM_N];
Complex received_QPSK_signal[POINT_N];
Complex H[POINT_N]; // channelEstimation
Complex h1, h2;     // real fading

double SSE = 0;
double MSE = 0;
double AverageBER = 0;
double ber_i = 0;


int main() {
	srand((unsigned)time(NULL)); // rand function needed
	for (Eb_N0 = 10; Eb_N0 < 30; Eb_N0+=5) {
//		system("pause");
		/*
		*Initialization
		*/
		h1.real = 1;
		h1.image = 0;
		h2.real = -1;
		h2.image = 0;
		for(int i=0; i<BITN; i++) {
			transmitted_bit[i]=0;
			received_bit[i]=0;
		}
		for(int i=0; i<POINT_N; i++) {
			QPSK_signal[i].real=0;
			QPSK_signal[i].image=0;
			received_QPSK_signal[i].real=0;
			received_QPSK_signal[i].image=0;
			H[i].real = 0;
			H[i].image = 0;
		}
		for(int i=0; i<OFDM_N; i++) {
			OFDM_signal[i].real=0;
			OFDM_signal[i].image=0;
			estimated_signal[i].real=0;
			estimated_signal[i].image=0;
		}
		for(int i=0; i<OFDM_N+GI; i++) {
			transmitted_signal[i].real=0;
			transmitted_signal[i].image=0;
			received_signal[i].real=0;
			received_signal[i].image=0;
			estimated_signalAndGI[i].real=0;
			estimated_signalAndGI[i].image=0;
		}
		Complex *h1ptr = &h1;
		Complex *h2ptr = &h2;
		generateTrueFading(h1ptr, h2ptr);

		CNR = (double)Eb_N0 + 3.0;
		AverageBER = 0;
		for (int hloop = 0; hloop < HLOOP; hloop++) {
			// system("pause");
			// getConvolution(h1, h2, H); // do FFT get real H for equalization
			estimateH(h1, h2, H);      // estimate H
			ber_i = 0;
			SSE = 0;
			for (int loop = 0; loop < LOOPN; loop++) {
				// system("pause");
				// printf("here is loop++\n");
				transmitter(transmitted_bit, QPSK_signal);
				oversampling_GI(QPSK_signal, transmitted_signal);
				awgn(transmitted_signal,transmitted_signal);
				freSel_fading(transmitted_signal, received_signal, h1, h2); // CHANNEL
				ADC(transmitted_signal, estimated_signalAndGI);
				for(int i=GI;i<OFDM_N+GI;i++){
					SSE += sqr(received_signal[i].real-estimated_signalAndGI[i].real);
				}
				//debug
//				printf("loop=%d\tSSE=%e\n",loop, SSE);
				removeGI(estimated_signalAndGI, estimated_signal);
				FFT_demodulation(estimated_signal, H, received_QPSK_signal);
				QPSK_demodulator(received_QPSK_signal, received_bit);
				ber(loop, transmitted_bit, received_bit, &ber_i);
//				system("pause");
			}
			MSE = SSE / (LOOPN*(OFDM_N));
			FILE* MSE_ADCFILE=NULL;
			MSE_ADCFILE=fopen("C:\\C-SIMULATIONRESULT\\MSE_ADC.csv", "a");
			printf("%d,%d,%d,%e\n", LOOPN, ADCSAMPLING_N, ADCSAMPLING_N*OFDM_N, MSE);
			fprintf(MSE_ADCFILE, "%d,%d,%d,%e\n", LOOPN, ADCSAMPLING_N, ADCSAMPLING_N*OFDM_N, MSE);
    		fclose(MSE_ADCFILE);
			
			
			AverageBER += ber_i / HLOOP;
			ber_i = 0.0;
			printf("HHHhloop=%dʱ BER: Eb/N0 = %lf, AverageBER = %e\n",hloop, (CNR - 3.0), AverageBER);
			FILE* HFILE=NULL;
			HFILE=fopen("C:\\C-SIMULATIONRESULT\\Hm.csv", "a");
			fprintf(HFILE, "HHHhloop=%d, %lf , %e\n",hloop, (CNR - 3.0), AverageBER);
			fclose(HFILE);

		}
	}
	return 0;
}
