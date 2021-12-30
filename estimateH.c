#include <stdio.h>
#include "const.h"

/*
 *params: H---ͨ������pilot���,���ں���OFDM�źŵ�H
 *	1. Ƶ��������1024��pilot
 *	2. pilots��IFFT��Ϊʱ��Ѷ��
 *	3. ����GI
 *   4. ʱ��Ѷ���ϼ�fading��awgn����
 *   5. ȥ��GI
 *   6. ��FFTתΪƵ��Ѷ��
 *	7. received_pilot[i]/pilot[i] = H[i]
 *	8. �����H[i], ���ں��������OFDMѶ�Ź����ŵ� Z[m]=R[m]*H^(*)[m]
 */
void estimateH(Complex(h1), Complex(h2), Complex(*H))
{

  Complex QPSKPilots[POINT_N], IFFTPilots[OFDM_N], IFFTPilotsAndGI[OFDM_N + GI];                               // transmitter used variables
  Complex receivedIFFTPilotsAndGI[OFDM_N + GI], ADCOutputIFFTPilotsAndGI[OFDM_N + GI], IFFTPilotsNoGI[OFDM_N]; // receiver used variables
  Complex FFTPilots[OFDM_N];                                                                                          // demodulator used variables
  Complex receivedQPSKPilots[POINT_N];

  generatePilots(QPSKPilots);
  oversampling_GI(QPSKPilots, IFFTPilotsAndGI);
  freSel_fading(IFFTPilotsAndGI, receivedIFFTPilotsAndGI, h1, h2);
  awgn(receivedIFFTPilotsAndGI, receivedIFFTPilotsAndGI);
  ADC(receivedIFFTPilotsAndGI, ADCOutputIFFTPilotsAndGI);
  removeGI(ADCOutputIFFTPilotsAndGI, IFFTPilotsNoGI);
  FFT(IFFTPilotsNoGI, FFTPilots);
  filter(FFTPilots, receivedQPSKPilots);
  generateH(receivedQPSKPilots, QPSKPilots, H);


}

void generatePilots(Complex(*pilots))
{
  for (int i = 0; i < POINT_N; i++)
  {
    pilots[i] = generateOnePilot();
//    printf
  }
}
Complex generateOnePilot()
{
  int rand1, rand2, symbol;
  int bin2sym[2][2] = {
      {0, 1},
      {3, 2}};
  double sym2sgnl[4][2] = {
      {OneBySqrt2, OneBySqrt2},
      {-OneBySqrt2, OneBySqrt2},
      {-OneBySqrt2, -OneBySqrt2},
      {OneBySqrt2, -OneBySqrt2}};
  rand1 = rand() & 0x1;
  rand2 = rand() & 0x1;
  symbol = bin2sym[rand1][rand2];
  Complex pilot;
  pilot.real = sym2sgnl[symbol][0];
  pilot.image = sym2sgnl[symbol][1];

  return pilot;
}
void generateH(Complex(*estimatedPilots), Complex(*pilots), Complex(*H))
{

  for (int i = 0; i < POINT_N; i++)
  {
    H[i] = ComplexDivision(estimatedPilots[i], pilots[i]);
  }
}
