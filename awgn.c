#include "const.h"
#include "complex.h"

extern double CNR;
void noise_generator(Complex(*noise));
double uniform01();

void awgn(Complex(*input_signal), Complex(*output_signal))
{
  Complex noise[OFDM_N + GI];
  for (int i = 0; i < OFDM_N + GI; i++)
  {
    noise[i].real = 0;
    noise[i].image = 0;
  }
  noise_generator(noise);

  for (int i = 0; i < OFDM_N + GI; i++)
  {
    output_signal[i] = ComplexAdd(input_signal[i], noise[i]);
  }
}

void noise_generator(Complex(*noise))
{
  int n;
  double u1, u2, num;
  //	printf("awgn-CNR=%lf\n", CNR);
  num = pow(10.0, (-CNR / 10));
  for (n = 0; n < OFDM_N + GI; n++)
  {

    u1 = uniform01();
    u2 = uniform01();

    noise[n].real = sqrt(-num * log(u1)) * cos(2.0 * PI * u2);
    noise[n].image = sqrt(-num * log(u1)) * sin(2.0 * PI * u2);
  }
}

double uniform01()
{
  double u;
  u = (double)rand() / RAND_MAX;
  while (u == 0.0)
  {
    u = (double)rand() / RAND_MAX;
  }
  return u;
}
