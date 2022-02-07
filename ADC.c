#include "const.h"
#include "complex.h"
double estimate(double *realSignal);
// extern MSE, SSE;
extern double VREF;
void ADC(Complex(*received_signal), Complex(*estimated_signal))
{
  for (int i = 0; i < OFDM_N + GI; i++)
  {
    estimated_signal[i].real = estimate(&received_signal[i].real);
    //		estimated_signal[i].image = 0;//
    estimated_signal[i].image = received_signal[i].image; // remain noise over image part, otherwise the BER will be smaller than the theory
  }
}

double estimate(double *realSignal)
{
  double estimated_signal = 0;

  double delta;
  double sumOfDelta = 0;
  int bitStream[ADCSAMPLING_N]; //.....
  // int *bitStream;
  // bitStream = (int*)malloc(sizeof(int) * adcN);
  for (int j = 0; j < ADCSAMPLING_N; j++)
  {
    bitStream[j] = 0;
  }

  double DACoutput = VREF; //+ or -Vref
  for (int j = 0; j < ADCSAMPLING_N; j++)
  {
    bitStream[j] = 0;
    delta = *realSignal - DACoutput;
    sumOfDelta += delta;

    if (sumOfDelta >= 0)
    {
      bitStream[j] = 1;
      DACoutput = VREF;
    }
    else
    {
      bitStream[j] = 0;
      DACoutput = -VREF;
    }
    estimated_signal += DACoutput;
  }
  estimated_signal /= ADCSAMPLING_N;
  return estimated_signal;
}
