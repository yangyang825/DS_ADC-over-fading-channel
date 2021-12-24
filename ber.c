
#include "const.h"

extern double CNR;


void ber(int loop, int(*tbit), int(*rbit), double(*ber_i)) {
    int n, error = 0;
    double Pe;

    for (n = 0; n < BITN; n++) {
        if (tbit[n] != rbit[n])
        {
            error++;
        }
    }
    Pe = (double)error / (double)BITN;
    *ber_i += (double)Pe / LOOPN;
    if(loop%10 == 0) {
    	    printf("%d次循环时候的结果为: Eb/N0 = %f, Pe=%e\t ber_i= %e\n", loop, (CNR - 3.0), Pe, *ber_i);
	} 

    
    if (loop == LOOPN - 1) {
        //     FILE* file;
        printf("loop=LOOPN-1的BER: Eb/N0 = %f, ber_i = %e\n", (CNR - 3.0), *ber_i);
        // printf("%f   %e\n", (CNR - 3.0), AverageBER);
        //      fopen_s(&file, "data2.txt", "a");
        //     fprintf(file, "%f\t%e\n", (CNR - 3.0), AverageBER);
        //   fclose(file);

    }

}
