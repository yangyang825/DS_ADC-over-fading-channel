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
	/*
	*Initialization
	*/
	h1.real = 1;
	h1.image = 0;
	h2.real = -1;
	h2.image = 0;
	for (Eb_N0 = 8; Eb_N0 < 30; Eb_N0++) {

		Complex *h1ptr = &h1;
		Complex *h2ptr = &h2;
		generateTrueFading(h1ptr, h2ptr);

		CNR = (double)Eb_N0 + 3.0;
		for (int hloop = 0; hloop < HLOOP; hloop++) {
			// system("pause");
			// getConvolution(h1, h2, H); // do FFT get real H for equalization
			estimateH(h1, h2, H);      // estimate H

			for (int loop = 0; loop < LOOPN; loop++) {
				// system("pause");
				// printf("here is loop++\n");
				transmitter(transmitted_bit, QPSK_signal);
				oversampling_GI(QPSK_signal, transmitted_signal);
				awgn(transmitted_signal,transmitted_signal);
				freSel_fading(transmitted_signal, received_signal, h1, h2); // CHANNEL
				ADC(received_signal, estimated_signalAndGI);
				removeGI(estimated_signalAndGI, estimated_signal);
				FFT_demodulation(estimated_signal, H, received_QPSK_signal);
				QPSK_demodulator(received_QPSK_signal, received_bit);

				ber(loop, transmitted_bit, received_bit, &ber_i);
			}
			AverageBER += ber_i / HLOOP;
			ber_i = 0.0;
			printf("HHHhloop=%dʱ BER: Eb/N0 = %lf, AverageBER = %e\n",hloop, (CNR - 3.0), AverageBER);
			FILE* HFILE=NULL;
			HFILE=fopen("C:\\C-SIMULATIONRESULT\\Hm.csv", "a");
	    	fprintf(HFILE, "HHHhloop=%d, %lf , %e\n",hloop, (CNR - 3.0), AverageBER);
	    	fclose(HFILE);
			// MSE = SSE / (OFDM_N* LOOPN);
			// printf("MSE %10.8f\n", MSE);
		}
	}
	return 0;
}
