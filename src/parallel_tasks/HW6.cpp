#ifndef HW6_CPP
#define HW6_CPP

#include <mpi.h>
#include "../utils/Generator.h"
#include "../utils/Console.h"
#include "../mathf/Matrix.h"

#define MASTER_PROCESS 0
#define OUT_OF_ARRAY 0

using namespace utils;
using namespace mathf;

namespace parallel_tasks {
    class HW6 {
    public:

        static void divideArray(int arraySize, int blockSize, int factor, int partsCnt, int *offsets, int *blocksSizes) {
            for (int process = 0; process < partsCnt; ++process) {
                int offset = process * blockSize;
                if (offset < arraySize) {
                    int sentElemsCnt = blockSize;
                    if (offset + blockSize > arraySize) {
                        sentElemsCnt = blockSize - (offset + blockSize - arraySize);
                    }
                    offsets[process] = offset * factor;
                    blocksSizes[process] = sentElemsCnt * factor;
                } else {
                    offsets[process] = OUT_OF_ARRAY;
                    blocksSizes[process] = OUT_OF_ARRAY;
                }
            }
        }

        static void task1(int procRank, int procCnt) {
            unsigned vectSize = 10,
                     blockSize = (vectSize / procCnt) + 1;
            vector<int> v;
            int vectorNorm = 0;

            if (procRank == MASTER_PROCESS) {
                v = Generator::generateVector(vectSize, 2);
                printf("\nGenerated vector: ");
                Console::printVector(v);
            }

            int *offsets = new int[procCnt],
                *blocksSizes = new int[procCnt];
            divideArray(vectSize, blockSize, 1, procCnt, offsets, blocksSizes);
            blockSize = (unsigned) blocksSizes[procRank];

            if (blockSize != 0) {

                vector<int> vPart(blockSize);
                int vectorNormPart = 0;
                MPI_Scatterv(&v[0], blocksSizes, offsets, MPI_INT, &vPart[0],
                        blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

                printf("\n Scattered vector:");
                Console::printVector(vPart);

                for (unsigned i = 0; i < blockSize; ++i) {
                    vectorNormPart += abs(vPart[i]);
                }

                MPI_Reduce(&vectorNormPart, &vectorNorm, 1, MPI_INT, MPI_SUM, MASTER_PROCESS, MPI_COMM_WORLD);

                if (procRank == MASTER_PROCESS) {
                    printf("\nResult %d \n", vectorNorm);
                }
            }
        }

        static void task2(int procRank, int procCnt) {
            unsigned vectSize = 10,
                    blockSize = (vectSize / procCnt) + 1;
            vector<int> x, y;
            int dotProduct = 0;

            if (procRank == MASTER_PROCESS) {
                x = Generator::generateVector(vectSize, 3);
                y = Generator::generateVector(vectSize, 3);
                printf("\nGenerated vector x: ");
                Console::printVector(x);
                printf("\nGenerated vector y: ");
                Console::printVector(y);
            }

            int *offsets = new int[procCnt],
                *blocksSizes = new int[procCnt];
            divideArray(vectSize, blockSize, 1, procCnt, offsets, blocksSizes);
            blockSize = (unsigned) blocksSizes[procRank];

            if (blockSize != 0) {

                vector<int> xPart(blockSize);
                vector<int> yPart(blockSize);
                int dotProductPart = 0;
                MPI_Scatterv(&x[0], blocksSizes, offsets, MPI_INT, &xPart[0],
                        blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
                MPI_Scatterv(&y[0], blocksSizes, offsets, MPI_INT, &yPart[0],
                        blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

                printf("\n Scattered vector x:");
                Console::printVector(xPart);

                printf("\n Scattered vector y:");
                Console::printVector(yPart);

                for (unsigned i = 0; i < blockSize; ++i) {
                    dotProductPart += xPart[i] * yPart[i];
                }

                MPI_Reduce(&dotProductPart, &dotProduct, 1, MPI_INT, MPI_SUM, MASTER_PROCESS, MPI_COMM_WORLD);

                if (procRank == MASTER_PROCESS) {
                    printf("\nResult %d \n", dotProduct);
                }
            }
        }

        static void task5(int procRank, int procCnt) {
            unsigned rows = 4, cols = 4, elemsCnt = rows * cols;
            Matrix mtxA(rows, cols), mtxB(rows, cols), resultMtx(rows, cols);

            if (procRank == MASTER_PROCESS) {
                mtxA.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxB.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                printf("\nGenerated matrixes: \n");
                mtxA.printToConsole("A: \n");
                mtxB.printToConsole("B: \n");
                for (int process = 1; process < procCnt; ++process) {
                    MPI_Send(mtxB.begin(), elemsCnt, MPI_INT, process, 0, MPI_COMM_WORLD);
                }
            } else {
                MPI_Status status;
                MPI_Recv(mtxB.begin(), elemsCnt, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(rows, (rows / procCnt) + 1, cols, procCnt, offsets, blocksSizes);
            int blockSize = blocksSizes[procRank];
            Matrix mtxAPart(blockSize/cols, cols);
            MPI_Scatterv(mtxA.begin(), blocksSizes, offsets, MPI_INT, mtxAPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            Matrix resultMtxPart = mtxAPart * mtxB;

            MPI_Gatherv(resultMtxPart.begin(), blockSize, MPI_INT, resultMtx.begin(),
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            if (procRank == MASTER_PROCESS) {
                resultMtx.printToConsole("\nResult mtx: \n");
            }
        }

        static void task6(int procRank, int procCnt) {
            unsigned rows = 3, cols = 3, elemsCnt = rows * cols;
            Matrix matrix(rows, cols), vector(rows, 1);

            if (procRank == MASTER_PROCESS) {
                matrix.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                vector.assignData(Generator::generateArray(rows, 3), rows, 1);
                matrix.printToConsole("\nGenerated matrix: \n");
                vector.printToConsole("\nGenerated vector: \n");
            }
            MPI_Bcast(vector.begin(), rows, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(rows, (rows / procCnt) + 1, cols, procCnt, offsets, blocksSizes);
            int blockSize = blocksSizes[procRank];
            unsigned blockRows = blockSize / cols;
            Matrix matrixPart(blockRows, cols);

            MPI_Scatterv(matrix.begin(), blocksSizes, offsets, MPI_INT, matrixPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            Matrix resultVectPart = matrixPart * vector;

            for (int j = 0; j < procCnt; ++j) {
                offsets[j] = offsets[j] / cols;
                blocksSizes[j] = blocksSizes[j] / cols;
            }
            MPI_Gatherv(resultVectPart.begin(), blockRows, MPI_INT, vector.begin(),
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            if (procRank == MASTER_PROCESS) {
                vector.printToConsole("\nResult vector: \n");
            }
        }


        static void run(int argc, char **argv) {
            int processRank, processesCnt;
            MPI_Init (&argc, &argv);
            MPI_Comm_rank (MPI_COMM_WORLD, &processRank);
            MPI_Comm_size (MPI_COMM_WORLD, &processesCnt);

//            task1(processRank, processesCnt);
//            task2(processRank, processesCnt);
            task5(processRank, processesCnt);
//            task6(processRank, processesCnt);
        }
    };
}

#endif