#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "Console.h"

#define N 16
#define M 8
#define BLOCKS 10
#define THREADS 64

#define OUT_OF_ARRAY -18

using namespace std;
using namespace utils;

__global__ void kernel_task1(int *vecBlockIdx, int *vecThreadIdx) {
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx < N) {
		vecBlockIdx[idx] = blockIdx.x;
		vecThreadIdx[idx] = threadIdx.x;
	}
}

void task1() {
	int *dev_vecBlockIdx = 0;
	int *dev_vecThreadIdx = 0;
	cudaMalloc((void**)&dev_vecBlockIdx, N * sizeof(int));
	cudaMalloc((void**)&dev_vecThreadIdx, N * sizeof(int));

	kernel_task1 << <N / THREADS + 1, THREADS >> >(dev_vecBlockIdx, dev_vecThreadIdx);

	int *vecBlockIdx = new int[N];
	int *vecThreadIdx = new int[N];

	cudaMemcpy(vecBlockIdx, dev_vecBlockIdx, N * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(vecThreadIdx, dev_vecThreadIdx, N * sizeof(int), cudaMemcpyDeviceToHost);

	cudaFree(dev_vecBlockIdx);
	cudaFree(dev_vecThreadIdx);

	printf("\n\nResults on host.");

	printf("\n\nvecBlockIdx: \n");
	// ¬ывел как матрицу просто дл€ большей нагл€дности
	Console::printMatrix(vecBlockIdx, 16, 8);

	printf("\n\nvecThreadIdx: \n");
	Console::printMatrix(vecThreadIdx, 16, 8);

	getchar();

	delete[] vecBlockIdx;
	delete[] vecThreadIdx;
}


__global__ void kernel_task2(int *matBlockIdx, int *matThreadIdx, int blocksCount, int threadsCount) {
	int _blockIdx = blockIdx.x * gridDim.y + blockIdx.y;

	if (_blockIdx < blocksCount) {
		matBlockIdx[_blockIdx] = blockIdx.x;
	} else { 
		matBlockIdx[_blockIdx] = OUT_OF_ARRAY;
	}

	int _threadIdxI = blockIdx.x * blockDim.x + threadIdx.x;
	int _threadIdxJ = blockIdx.y * blockDim.y + threadIdx.y;
	int _threadIdx = _threadIdxI * gridDim.y * blockDim.x + _threadIdxJ;
	if (_threadIdx < threadsCount) {
		matThreadIdx[_threadIdx] = threadIdx.x;
	} else {
		matThreadIdx[_threadIdx] = OUT_OF_ARRAY;
	}
}

void task2() {

	dim3 threadsPerBlock(8, 4);
	dim3 numBlocks((N + threadsPerBlock.x - 1) / threadsPerBlock.x, (M + threadsPerBlock.y - 1) / threadsPerBlock.y);
	unsigned blocksCount = numBlocks.x * numBlocks.y;
	unsigned threadsCount = numBlocks.x * threadsPerBlock.x * numBlocks.y * threadsPerBlock.y;

	cout << blocksCount << " " << threadsCount << endl;

	int *dev_matBlockIdx = 0;
	int *dev_matThreadIdx = 0;
	cudaMalloc((void**)&dev_matBlockIdx, blocksCount * sizeof(int));
	cudaMalloc((void**)&dev_matThreadIdx, threadsCount * sizeof(int));

	kernel_task2 << <numBlocks, threadsPerBlock >> >(dev_matBlockIdx, dev_matThreadIdx, blocksCount, threadsCount);

	int *matBlockIdx = new int[blocksCount];
	int *matThreadIdx = new int[threadsCount];

	cudaMemcpy(matBlockIdx, dev_matBlockIdx, blocksCount * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(matThreadIdx, dev_matThreadIdx, threadsCount * sizeof(int), cudaMemcpyDeviceToHost);

	printf("\n\nResults on host.");

	printf("\n\nmatBlockIdx: \n");
	Console::printMatrix(matBlockIdx, numBlocks.x, numBlocks.y);

	printf("\n\nmatThreadIdx: \n");
	Console::printMatrix(matThreadIdx, numBlocks.x * threadsPerBlock.x, numBlocks.y * threadsPerBlock.y);

	getchar();

	cudaFree(dev_matBlockIdx);
	cudaFree(dev_matThreadIdx);

	delete[] matBlockIdx;
	delete[] matThreadIdx;
}

int* generateArray(int size, int dispersion) {
	int* array = new int[size];
	for (int i = 0; i < size; ++i) {
		array[i] = rand() % dispersion;
	}
	return array;
}

__global__ void kernel_task3(int *matrix, int *vector, int *outVector, int rows, int cols) {
	int firstRowElemIdx = blockIdx.x * gridDim.y * blockDim.x;
	if (firstRowElemIdx < rows * cols) {
		int sum = 0;
		for (size_t col = 0; col < cols; col++) {
			sum += matrix[firstRowElemIdx + col] * vector[col];
		}
		outVector[blockIdx.x] = sum;
	}
}

void task3() {
	int rows = 3;
	int cols = 3;
	int elemsCount = rows * cols;

	int threadsPerBlock = rows;
	int blocksCount = cols / threadsPerBlock + 1;

	int *matrix = generateArray(elemsCount, 3);
	int *vector = generateArray(cols, 3);

	printf("\n Generated matrix: \n");
	Console::printMatrix(matrix, rows, cols);

	printf("\n Generated vector: ");
	Console::printArray(vector, cols);

	int *dev_matrix = 0;
	int *dev_vector = 0;
	int *dev_outVector = 0;
	cudaMalloc((void**)&dev_matrix, elemsCount * sizeof(int));
	cudaMalloc((void**)&dev_vector, cols * sizeof(int));
	cudaMalloc((void**)&dev_outVector, cols * sizeof(int));

	cudaMemcpy(dev_matrix, matrix, elemsCount * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_vector, vector, cols * sizeof(int), cudaMemcpyHostToDevice);

	kernel_task3 << <blocksCount, threadsPerBlock >> > (dev_matrix, dev_vector, dev_outVector, rows, cols);

	int *outVector = new int[cols];

	cudaMemcpy(outVector, dev_outVector, cols * sizeof(int), cudaMemcpyDeviceToHost);

	printf("\n\n Result vector: ");
	Console::printArray(outVector, cols);

	getchar();

	cudaFree(dev_matrix);
	cudaFree(dev_vector);
	cudaFree(dev_outVector);

	delete[] matrix;
	delete[] vector;
	delete[] outVector;
}

#define M_PI 3.14159265358979323846 
#define T 2 * M_PI
#define EPS 0.0001

float* arrayFromSegment(float pointA, float pointB, unsigned partsCount) {
	float* array = new float[partsCount];
	float step = (pointB - pointA) / (partsCount - 1);
	for (int i = 0; i < partsCount; ++i) {
		array[i] = pointA + step * i;
	}
	return array;
}

#define ROWS 10
#define COLS 5
#define ELEMS_COUNT (ROWS * COLS)

__global__ void kernel_task4A(float *matrix, float *vectorX, float *vectorY) {
	int _threadIdxI = blockIdx.x * blockDim.x + threadIdx.x;
	int _threadIdxJ = blockIdx.y * blockDim.y + threadIdx.y;
	int _threadIdx = _threadIdxI * gridDim.y * blockDim.x + _threadIdxJ;

	int mtxIdxI = _threadIdx / COLS;
	int mtxIdxJ = _threadIdx - (mtxIdxI * COLS);
	
	matrix[_threadIdx] = T * __sinf(vectorX[mtxIdxI]) * __cosf(vectorY[mtxIdxJ]);
}

void task4A() {
	printf("Rows count: %d, Cols count: %d \n", ROWS, COLS);

	dim3 threadsPerBlock(2, 2);
	dim3 blocksCount((ROWS + threadsPerBlock.x - 1) / threadsPerBlock.x,
					 (COLS + threadsPerBlock.y - 1) / threadsPerBlock.y);

	float *vectorX = arrayFromSegment(1.0f, 5.0f, COLS);
	float *vectorY = arrayFromSegment(1.0f, 5.0f, ROWS);

	printf("\n Generated vector X: \n");
	Console::printArray(vectorX, COLS);

	printf("\n Generated vector Y: \n");
	Console::printArray(vectorY, ROWS);

	float *dev_matrix = 0;
	float *dev_vectorX = 0;
	float *dev_vectorY = 0;
	cudaMalloc((void**)&dev_matrix, ELEMS_COUNT * sizeof(float));
	cudaMalloc((void**)&dev_vectorX, COLS * sizeof(float));
	cudaMalloc((void**)&dev_vectorY, ROWS * sizeof(float));

	cudaMemcpy(dev_vectorX, vectorX, COLS * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_vectorY, vectorY, ROWS * sizeof(float), cudaMemcpyHostToDevice);

	kernel_task4A << <blocksCount, threadsPerBlock >> > (dev_matrix, dev_vectorX, dev_vectorY);

	float *matrix = new float[ELEMS_COUNT];

	cudaMemcpy(matrix, dev_matrix, ELEMS_COUNT * sizeof(float), cudaMemcpyDeviceToHost);

	printf("\n\n Result matrix: \n");
	Console::printMatrix(matrix, ROWS, COLS);

	getchar();

	cudaFree(dev_matrix);
	cudaFree(dev_vectorX);
	cudaFree(dev_vectorY);

	delete[] matrix;
	delete[] vectorX;
	delete[] vectorY;
}

__device__ float numberFromSegment(float pointA, float pointB, unsigned partsCount, unsigned currStep) {
	float step = (pointB - pointA) / (partsCount - 1);
	return pointA + step * currStep;
}

__global__ void kernel_task4B(float *matrix, float s1PointA, float s1PointB, float s2PointA, float s2PointB) {
	int _threadIdxI = blockIdx.x * blockDim.x + threadIdx.x;
	int _threadIdxJ = blockIdx.y * blockDim.y + threadIdx.y;
	int _threadIdx = _threadIdxI * gridDim.y * blockDim.x + _threadIdxJ;

	int mtxIdxI = _threadIdx / COLS;
	int mtxIdxJ = _threadIdx - (mtxIdxI * COLS);

	float x = numberFromSegment(s1PointA, s1PointB, COLS, mtxIdxJ);
	float y = numberFromSegment(s2PointA, s2PointB, ROWS, mtxIdxI);

	matrix[_threadIdx] = T * __sinf(x) * __cosf(y);
}


void task4BC() {
	printf("Rows count: %d, Cols count: %d \n", ROWS, COLS);

	dim3 threadsPerBlock(2, 2);
	dim3 blocksCount((ROWS + threadsPerBlock.x - 1) / threadsPerBlock.x,
					 (COLS + threadsPerBlock.y - 1) / threadsPerBlock.y);

	float segment1PointA = 1.0;
	float segment1PointB = 5.0;
	float segment2PointA = 3.0;
	float segment2PointB = 15.0;

	float *dev_matrix = 0;
	cudaMalloc((void**)&dev_matrix, ELEMS_COUNT * sizeof(float));

	kernel_task4B << <blocksCount, threadsPerBlock >> > (dev_matrix, segment1PointA, segment1PointB, segment2PointA, segment2PointB);

	float *fromDev_matrix = new float[ELEMS_COUNT];
	cudaMemcpy(fromDev_matrix, dev_matrix, ELEMS_COUNT * sizeof(float), cudaMemcpyDeviceToHost);

	printf("\n\n Result from device matrix: \n");
	// Console::printMatrix(fromDev_matrix, ROWS, COLS);

	float *vectorX = arrayFromSegment(segment1PointA, segment1PointB, COLS);
	float *vectorY = arrayFromSegment(segment2PointA, segment2PointB, ROWS);
	float *host_matrix = new float[ELEMS_COUNT];
	float totalError = 0.;
	for (size_t i = 0; i < ROWS; i++) {
		for (size_t j = 0; j < COLS; j++) {
			host_matrix[i * COLS + j] = T * sinf(vectorX[j]) * cosf(vectorY[i]);
			totalError += host_matrix[i * COLS + j] - fromDev_matrix[i * COLS + j];
		}
	}

	printf("\n\n Result from host matrix: \n");
	// Console::printMatrix(host_matrix, ROWS, COLS);

	printf("\n\n Total error: %f", totalError);

	getchar();

	cudaFree(dev_matrix);

	delete[] vectorX;
	delete[] vectorY;
	delete[] fromDev_matrix;
	delete[] host_matrix;
}


int main() {

	// task1();
	// task2();
	// task3();
	task4A();
	// task4BC();

	return 0;
}