  #include "const.h"
  /*
  * Checking- do IFFT over convolutionH to see if it is right
  */
  void checkH(Complex(*H)){
  	int lengthOfH = sizeof(H)/sizeof(H[0]);
  	if(lengthOfH == 64)
	  {
  			printf("now adding 000000...");
//  			for()
	  }
	FILE* HFILE=NULL;
	for(int i=0;i<OFDM_N;i++){
		HFILE=fopen("C:\\C-SIMULATIONRESULT\\Hm.csv", "a");
    	fprintf(HFILE, "%lf , %lf\n", H[i].real, H[i].image);
    	fclose(HFILE);
	}
	printf("will do IFFT OVER H");
	system("pause");
	Complex recovered_h[OFDM_N];
	IFFT(H, recovered_h);
	for(int i=0;i<OFDM_N;i++){
		printf("recovered_h[%d]=%lf+%lf\n", i, recovered_h[i].real, recovered_h[i].image);
	}
	system("pause");
  }
