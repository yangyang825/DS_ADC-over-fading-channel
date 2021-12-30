#include "const.h"

void getConvolution(Complex(h1), Complex(h2), Complex(*H))
{
  /*h1, h2 ��[GI,OFDM_N+GI)����������*/
  Complex current[OFDM_N], hm[OFDM_N], convolutionH[OFDM_N];
  // initialization
  for (int i = 0; i < OFDM_N; i++)
  {
    hm[i].real = 0;
    hm[i].image = 0;
    current[i].real = 0;
    current[i].image = 0;
    convolutionH[i].real = 0;
    convolutionH[i].image = 0;
  }

  for (int m = 0; m < OFDM_N; m++)
  {
    hm[0] = h1; //ȥ��GI hm.real .............��ͼ
    hm[D] = h2; //
    for (int d = 0; d < OFDM_N; d++)
    {
      current[m].real = ComplexMulti(hm[d], Exp(-2.0 * PI * m * d / OFDM_N)).real;
      current[m].image = ComplexMulti(hm[d], Exp(-2.0 * PI * m * d / OFDM_N)).image;

      convolutionH[m].real = convolutionH[m].real + current[m].real;
      convolutionH[m].image = convolutionH[m].image + current[m].image;
    }
  }
  for (int i = 0; i < POINT_N; i++)
  {
    H[i] = convolutionH[612 + i];
  }
}
