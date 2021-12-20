#include "const.h"
#include "complex.h"

void removeGI(Complex (*GI_signal), Complex(*output_signal))
{
	int j;
    for (j = 0; j < OFDM_N; j++)
    {
		output_signal[j].real = GI_signal[j+GI].real;
		output_signal[j].image = GI_signal[j+GI].image;
    }

}
