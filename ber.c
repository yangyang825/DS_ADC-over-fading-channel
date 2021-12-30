
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
    if(loop%100 == 0) {
    	    printf("%d次loop为: Eb/N0 = %f, Pe=%e\t ber_i= %e\n", loop, (CNR - 3.0), Pe, *ber_i);
	} 

    
    if (loop == LOOPN - 1) {

        printf("完事儿加起来loop=%d的BER: Eb/N0 = %f, ber_i = %e\n",loop, (CNR - 3.0), *ber_i);
		FILE* berFILE=NULL;
		berFILE=fopen("C:\\C-SIMULATIONRESULT\\ber.csv", "a");
    	fprintf(berFILE, "%d , %lf , %e\n",loop, (CNR - 3.0), *ber_i);
    	fclose(berFILE);
    }

}
