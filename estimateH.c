#include <stdio.h>
#include "const.h"
Complex generateOnePilot();
void generatePilots(Complex(*pilots));
void generateH(Complex(*estimatedPilots), Complex(*pilots), Complex(*H));
/*
*params: H---通过这组pilot获得,用于后续OFDM信号的H 
*	1. 频域上生成1024个pilot 
*	2. pilots做IFFT变为时域讯号
*	3. 加上GI 
*   4. 时域讯号上加fading和awgn计算
*   5. 去掉GI
*   6. 做FFT转为频域讯号
*	7. received_pilot[i]/pilot[i] = H[i]
*	8. 最后获得H[i], 用于后续传输的OFDM讯号估计信道 Z[m]=R[m]*H^(*)[m] 
*/
void estimateH(Complex(*H),Complex(*h1),Complex(*h2)){

	Complex *pilots;
	pilots = (Complex*)malloc(sizeof(Complex)*POINT_N);
	Complex *IFFTPilotsAndGI;
	IFFTPilotsAndGI = (Complex*)malloc(sizeof(Complex)*(OFDM_N+GI));
	Complex *receivedIFFTPilots;
	receivedIFFTPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	Complex *estimatedIFFTPilots;
	estimatedIFFTPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	Complex *estimatedPilots;
	estimatedPilots = (Complex*)malloc(sizeof(Complex)*(OFDM_N));
	
	generatePilots(pilots);
	oversampling_GI(pilots, IFFTPilotsAndGI);
	freSel_fading(IFFTPilotsAndGI, IFFTPilotsAndGI, h1, h2); 
//	awgn(IFFTPilotsAndGI, IFFTPilotsAndGI);
	removeGI(IFFTPilotsAndGI, receivedIFFTPilots);
	ADC(receivedIFFTPilots, estimatedIFFTPilots);//ADC只对实部做操作, 所以输出estimatedFFTPilots是double []类型 
	FFT(estimatedIFFTPilots, estimatedPilots);//[]
	generateH(estimatedPilots, pilots, H);
	free(pilots);
	free(IFFTPilotsAndGI);
	free(receivedIFFTPilots);
	free(estimatedIFFTPilots);
	free(estimatedPilots);
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
	for(int i=0; i<subcar_N; i++){
		H[i] = ComplexDivision(estimatedPilots[i], pilots[i]);
	}
}
