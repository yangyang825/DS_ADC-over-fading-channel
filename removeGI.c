#include "const.h"
#include "complex.h"

void removeGI(Complex (*GI_signal), Complex(*output_signal))
{
	int i;
    for (i = 0; i < OFDM_N; i++)
    {
		output_signal[i].real = GI_signal[i+GI].real;
		output_signal[i].image = GI_signal[i+GI].image;
//		printf("i=%d: output_signal[i]=%lf+%lf \t \n", i,GI_signal[i+GI].real, GI_signal[i+GI].image);
    }

}
