#include "const.h"
#include "complex.h"
double estimate(double* realSignal);
//extern MSE, SSE;

void ADC(Complex(*received_signal), Complex(*estimated_signal))
{

	for (int i = 0; i < OFDM_N+GI; i++)
	{
		estimated_signal[i].real = estimate(&received_signal[i].real);
		estimated_signal[i].image = 0;
	}
}

double estimate(double *realSignal) 
{
	double estimated_signal=0;
	
	int adcN = 35; 	//取样次数.......后期写进const.h里，用频率1/fs表示

	double delta;
	double sumOfDelta=0;
	int bitStream[35];
	//int *bitStream;
	//bitStream = (int*)malloc(sizeof(int) * adcN);
	for (int j = 0; j < adcN; j++)
	{
		//初始化
		bitStream[j] = 0;
	}

	double DACoutput = Vref;	//+ or -Vref

	//对于每一个信号都取adcN次平均值
	for (int j = 0; j < adcN; j++)
	{
		//初始化
		bitStream[j] = 0;
		delta = *realSignal - DACoutput;
		sumOfDelta += delta;

		if (sumOfDelta >= 0)
		{
			bitStream[j] = 1;
			DACoutput = Vref;
		}
		else
		{
			bitStream[j] = 0;
			DACoutput = -Vref;
		}
		estimated_signal += DACoutput;
//		free(bitStream);
	}
	//求平均 
	estimated_signal /= adcN;
	//随着循环次数增加,能输出一个收敛于transmitted_signal[0]的值
	return estimated_signal;
}

