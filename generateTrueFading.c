#include "const.h"

void generateTrueFading(Complex(*h1),Complex(*h2)){
	Doppler(h1);
    Doppler(h2);
    setPopagation(h1);
    setPopagation(h2);//Hȡģ 
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

void setPopagation(Complex(*h)) {
	int j;
	for (j=0; j<OFDM_N+GI; j++)
    {
        h[j].real = Modules(h[j]);
        h[j].image = 0;
    }
}


	

