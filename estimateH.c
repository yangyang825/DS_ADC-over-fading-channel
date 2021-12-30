#include <stdio.h>
#include "const.h"
Complex generateOnePilot();
void generatePilots(Complex(*pilots));
void generateH(Complex(*estimatedPilots), Complex(*pilots), Complex(*H));
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
void estimateH(Complex(*H), Complex(h1), Complex(h2))
{

  Complex QPSKPilots[POINT_N], IFFTPilots[OFDM_N], IFFTPilotsAndGI[OFDM_N + GI];                               // transmitter used variables
  Complex receivedIFFTPilotsAndGI[OFDM_N + GI], ADCOutputIFFTPilotsAndGI[OFDM_N + GI], IFFTPilotsNoGI[OFDM_N]; // receiver used variables
  Complex *FFTPilots;                                                                                          // demodulator used variables
  Complex *receivedQPSKPilots;

  generatePilots(QPSKPilots);
  oversampling_GI(QPSKPilots, IFFTPilotsAndGI);
  freSel_fading(IFFTPilotsAndGI, receivedIFFTPilotsAndGI, h1, h2);
  // awgn(IFFTPilotsAndGI, IFFTPilotsAndGI);
  // ADC(IFFTPilotsAndGI, estimatedIFFTPilotsAndGI);
  removeGI(receivedIFFTPilotsAndGI, IFFTPilotsNoGI);
  FFT(IFFTPilotsNoGI, FFTPilots); 
  filter(FFTPilots, receivedQPSKPilots);
  generateH(receivedQPSKPilots, QPSKPilots, H);

  //	for(int i=0; i<POINT_N; i++){
  //		printf("%d\tpilots before fading: QPSKPilots=%lf+%lf\t\n", i, QPSKPilots[i].real, QPSKPilots[i].image);
  ////		printf("%d\tpilots after fading: IFFTPilotsAndGI=%lf+%lf\t\n", i, IFFTPilotsAndGI[612+i].real, IFFTPilotsAndGI[612+i].image);
  //		printf("%d\tpilots when estimate H: estimatedQPSKPilots=%lf + %lf\n", i, estimatedQPSKPilots[i].real, estimatedQPSKPilots[i].image);
  //	}
}

void generatePilots(Complex(*pilots))
{
  for (int i = 0; i < POINT_N; i++)
  {
    pilots[i] = generateOnePilot();
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
