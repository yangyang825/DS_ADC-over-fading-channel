#pragma once


void MatrixSubMatrixToMatrix(double(*a)[DIM], double(*b)[DIM], double(*c)[DIM]);  /* ���ξ���ļ���C[n*n]=A[n*n]-B[n*n] */

void MatrixAddMatrixToMatrix(double(*a)[DIM], double(*b)[DIM], double(*c)[DIM]); /* ���ξ���ļӷ�*/

void MatrixMulMatrixToMatrix(double(*a)[DIM], double(*b)[DIM], double(*c)[DIM]); /* ���ξ���ĳ˷�*/

void MatrixMulVectorToVector(double(*a)[DIM], double(*v1), double(*v2)); /* ���ξ���*������=������ */
 
void ScalarMulMatrixToMatrix(double s, double(*a)[DIM], double(*c)[DIM]); /* ����*���ξ���=���ξ���*/

void VectorMulVectorToScalar(double(*v1), double(*v2), double(*s)); /*������*������=����*/

void VectorAddVectorToVector(double(*v1), double(*v2), double(*v3)); /*������+������ */

void VectorMulMatrixToVector(double(*v1), double(*a)[DIM], double(*v2)); /*������*���ξ���=������*/

void VectorMulVectorToMatrix(double(*v1), double(*v2), double(*a)[DIM]); /* ������*������=���ξ��� */

void ScalarMulVectorToVector(double s, double(*v1), double(*v2)); /*����*������ */
