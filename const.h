#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "complex.h"

#ifndef const_h
#define const_h
#endif /* const_h */

/*---------------------------------------------------------*/
/* Parameters                                              */
/*---------------------------------------------------------*/

#define LOOPN		10000
/*----------����----------*/
#define sqr(x)				((x)*(x))
#define DIM			100

/*----------transmitter----------*/
#define POINT_N		(64) //�ź�����
#define BITN		(POINT_N * 2)		//QPSK���ƣ��źŶ�Ӧ��bit����Ϊ�ź���������
#define PI			3.141592654		//acos(-1.0)
#define OneBySqrt2	0.7071067812	//1.0/sqrt(2.0)
/*----------����OFDM�ź�----------*/
#define	GI			0//(OFDM_N/4)				/* Guard interval */
#define subcar_N	1024				//FFT�źŸ���
#define OFDM_N     	(subcar_N)//1024          //OFDM�źŸ���
#define trans_N		(OFDM_N+GI)//1024+1024/4
#define pilot_real   OneBySqrt2//1//����Ϊ1 
#define pilot_image  OneBySqrt2//0//
/*----------freSel_fading----------*/
#define ORIGINALTHETA         0
#define Fd                    0
#define D                     0
/*................��ͳһ���ű�ʾ...................*/
#define SYMBOL_RATE			42.0e3
#define Ts					(1.0/SYMBOL_RATE)
#define fs2					1024000/Ts
#define fs1					Ts/OFDM_N



#define Vref  (1.0);	//|Vref|>Math.max(received_signal[i].real)
/*----------statistic/ber----------*/
#define FILENAME			".\\data\\awgn.dat"
 


/*---------------------------------------------------------*/
/* Subfunctions                                            */
/*---------------------------------------------------------*/

/*----------Transmitter----------*/

void transmitter(int(*bit), Complex(*signal));

void oversampling_GI(Complex(*modulated_signal), Complex(*transmitted_signal));
	void IFFT(Complex(*FFT_signal), Complex(*OFDM_signal));
	void overSampling(Complex(*modulated_signal), Complex(*OFDM_signal));
	void addGI(Complex (*OFDM_signal), Complex(*transmitted_signal));
void estimateH(Complex(*H));
void awgn(Complex(*input_signal), Complex(*output_signal));

void freSel_fading(Complex(*input), Complex(*output));

void removeGI(Complex(*signal), Complex(*output_signal));

void ADC(Complex(*received_signal), Complex(*estimated_signal));

void FFT_demodulation(Complex(*estimated_signal), Complex(*H), int(*received_bit));
	void FFT(Complex(*estimated_signal), Complex(*recovered_signal));
	void clearHimpact(Complex(*recovered_signal), Complex(*H));
	void demodulation(Complex(*signal), int(*received_bit));

void ber(int loop, int(*tbit), int(*rbit));
