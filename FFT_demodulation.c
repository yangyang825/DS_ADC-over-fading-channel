#include "const.h"

void FFT_demodulation(Complex(*estimated_signal), Complex(*H), Complex(*modulated_signal))
{
    Complex* recovered_signal;
    recovered_signal = (Complex*)malloc(sizeof(Complex) * subcar_N);
    for (int i = 0; i < subcar_N; i++) {
        recovered_signal[i].real = 0;
        recovered_signal[i].image = 0;
    }
    /*
	*1. First do FFT to obtain signals in frequency domain 
	*/
	FFT(estimated_signal,recovered_signal);	
	
	/*
	*2. filter-select 64 QPSK modulated signals from 1024 subcarriers
	*recovered_signal: [0,349)=0, [349, 412]=调制信号, (412-512-612)=0, [612,675]=调制信号, (675,1023]=0
	*/
    for (int i = 0; i < POINT_N; i++) {
        modulated_signal[i].real = recovered_signal[Right_StartF+i].real;
        modulated_signal[i].image = recovered_signal[Right_StartF+i].image;
    }
    
    /*
	*3. Use the estimated H to clear fading impact of Channel and the ADC
	*/
    clearHimpact(modulated_signal, H);

    free(recovered_signal);
    recovered_signal = NULL;
}

void FFT(Complex(*estimated_signal), Complex(*recovered_signal))
{
    Complex * temp_signal;
    temp_signal = (Complex*)malloc(sizeof(Complex) * OFDM_N);
    for (int i = 0; i < OFDM_N; i++)
    {
        temp_signal[i].real = 0;
        temp_signal[i].image = 0;
    }

    for (int n = 0; n < OFDM_N; n++)
    {
        for (int k = 0; k < OFDM_N; k++)
        {
            temp_signal[k].real = 1 / sqrt(OFDM_N) * ComplexMulti(estimated_signal[k], Exp(-2 * PI * k * n / OFDM_N)).real;
            temp_signal[k].image = 1 / sqrt(OFDM_N) * ComplexMulti(estimated_signal[k], Exp(-2 * PI * k * n / OFDM_N)).image;

            recovered_signal[n].real = recovered_signal[n].real + temp_signal[k].real;
            recovered_signal[n].image = recovered_signal[n].image + temp_signal[k].image;
        }
//      printf("%d, FFT INPUT = %lf+%lf\n", n, estimated_signal[n].real, estimated_signal[n].image);
//		printf("%d, FFT OUTPUT = %lf+%lf\n", n, recovered_signal[n].real, recovered_signal[n].image); // 1...........2.CHANNEL
    }
}

void clearHimpact(Complex(*recovered_signal), Complex(*H))
{
	for(int i=0; i<POINT_N; i++){
//		printf("%d, clearHimpact前recovered_signal = %lf+%lf\n", i, recovered_signal[i].real, recovered_signal[i].image);
		recovered_signal[i] = ComplexMulti(recovered_signal[i], ComplexConjugate(H[i]));
//		printf("%d, clearHimpact之后recovered_signal = %lf+%lf\n", i, recovered_signal[i].real, recovered_signal[i].image);
	}
}


