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
void estimateH(Complex(*H),Complex(*h1),Complex(*h2)){

	Complex *QPSKPilots;
	Complex *IFFTPilotsAndGI;
	Complex *receivedIFFTPilots;
	Complex *estimatedIFFTPilotsAndGI,*FFTPilots;
	Complex *estimatedQPSKPilots;
	
	QPSKPilots = (Complex*)malloc(sizeof(Complex)*POINT_N);
	IFFTPilotsAndGI = (Complex*)malloc(sizeof(Complex)*(OFDM_N+GI));
	estimatedIFFTPilotsAndGI = (Complex*)malloc(sizeof(Complex)*(OFDM_N+GI));
	receivedIFFTPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	FFTPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	estimatedQPSKPilots = (Complex*)malloc(sizeof(Complex)*(POINT_N));
	
	generatePilots(QPSKPilots);
	oversampling_GI(QPSKPilots, IFFTPilotsAndGI);
	freSel_fading(IFFTPilotsAndGI, IFFTPilotsAndGI, h1, h2); 
//	awgn(IFFTPilotsAndGI, IFFTPilotsAndGI);
	ADC(IFFTPilotsAndGI, estimatedIFFTPilotsAndGI);
	removeGI(estimatedIFFTPilotsAndGI, receivedIFFTPilots);
	FFT(receivedIFFTPilots, FFTPilots);//����ΪOFDM_N 
	filter(FFTPilots, estimatedQPSKPilots);
	generateH(estimatedQPSKPilots, QPSKPilots, H);
	
	for (int i = 0; i < OFDM_N; i++)
	{
	  printf("%d, IFFTPilotsAndGI = %lf+%lf\n", i, IFFTPilotsAndGI[i].real, IFFTPilotsAndGI[i].image);
	  printf("%d, estimatedIFFTPilotsAndGI = %lf+%lf\n", i, estimatedIFFTPilotsAndGI[i].real, estimatedIFFTPilotsAndGI[i].image);
	}
	
//	free(QPSKPilots);
//	free(IFFTPilotsAndGI);
//	free(receivedIFFTPilots);
//	free(estimatedIFFTPilots);
//	free(estimatedQPSKPilots);
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
void generateH(Complex(*estimatedPilots), Complex(*pilots), Complex(*H))//.......
{
	for(int i=0; i<POINT_N; i++){
		H[i] = ComplexDivision(estimatedPilots[i], pilots[i]);
		
	}
}
