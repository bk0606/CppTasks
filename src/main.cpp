#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#define MAIN_PROCESS 0

void printArray(int *array, int size) {
    for (unsigned i = 0; i < size; ++i) {
        printf(" [%d]: %d; ", i, array[i]);
    }
}

int main (int argc, char** argv) {
    srand((unsigned int) time(NULL));
    int processRank, processesCnt;
    int *array;
    int rows = 3, cols = 3, arraySize = rows * cols;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &processRank);
    MPI_Comm_size (MPI_COMM_WORLD, &processesCnt);

    int blockSize = arraySize / processesCnt;

    float globalNorm = 0.0f;
    float localNorm = 0.0f;

    if (processRank == MAIN_PROCESS) {
        array = new int[arraySize];
        for (int i = 0; i < arraySize * arraySize; ++i) {
            array[i] = rand() % 5;
        }
        printf("Created array: ");
        printArray(array, arraySize);
        for (int process = 1; process < processesCnt; process += 1) {
            MPI_Send(&array[process * blockSize], blockSize, MPI_INT, process, 0, MPI_COMM_WORLD);
        }

    } else {
        int *recvArray = new int[blockSize];
        MPI_Status status;
        MPI_Recv(recvArray, blockSize, MPI_INT, MAIN_PROCESS, 0, MPI_COMM_WORLD, &status);

        printf("\n Block recieved on %d process: \n", processRank);
        printArray(recvArray, blockSize);

        for (int i = 0; i < cols; ++i) {
            localNorm += powf(recvArray[i], 2);
        }
    }

    MPI_Reduce(&localNorm, &globalNorm, 1, MPI_FLOAT, MPI_SUM, MAIN_PROCESS, MPI_COMM_WORLD);

    if (processRank == MAIN_PROCESS) {
        for (int j = 0; j < blockSize; ++j) {
            globalNorm += powf(array[j], 2);
        }
        globalNorm = sqrtf(globalNorm);
        printf("\nReduced number: %f ", globalNorm);
    }

}