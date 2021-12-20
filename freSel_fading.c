#include "const.h"
double PHI();
void Doppler(Complex(*h));
void setPopagation(Complex(*H));

void freSel_fading(Complex(*input),Complex(*output))
{
	Complex *H1;
	H1 = (Complex*)malloc(sizeof(Complex) * (OFDM_N+GI));
	Complex *H2;
	H2 = (Complex*)malloc(sizeof(Complex) * (OFDM_N+GI));
	
    Doppler(H1);
    Doppler(H2);
    setPopagation(H1);
    setPopagation(H2);//H取模 
    int i;
    for(i=D;i<(OFDM_N+GI);i++)
    {
    	//printf("i= %d, freSelfading前的信号%lf + %lf j\n", i, input[i].real, input[i].image);
    	//printf("i= %d, 衰落H1=%lf, H2=%lf\n", i, H1[i].real, H2[i].real);
        output[i]=ComplexAdd(ComplexMulti(input[i], H1[i]), ComplexMulti(input[i-D], H2[i-D]));
        //printf("i= %d, add freSelfading后的信号%lf + %lf j\n", i, output[i].real, output[i].image);
    }
    free(H1);
    free(H2);
}

double PHI() {
    static int seed;
    static int flag = 0;
    if (flag == 0) {
        flag = 1;
        srand(time(NULL));
        seed = rand();
    }
    seed = 2045 * seed + 1;
    seed = seed - (seed / 1048576) * 1048576;
    return (seed) * 2 * PI / 1048576.0;


}
double uni() {
    static int seed;
    static int flag = 0;
    if (flag == 0) {
        flag = 1;
        srand(time(NULL));
        seed = rand();
    }
    seed = 2045 * seed + 1;
    seed = seed - (seed / 1048576) * 1048576;
    return (seed) / 1048576.0;

}
void Doppler(Complex(*h))
{
    Complex An;
    Complex hn[OFDM_N+GI];
    double thetan, phin;
    int n, m, i, j;
    double u1, u2;

    for (m = 0; m < (OFDM_N+GI); m++)
    {
        h[m].image = 0;
        h[m].real = 0;
    }

    for (n = 0; n < 8; n++)

    {
        thetan = PHI();
        phin = PHI();
        u1 = uni();
        u2 = uni();
        An.real = sqrt(-(1 / 16.0) * log(u1)) * cos(2 * PI * u2);//...
        An.image = sqrt(-(1 / 16.0) * log(u1)) * sin(2 * PI * u2);//...
        for (i = 0; i < (OFDM_N+GI); i++)
        {
            hn[i] = ComplexMulti(An, Exp(2 * PI * Fd * cos(thetan) * i * Ts + phin));
        }
        for (j = 0; j < (OFDM_N+GI); j++)
        {
            h[j].real = h[j].real + hn[j].real;
            h[j].image = h[j].image + hn[j].image;
        }
    }

}

void setPopagation(Complex(*H)) {
	int j;
	for (j=0; j<OFDM_N; j++)
    {
        H[j].real = Modules(H[j]);
        H[j].image = 0;
    }
}
