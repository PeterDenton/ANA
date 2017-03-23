/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#include "Matrix.h"

// v2 = v1*A where v are row vectors a.la 1.5.8 from Hipparcos
void vector_dot_matrix(double A[3][3], double v1[3], double * v2)
{
	for (int i = 0; i < 3; i++)
	{
		v2[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			v2[i] += A[j][i] * v1[j];
		} // j
	} // i
}

