#include "const.h"
void clearHimpact(Complex(*recovered_signal), Complex(*estimatedH));
void FFT_demodulation(Complex(*estimated_signal), Complex(*H), int(*received_bit)) {
    Complex* recovered_signal;
    recovered_signal = (Complex*)malloc(sizeof(Complex) * subcar_N);
    for (int i = 0; i < subcar_N; i++) {
        recovered_signal[i].real = 0;
        recovered_signal[i].image = 0;
    }
    Complex* QPSK_signal;
    QPSK_signal = (Complex*)malloc(sizeof(Complex) * POINT_N);
    //1. 做FFT转为频域信号
	FFT(estimated_signal,recovered_signal);	
	//2. 清除H影响
    clearHimpact(recovered_signal, H);
	//3. 从recovered_signal(1024个FFT频域), 择出64个QPSK讯号 
    /* recovered_signal: [0,349)=0, [349, 412]=调制信号, (412-512-612)=0, [612,675]=调制信号, (675,1023]=0 */
    for (int i = 0; i < POINT_N; i++) {
        QPSK_signal[i].real = recovered_signal[612+i].real;
        QPSK_signal[i].image = recovered_signal[612+i].image;
    }
	//4. 反QPSK调制 
    demodulation(QPSK_signal, received_bit);

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
//        printf("%d, FFT INPUT = %lf+%lf\n", n, estimated_signal[n].real, estimated_signal[n].image);
//		printf("%d, FFT OUTPUT = %lf+%lf\n", n, recovered_signal[n].real, recovered_signal[n].image); // 1...........2.CHANNEL
    }
}

void clearHimpact(Complex(*recovered_signal), Complex(*H))
{
	for(int i=1; i<OFDM_N; i++){
		recovered_signal[i] = ComplexMulti(recovered_signal[i], ComplexConjugate(H[i]));
	}
}

void demodulation(Complex(*signal), int(*received_bit))
{
    //recovered_signal: [0,349)=0, [349, 412]=调制信号, (412-512-612)=0, [612,675]=调制信号, (675,1023]=0
    int n;
    for (n = 0; n < POINT_N; n++)
    {
        if (signal[n].real > 0 && signal[n].image > 0)
        {
            received_bit[2 * n] = 0;
            received_bit[2 * n + 1] = 0;
        }
        else if (signal[n].real < 0 && signal[n].image > 0)
        {
            received_bit[2 * n] = 0;
            received_bit[2 * n + 1] = 1;

        }
        if (signal[n].real < 0 && signal[n].image < 0)
        {
            received_bit[2 * n] = 1;
            received_bit[2 * n + 1] = 1;
        }
        if (signal[n].real > 0 && signal[n].image < 0)
        {
            received_bit[2 * n] = 1;
            received_bit[2 * n + 1] = 0;
        }
    }
}
