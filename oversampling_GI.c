#include "const.h"
#include "complex.h"

extern double VREF;
void oversampling_GI(Complex(*modulated_signal), Complex(*transmitted_signal))
{
  Complex *OFDM_signal;
  OFDM_signal = (Complex *)malloc(sizeof(Complex) * OFDM_N);
  for (int i = 0; i < OFDM_N; i++)
  {
    OFDM_signal[i].real = 0;
    OFDM_signal[i].image = 0;
  }
  overSampling(modulated_signal, OFDM_signal);
  // Amp--Obtain Vref
  double max = 0;
  for (int k = 0; k < OFDM_N; k++)
  {
    if (k == 0)
    {
      max = fabs(OFDM_signal[k].real);
    }
    else
    {
      if (max < fabs(OFDM_signal[k].real))
      {
        max = fabs(OFDM_signal[k].real);
      }
    }
  }
  VREF = 2 * max;
  addGI(OFDM_signal, transmitted_signal);
}
//	overSampling
void overSampling(Complex(*modulated_signal), Complex(*OFDM_signal))
{
  // mapping
  Complex *FFT_signal;
  FFT_signal = (Complex *)malloc(sizeof(Complex) * subcar_N);
  // Complex FFT_signal[subcar_N];
  for (int i = 0; i < subcar_N; i++)
  {
    FFT_signal[i].real = 0;
    FFT_signal[i].image = 0;
  }

  Complex *conjugate_signal;
  conjugate_signal = (Complex *)malloc(sizeof(Complex) * POINT_N);
  for (int j = 0; j < POINT_N; j++) // obtain conjugate(s)
  {
    conjugate_signal[j].real = modulated_signal[j].real;
    conjugate_signal[j].image = -modulated_signal[j].image;
  }
  for (int k = 0; k < POINT_N; k++) // obtain FFT signals
  {
    FFT_signal[k + deltaFrequency + (subcar_N / 2)] = modulated_signal[k]; //[~512~] 512+0+100
    FFT_signal[-k - deltaFrequency + (subcar_N / 2)] = conjugate_signal[k];
  }
  /* IFFT */
  IFFT(FFT_signal, OFDM_signal);
}
void IFFT(Complex(*FFT_signal), Complex(*OFDM_signal))
{
  Complex *temp_signal;
  temp_signal = (Complex *)malloc(sizeof(Complex) * subcar_N);
  for (int i = 0; i < subcar_N; i++)
  {
    temp_signal[i].real = 0;
    temp_signal[i].image = 0;
    OFDM_signal[i].real = 0;
    OFDM_signal[i].image = 0;
  }
  for (int k = 0; k < OFDM_N; k++)
  {
    for (int n = 0; n < subcar_N; n++) // 1024=subcar_N=OFDM_N
    {
      temp_signal[k].real = 1 / sqrt(OFDM_N) * ComplexMulti(FFT_signal[n], Exp(2 * PI * k * n / OFDM_N)).real;
      temp_signal[k].image = 1 / sqrt(OFDM_N) * ComplexMulti(FFT_signal[n], Exp(2 * PI * k * n / OFDM_N)).image;

      OFDM_signal[k].real = OFDM_signal[k].real + temp_signal[k].real;
      OFDM_signal[k].image = OFDM_signal[k].image + temp_signal[k].image;
    }
    //    	FILE* file = NULL;
    //		printf("���� %d ��ʱ��� s[%d]��Ϊ %e\n", OFDM_N, k, OFDM_signal[k].real );
    //       	file=fopen("C:\\C-SIMULATIONRESULT\\tesingPower1024ofADC.csv", "a");
    //        fprintf(file, "���� %d ��ʱ��� , s[%d]Ϊ , %e\n", OFDM_N, k, OFDM_signal[k].real );
    //    	fclose(file);
  }
}
void addGI(Complex(*OFDM_signal), Complex(*transmitted_signal))
{
  int n;
  for (n = 0; n < GI; n++)
  {
    transmitted_signal[n] = OFDM_signal[n + GI * 3];
  }
  for (n = GI; n < OFDM_N + GI; n++)
  {
    transmitted_signal[n] = OFDM_signal[n - GI];
  }
}
