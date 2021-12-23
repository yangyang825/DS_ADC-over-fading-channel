#include "const.h"


void freSel_fading(Complex(*input),Complex(*output),Complex(*h1),Complex(*h2))
{
    int i;
    //i<D i[i]*h[1]
    for(i=D;i<(OFDM_N+GI);i++)
    {
//    	printf("i= %d, freSelfading前的信号%lf + %lf j\n", i, input[i].real, input[i].image);
//    	printf("i= %d, 真实的h h1=%lf, h2=%lf\n", i, H1[i].real, H2[i].real);
        output[i]=ComplexAdd(ComplexMulti(input[i], h1[i]), ComplexMulti(input[i-D], h2[i-D]));
        //printf("i= %d, add freSelfading后的信号%lf + %lf j\n", i, output[i].real, output[i].image);
    }
}

