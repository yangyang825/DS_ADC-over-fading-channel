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
*	8. �����H[i]�Ĺ���, ���ں��������OFDMѶ�Ź����ŵ� Z[m]=R[m]*H^(*)[m] 
*/
void estimateH(Complex(*H)){

	Complex *pilots, *IFFTPilots;
	pilots = (Complex*)malloc(sizeof(Complex)*subcar_N);
	IFFTPilots = (Complex*)malloc(sizeof(Complex)*OFDM_N);
	Complex *IFFTPilotsAndGI;
	IFFTPilotsAndGI = (Complex*)malloc(sizeof(Complex)*(OFDM_N+GI));
	Complex *receivedIFFTPilots;
	receivedIFFTPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	Complex *estimatedIFFTPilots;
	estimatedIFFTPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	Complex *estimatedPilots;
	estimatedPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	
	generatePilots(pilots);
	IFFT(pilots, IFFTPilots);
	for(int i=0; i<OFDM_N+GI; i++){
//		printf("%d, %lf+%lf\n", i, IFFTPilots[i].real, IFFTPilots[i].image);
	}
	addGI(IFFTPilots, IFFTPilotsAndGI);
	freSel_fading(IFFTPilotsAndGI, IFFTPilotsAndGI);
	removeGI(IFFTPilotsAndGI, receivedIFFTPilots);
	ADC(receivedIFFTPilots, estimatedIFFTPilots);//ADCֻ��ʵ��������, �������estimatedFFTPilots��double []���� 
	FFT(estimatedIFFTPilots, estimatedPilots);
	generateH(estimatedPilots, pilots, H);

	free(pilots);
	free(IFFTPilots);
	free(IFFTPilotsAndGI);
	free(receivedIFFTPilots);
	free(estimatedIFFTPilots);
	free(estimatedPilots);
}

void generatePilots(Complex(*pilots))
{
	for (int i = 0; i < subcar_N; i++)
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
	for(int i=0; i<subcar_N; i++){
		H[i] = ComplexDivision(estimatedPilots[i], pilots[i]);
//		printf("%d, %lf+%lfj\n",i, H[i].real, H[i].image);
	}
}