#include "const.h"

void freSel_fading(Complex(*input), Complex(*output), Complex(h1), Complex(h2))
{
  int i;
  // i<D i[i]*h[1]
  for (i = D; i < (OFDM_N + GI); i++)
  {
  output[i] = ComplexAdd(ComplexMulti(input[i], h1), ComplexMulti(input[i - D], h2));//frequency selective 
//    output[i] = ComplexMulti(input[i], h1); //flat fading 
  }
}
