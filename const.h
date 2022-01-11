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

#define LOOPN 10000
/*----------����----------*/
#define sqr(x) ((x) * (x))
#define DIM 100
/*----------channel----------*/
#define HLOOP 10
/*----------transmitter----------*/
#define POINT_N 64              //(1024)// //�ź�����
#define BITN (POINT_N * 2)      // QPSK���ƣ��źŶ�Ӧ��bit����Ϊ�ź���������
#define PI 3.141592654          // acos(-1.0)
#define OneBySqrt2 0.7071067812 // 1.0/sqrt(2.0)
//set IF frequency
#define N_C		Left_StartF-POINT_N/2
#define Left_StartF		subcar_N/2-deltaFrequency
#define Right_StartF	subcar_N/2+deltaFrequency
#define deltaFrequency	100
/*----------����OFDM�ź�----------*/
#define GI (OFDM_N / 4)   /* Guard interval */
#define subcar_N 1024     // FFT�źŸ���
#define OFDM_N (subcar_N) // 1024          //OFDM�źŸ���
//#define trans_N		(OFDM_N+GI)//1024+1024/4
#define pilot_real OneBySqrt2  // 1//����Ϊ1
#define pilot_image OneBySqrt2 // 0//
/*----------freSel_fading----------*/
#define ORIGINALTHETA 0
#define Fd 0
#define D 2 //.................
/*................��ͳһ���ű�ʾ...................*/
#define SYMBOL_RATE 42.0e3
#define Ts (1.0 / SYMBOL_RATE)
#define fs2 1024000 / Ts
#define fs1 Ts / OFDM_N

#define Vref (1.0); //|Vref|>Math.max(received_signal[i].real)
/*----------statistic/ber----------*/
#define FILENAME ".\\data\\awgn.dat"

#define REALHFILE "realH.csv"
/*-----------------ADC-------------*/
#define ADCSAMPLING_N  50
/*---------------------------------------------------------*/
/* Subfunctions                                            */
/*---------------------------------------------------------*/

/*----------Transmitter----------*/

void transmitter(int(*bit), Complex(*signal));

void oversampling_GI(Complex(*modulated_signal), Complex(*transmitted_signal));
void IFFT(Complex(*FFT_signal), Complex(*OFDM_signal));
void overSampling(Complex(*modulated_signal), Complex(*OFDM_signal));
void addGI(Complex(*OFDM_signal), Complex(*transmitted_signal));

void estimateH(Complex(h1), Complex(h2), Complex(*H));
Complex generateOnePilot();
void generatePilots(Complex(*pilots));
void generateH(Complex(*estimatedPilots), Complex(*pilots), Complex(*H));
/*channel:*/
void awgn(Complex(*input_signal), Complex(*output_signal));
void generateTrueFading(Complex(*h1),Complex(*h2));

double PHI();
void Doppler(Complex(*h));
void setPopagation(Complex(*h));
void freSel_fading(Complex(*input), Complex(*output), Complex(h1), Complex(h2));

void removeGI(Complex(*signal), Complex(*output_signal));

void ADC(Complex(*received_signal), Complex(*estimated_signal));

void FFT_demodulation(Complex(*estimated_signal), Complex(*H), Complex(*modulated_signal));
void FFT(Complex(*estimated_signal), Complex(*recovered_signal));
void clearHimpact(Complex(*recovered_signal), Complex(*H));

void QPSK_demodulator(Complex(*signal), int(*received_bit));

void ber(int loop, int(*tbit), int(*rbit), double(*ber_i));

void getConvolution(Complex(h1), Complex(h2), Complex(*convolutionH));
void filter(Complex(*recovered_signal), Complex(*QPSK_signal));

//others
void checkH(Complex(*H));
