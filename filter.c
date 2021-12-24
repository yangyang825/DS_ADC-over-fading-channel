#include "const.h"

void filter(Complex(*recovered_signal), Complex(*QPSK_signal)) {
	for (int i = 0; i < POINT_N; i++) {
        QPSK_signal[i].real = recovered_signal[612+i].real;
        QPSK_signal[i].image = recovered_signal[612+i].image;
    }
}
