#pragma once


//���帴���ṹ��a+bj=real+image*j 
typedef struct {
    double real, image;
}Complex;


Complex initComplex(double real, double image);

Complex ComplexAdd(Complex a, Complex b); //�����ӷ� 

Complex ComplexMinus(Complex a, Complex b);	// ��������

Complex ComplexMulti(Complex a, Complex b);	//�����˷�

Complex  ComplexConjugate(Complex a);	//�����˷� 

Complex Exp(double phi);	//e^j��,������ָ����ʽ

double Modules(Complex a); 	//������ģ

Complex ComplexDivision(Complex a, Complex b);  //�������� 
