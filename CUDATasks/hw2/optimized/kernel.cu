#include <stdio.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdio.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "Complex.h"

# define M_PI   3.14159265358979323846f
# define M_PI_2 1.57079632679489661923f
# define M_PI_4 0.785398163397448309616f
# define M_2_PI 0.636619772367581343076f
#define eps		1e-15f
#define el		0.5772156649015329f

#define N	5
#define H_T ((2.0f * M_PI) / (N - 1))

typedef complex<float> complexf;

__constant__ float a[14] = {
	-7.03125e-2,
	0.112152099609375,
	-0.5725014209747314,
	6.074042001273483,
	-1.100171402692467e2,
	3.038090510922384e3,
	-1.188384262567832e5,
	6.252951493434797e6,
	-4.259392165047669e8,
	3.646840080706556e10,
	-3.833534661393944e12,
	4.854014686852901e14,
	-7.286857349377656e16,
	1.279721941975975e19 
};
__constant__ float b[14] = {
	7.32421875e-2,
	-0.2271080017089844,
	1.727727502584457,
	-2.438052969955606e1,
	5.513358961220206e2,
	-1.825775547429318e4,
	8.328593040162893e5,
	-5.006958953198893e7,
	3.836255180230433e9,
	-3.649010818849833e11,
	4.218971570284096e13,
	-5.827244631566907e15,
	9.476288099260110e17,
	-1.792162323051699e20 
};

__device__ complexf besselh01Function(complexf z)
{
	complexf z1, z2, cr, cp, cs, cp0, cq0, cp1, cq1, ct1, ct2, cu;
	complexf cj0, cy0;
	complexf cii(0.0, 1.0);
	complexf cone(1.0, 0.0);
	complexf czero(0.0, 0.0);

	float a0, w0;
	int k, kz;

 	a0 = abs(z);
	z2 = z*z;
	z1 = z;
	if (a0 == 0.0f) {
		cj0 = cone;
		cy0 = complexf(-1e38f, 0.0f);
		return 0;
	}
	if (real(z) < 0.0f) z1 = -z;
	if (a0 <= 12.0f) {
		cj0 = cone;
		cr = cone;
		for (k = 1; k <= 40; k++) {
			cr *= -0.25f * z2 / (float) (k*k);
			cj0 += cr;
			if (abs(cr) < abs(cj0) * eps) break;
		}
		w0 = 0.0f;
		cr = cone;
		cs = czero;
		for (k = 1; k <= 40; k++) {
			w0 += 1.0f / k;
			cr *= -0.25f * z2 / (float) (k*k);
			cp = cr*w0;
			cs += cp;
			if (abs(cp) < abs(cs) * eps) break;
		}
		cy0 = M_2_PI * ((log(0.5f * z1) + el) * cj0 - cs);
	}
	else {
		if (a0 >= 50.0f) kz = 8;         // can be changed to 10
		else if (a0 >= 35.0f) kz = 10;   //   "      "     "  12
		else kz = 12;                    //   "      "     "  14
		ct1 = z1 - M_PI_4;
		cp0 = cone;
		for (k = 0; k<kz; k++) {
			cp0 += a[k] * pow(z1, -2.0f*k - 2.0f);
		}
		cq0 = -0.125f / z1;
		for (k = 0; k<kz; k++) {
			cq0 += b[k] * pow(z1, -2.0f*k - 3.0f);
		}
		cu = sqrt(M_2_PI / z1);
		cj0 = cu*(cp0*cos(ct1) - cq0*sin(ct1));
		cy0 = cu*(cp0*sin(ct1) + cq0*cos(ct1));
	}
	if (real(z) < 0.0f) {
		if (imag(z) < 0.0f) {
			cy0 -= 2.0f * cii * cj0;
		}
		else if (imag(z) > 0.0f) {
			cy0 += 2.0f * cii * cj0;
		}
	}
	return (cj0 + cii * cy0);
}

__device__ float computeR(int i, int j) 
{
	float t_i = H_T * i;
	float t_j = H_T * j;
	float x1 = cos(t_i);
	float x2 = sin(t_i);
	float y1 = cos(t_j);
	float y2 = sin(t_j);
	return sqrtf(powf(x1 - y1, 2) + powf(x2 - y2, 2));
}

__global__ void complexKernel(complexf *result, float* rArray)
{
	complexf hi(1.7, -2.7);

	int idx = blockIdx.x * blockDim.x + threadIdx.x;

	int elemsBeneathDiag = 0;
	for (int i = N, negateDispl = idx - i; negateDispl >= 0; i--) {
		elemsBeneathDiag += elemsBeneathDiag + 1;
		negateDispl -= i;
	}
	int displacedIdx = idx + elemsBeneathDiag;
	

	int mtxI = displacedIdx / N;
	int mtxJ = displacedIdx % N;

	float R = computeR(mtxI, mtxJ);

	int mtxIdx = mtxI * N + mtxJ;
	int transposeMtxIdx = mtxJ * N + mtxI;

	rArray[mtxIdx] = R;
	rArray[transposeMtxIdx] = R;

	result[mtxIdx] = besselh01Function(hi * R);
	result[transposeMtxIdx] = result[mtxIdx];
}

__host__ void printArray(float *array, unsigned size) {
	for (unsigned i = 0; i < size; ++i) {
		printf(" [%d]: %f; ", i, array[i]);
	}
}

__host__ void printArray(complexf *array, unsigned size) {
	for (unsigned i = 0; i < size; ++i) {
		printf(" [%d]: ", i);
		std::cout << array[i];
	}
}

int main()
{
	int elemsCnt  = N * N;
	int diagonalMtxElemsCount = (elemsCnt - N) / 2 + N;

	printf("\n\n diagonalElemsCount = %d \n", diagonalMtxElemsCount);

	dim3 threadsPerBlock(2); // TODO: Make 32 and higher
	dim3 numBlocks((diagonalMtxElemsCount + threadsPerBlock.x - 1) / threadsPerBlock.x);

	printf("\n\n num blocks = %d \n", numBlocks.x);

	int extendedElemsCount = N * N + threadsPerBlock.x - 1;

	printf("\n\n num extendedElemsCount = %d \n", extendedElemsCount);

	complexf *dev_result;
	cudaMalloc((void**)&dev_result, sizeof(complexf) * extendedElemsCount);

	float *dev_rArray;
	cudaMalloc((void**)&dev_rArray, sizeof(float) * extendedElemsCount);

	complexKernel << <numBlocks, threadsPerBlock >> >(dev_result, dev_rArray);

	complexf *result = new complexf[N * N];
	float *rArray = new float[N * N];

	cudaMemcpy(result, dev_result, sizeof(complexf) * N * N, cudaMemcpyDeviceToHost);
	cudaMemcpy(rArray, dev_rArray, sizeof(float) * N * N, cudaMemcpyDeviceToHost);

	printf("N: %d \n\n\n", N);

	printf("R: \n");
	printArray(rArray, N * N);

	/*printf("\n\n\n Rsutlt: \n");
	printArray(result, N * N);*/

	cudaFree(&dev_rArray);
	cudaFree(&dev_result);

	delete rArray;
	delete result;

	getchar();

    return 0;
}

