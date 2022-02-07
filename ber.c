
#include "const.h"

extern double CNR;

void ber(int loop, int(*tbit), int(*rbit), double(*averageBERloop))
{
  int n, error = 0;
  double Pe;
  for (n = 0; n < BITN; n++)
  {
    if (tbit[n] != rbit[n])
    {
      error++;
      //            printf("error happens at %d\n",n);
    }
  }
  Pe = (double)error / (double)BITN;
  *averageBERloop += (double)Pe / LOOPN;
  if (loop % 10 == 0)
  {
    printf("loop=%d: Eb/N0 = %f, Pe=%e\t averageBERloop= %e\n", loop, (CNR - 3.0), Pe, *averageBERloop);
  }

  if (loop == LOOPN - 1)
  {
    //    printf("ber.c loop=%d PATH=%d BER: Eb/N0 = %f, averageBERloop = %e\n",loop, D, (CNR - 3.0), *averageBERloop);
    //		FILE* berFILE=NULL;
    //		berFILE=fopen("C:\\C-SIMULATIONRESULT\\ber.csv", "a");
    //    fprintf(berFILE, "PATH=%d , %d , %lf , %e\n",D, loop, (CNR - 3.0), *averageBERloop);
    //    fclose(berFILE);
  }
}
