#include "const.h"

void QPSK_demodulator(Complex(*signal), int(*received_bit))
{
    int n;
    for (n = 0; n < POINT_N; n++)
    {
        if (signal[n].real > 0 && signal[n].image > 0)
        {
            received_bit[2 * n] = 0;
            received_bit[2 * n + 1] = 0;
        }
        else if (signal[n].real < 0 && signal[n].image > 0)
        {
            received_bit[2 * n] = 0;
            received_bit[2 * n + 1] = 1;

        }
        if (signal[n].real < 0 && signal[n].image < 0)
        {
            received_bit[2 * n] = 1;
            received_bit[2 * n + 1] = 1;
        }
        if (signal[n].real >= 0 && signal[n].image <= 0)
        {
            received_bit[2 * n] = 1;
            received_bit[2 * n + 1] = 0;
        }
    }
}
