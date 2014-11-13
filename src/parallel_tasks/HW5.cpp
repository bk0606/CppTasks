#ifndef HW5_CPP
#define HW5_CPP

#include <mpi.h>
#include "../utils/Generator.h"
#include "../utils/Console.h"
#include "../mathf/Matrix.h"

#define MASTER_PROCESS 0
#define OUT_OF_ARRAY 0

#define ALPHA 1
#define BETA 1
#define GAMMA 1

using namespace utils;
using namespace mathf;

namespace parallel_tasks {

    class HW5 {
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

        static void task1A(int procRank, int procCnt) {
            unsigned arraySize = 10, blockSize;
            int *Z, *X, *Y;

            if (procRank == MASTER_PROCESS) {
                Z = new int[arraySize];
                X = Generator::generateArray(arraySize, 10);
                Y = Generator::generateArray(arraySize, 10);
                Console::printArray("Generated X: ", X, arraySize);
                Console::printArray("Generated Y: ", Y, arraySize);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(arraySize, (arraySize / procCnt) + 1, 1, procCnt, offsets, blocksSizes);
            blockSize = (unsigned) blocksSizes[procRank];
            int *ZArrayBlock = new int[blockSize],
                *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (unsigned i = 0; i < blockSize; ++i) {
                ZArrayBlock[i] = ALPHA * XArrayBlock[i] + BETA * YArrayBlock[i] + GAMMA;
            }

            MPI_Gatherv(ZArrayBlock, blockSize, MPI_INT, Z, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nGather Z: ", Z, arraySize);
            }
        }

        static void task1B(int procRank, int procCnt) {
            unsigned arraySize = 10, blockSize;
            int *X, *Y;

            if (procRank == MASTER_PROCESS) {
                X = Generator::generateArray(arraySize, 10);
                Y = Generator::generateArray(arraySize, 10);
                Console::printArray("Generated X: ", X, arraySize);
                Console::printArray("Generated Y: ", Y, arraySize);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(arraySize, (arraySize / procCnt) + 1, 1, procCnt, offsets, blocksSizes);
            blockSize = (unsigned) blocksSizes[procRank];
            int *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (unsigned i = 0; i < blockSize; ++i) {
                YArrayBlock[i] = ALPHA * XArrayBlock[i] + BETA * YArrayBlock[i] + GAMMA;
            }

            MPI_Gatherv(YArrayBlock, blockSize, MPI_INT, Y, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nGather Y: ", Y, arraySize);
            }
        }

        static void task1C(int procRank, int procCnt) {
            unsigned arraySize = 10, blockSize;
            int *Z, *X, *Y;

            if (procRank == MASTER_PROCESS) {
                Z = new int[arraySize];
                X = Generator::generateArray(arraySize, 10);
                Y = Generator::generateArray(arraySize, 10);
                Console::printArray("Generated X: ", X, arraySize);
                Console::printArray("Generated Y: ", Y, arraySize);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(arraySize, (arraySize / procCnt) + 1, 1, procCnt, offsets, blocksSizes);
            blockSize = (unsigned) blocksSizes[procRank];
            int *ZArrayBlock = new int[blockSize],
                *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (unsigned i = 0; i < blockSize; ++i) {
                ZArrayBlock[i] = ALPHA * XArrayBlock[i] * XArrayBlock[i] * YArrayBlock[i];
            }

            MPI_Gatherv(ZArrayBlock, blockSize, MPI_INT, Z, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nGather Z: ", Z, arraySize);
            }
        }

        static void task1D(int procRank, int procCnt) {
            unsigned arraySize = 10, blockSize;
            int *X, *Y;

            if (procRank == MASTER_PROCESS) {
                X = Generator::generateArray(arraySize, 10);
                Y = Generator::generateArray(arraySize, 10);
                Console::printArray("Generated X: ", X, arraySize);
                Console::printArray("Generated Y: ", Y, arraySize);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(arraySize, (arraySize / procCnt) + 1, 1, procCnt, offsets, blocksSizes);
            blockSize = (unsigned) blocksSizes[procRank];
            int *ZArrayBlock = new int[blockSize],
                *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (unsigned i = 0; i < blockSize; ++i) {
                ZArrayBlock[i] = YArrayBlock[i];
                YArrayBlock[i] = XArrayBlock[i];
                XArrayBlock[i] = ZArrayBlock[i];
            }

            MPI_Gatherv(XArrayBlock, blockSize, MPI_INT, X, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Gatherv(YArrayBlock, blockSize, MPI_INT, Y, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nSwapped X: ", X, arraySize);
                Console::printArray("\n\nSwapped Y: ", Y, arraySize);
            }
        }

        static void task2A(int procRank, int procCnt) {
            unsigned rows = 3, cols = 3, elemsCnt = rows * cols;
            Matrix mtxA(rows, cols), mtxB(rows, cols), mtxC(rows, cols), resultMtx(rows, cols);

            if (procRank == MASTER_PROCESS) {
                mtxA.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxB.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxC.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                printf("\nGenerated matrixes: \n");
                mtxA.printToConsole("A: \n");
                mtxB.printToConsole("B: \n");
                mtxC.printToConsole("C: \n");
                for (int process = 1; process < procCnt; ++process) {
                    MPI_Send(mtxB.begin(), elemsCnt, MPI_INT, process, 0, MPI_COMM_WORLD);
                    MPI_Send(mtxC.begin(), elemsCnt, MPI_INT, process, 1, MPI_COMM_WORLD);
                }
            } else {
                MPI_Status status;
                MPI_Recv(mtxB.begin(), elemsCnt, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(mtxC.begin(), elemsCnt, MPI_INT, MASTER_PROCESS, 1, MPI_COMM_WORLD, &status);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(rows, (rows / 2) + 1, cols, procCnt, offsets, blocksSizes);
            int blockSize = blocksSizes[procRank];
            Matrix mtxAPart(blockSize/cols, cols);
            MPI_Scatterv(mtxA.begin(), blocksSizes, offsets, MPI_INT, mtxAPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            Matrix resultMtxPart = mtxAPart * mtxB * mtxC;

            MPI_Gatherv(resultMtxPart.begin(), blockSize, MPI_INT, resultMtx.begin(),
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            if (procRank == MASTER_PROCESS) {
                resultMtx.printToConsole("\nResult mtx: \n");
            }

        }

        static void task2B(int procRank, int procCnt) {
            unsigned rows = 3, cols = 3, elemsCnt = rows * cols;
            Matrix mtxA(rows, cols), mtxB(rows, cols), mtxC(rows, cols),
                   mtxD(rows, cols), resultMtx(rows, cols);

            if (procRank == MASTER_PROCESS) {
                mtxA.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxB.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxC.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxD.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                printf("\nGenerated matrixes: \n");
                mtxA.printToConsole("A:\n");
                mtxB.printToConsole("B:\n");
                mtxC.printToConsole("C:\n");
                mtxD.printToConsole("D:\n");
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(rows, (rows / procCnt) + 1, cols, procCnt, offsets, blocksSizes);
            unsigned blockSize = (unsigned) blocksSizes[procRank],
                    blockRows = blockSize/cols;
            Matrix mtxAPart(blockRows, cols), mtxBPart(blockRows, cols),
                    mtxCPart(blockRows, cols), mtxDPart(blockRows, cols);

            MPI_Scatterv(mtxA.begin(), blocksSizes, offsets, MPI_INT, mtxAPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(mtxB.begin(), blocksSizes, offsets, MPI_INT, mtxBPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(mtxC.begin(), blocksSizes, offsets, MPI_INT, mtxCPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(mtxD.begin(), blocksSizes, offsets, MPI_INT, mtxDPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            Matrix resultMtxPart (blockRows, cols);
            for (unsigned i = 0; i < blockRows; ++i) {
                for (unsigned j = 0; j < cols; ++j) {
                    resultMtxPart(i, j) = mtxAPart(i, j) * mtxBPart(i, j) + mtxCPart(i, j) * mtxDPart(i, j);
                }
            }

            MPI_Gatherv(resultMtxPart.begin(), blockSize, MPI_INT, resultMtx.begin(),
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            if (procRank == MASTER_PROCESS) {;
                resultMtx.printToConsole("\nResult matrix: \n");
            }

        }

        static void task2C(int procRank, int procCnt) {
            unsigned rows = 3, cols = 3, elemsCnt = rows * cols;
            Matrix mtxA(rows, cols), mtxATranspose(cols, rows), resultMtx(rows, cols);

            if (procRank == MASTER_PROCESS) {
                mtxA.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxATranspose = Matrix::transpose(mtxA);
                mtxA.printToConsole("\nGenerated mtx A: \n");
                mtxATranspose.printToConsole("\nTransposed mtx A: \n");
                for (int process = 1; process < procCnt; ++process) {
                    MPI_Send(mtxATranspose.begin(), elemsCnt, MPI_INT, process, 1, MPI_COMM_WORLD);
                }
            } else {
                MPI_Status status;
                MPI_Recv(mtxATranspose.begin(), elemsCnt, MPI_INT, MASTER_PROCESS, 1, MPI_COMM_WORLD, &status);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(rows, (rows / procCnt) + 1, cols, procCnt, offsets, blocksSizes);
            int blockSize = blocksSizes[procRank];
            Matrix mtxAPart(blockSize/cols, cols);
            MPI_Scatterv(mtxA.begin(), blocksSizes, offsets, MPI_INT, mtxAPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            Matrix resultMtxPart = mtxAPart * mtxATranspose;

            MPI_Gatherv(resultMtxPart.begin(), blockSize, MPI_INT, resultMtx.begin(),
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            if (procRank == MASTER_PROCESS) {
                resultMtx.printToConsole("\nResult mtx: \n");
            }

        }

        static void task3A(int procRank, int procCnt) {
            unsigned rows = 3, cols = 4, elemsCnt = rows * cols,
                     diagonalLen = rows < cols ? rows : cols;
            Matrix matrix(rows, cols);
            vector<int> diagonalVect(diagonalLen);

            if (procRank == MASTER_PROCESS) {
                matrix.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                matrix.printToConsole("\nGenerated matrix: \n");
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(diagonalLen, (diagonalLen / procCnt) + 1, cols, procCnt, offsets, blocksSizes);
            int blockSize = blocksSizes[procRank];
            unsigned blockRows = blockSize / cols,
                     offsetRows = offsets[procRank] / cols;
            vector<int> diagonalVectPart(blockRows);
            Matrix matrixPart(blockRows, cols);
            MPI_Scatterv(matrix.begin(), blocksSizes, offsets, MPI_INT, matrixPart.begin(),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (unsigned i = 0; i < blockRows; ++i) {
                diagonalVectPart[i] = matrixPart(i, offsetRows + i);
            }

            for (int j = 0; j < procCnt; ++j) {
                offsets[j] = offsets[j] / cols;
                blocksSizes[j] = blocksSizes[j] / cols;
            }
            MPI_Gatherv(&diagonalVectPart[0], blockRows, MPI_INT, &diagonalVect[0],
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            if (procRank == MASTER_PROCESS) {
                printf("\nResult diagonal vector: ");
                Console::printVector(diagonalVect);
            }
        }

        static void task3B(int procRank, int procCnt) {
            unsigned rows = 3, cols = 3, elemsCnt = rows * cols;
            Matrix matrix(rows, cols), vector(rows, 1);

            if (procRank == MASTER_PROCESS) {
                matrix.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                vector.assignData(Generator::generateArray(rows, 3), rows, 1);
                matrix.printToConsole("\nGenerated matrix: \n");
                vector.printToConsole("\nGenerated vector: \n");
                for (int process = 1; process < procCnt; ++process) {
                    MPI_Send(vector.begin(), rows, MPI_INT, process, 0, MPI_COMM_WORLD);
                }
            }
            else {
                MPI_Status status;
                MPI_Recv(vector.begin(), rows, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
            }

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

        static void task4A(int procRank, int procCnt) {
            unsigned n = 1000;
            char* sending = new char[n];
            if (procRank == MASTER_PROCESS) {
                for (unsigned i = 0; i < n; ++i) {
                    sending[i] = 'a';
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double start = MPI_Wtime();

            if (procRank == MASTER_PROCESS) {
                MPI_Status status;
                MPI_Send(sending, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(sending, n, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
            } else if (procRank == 1) {
                MPI_Status status;
                MPI_Recv(sending, n, MPI_CHAR, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
                MPI_Send(sending, n, MPI_CHAR, MASTER_PROCESS, 1, MPI_COMM_WORLD);
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double end = MPI_Wtime();

            if (procRank == MASTER_PROCESS) {
                printf("\n Result time: %f \n", end - start);
            }
        }

        static void task4B(int procRank, int procCnt) {
            unsigned n = 1000;
            char* sendingA = new char[n];
            char* sendingB = new char[n];
            if (procRank == MASTER_PROCESS) {
                for (unsigned i = 0; i < n; ++i) {
                    sendingA[i] = 'a';
                    sendingB[i] = 'b';
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double start = MPI_Wtime();

            MPI_Request sendRequest;
            MPI_Request receiveRequest;
            if (procRank == MASTER_PROCESS) {
                MPI_Isend(sendingA, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &sendRequest);
                MPI_Irecv(sendingB, n, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &receiveRequest);
            } else if (procRank == 1) {
                MPI_Isend(sendingB, n, MPI_CHAR, MASTER_PROCESS, 1, MPI_COMM_WORLD, &sendRequest);
                MPI_Irecv(sendingA, n, MPI_CHAR, MASTER_PROCESS, 0, MPI_COMM_WORLD, &receiveRequest);
            }
            MPI_Status status;
            MPI_Wait(&sendRequest, &status);
            MPI_Wait(&receiveRequest, &status);

            MPI_Barrier(MPI_COMM_WORLD);
            double end = MPI_Wtime();

            if (procRank == MASTER_PROCESS) {
                printf("\nResult time: %f \n", end - start);
            }
        }

        static void run(int argc, char **argv) {
            int processRank, processesCnt;
            MPI_Init (&argc, &argv);
            MPI_Comm_rank (MPI_COMM_WORLD, &processRank);
            MPI_Comm_size (MPI_COMM_WORLD, &processesCnt);

//            task1A(processRank, processesCnt);
//            task1B(processRank, processesCnt);
//            task1C(processRank, processesCnt);
//            task1D(processRank, processesCnt);
//            task2A(processRank, processesCnt);
//            task2B(processRank, processesCnt);
//            task2C(processRank, processesCnt);
//            task3A(processRank, processesCnt);
//            task3B(processRank, processesCnt);
//            task4A(processRank, processesCnt);
            task4B(processRank, processesCnt);

            MPI_Finalize();
        }

    };
}

#endif