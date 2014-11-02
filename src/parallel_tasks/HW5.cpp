#ifndef HW5_CPP
#define HW5_CPP

#include <mpi.h>
#include "../utils/Generator.h"
#include "../utils/Console.h"
#include "../mathf/Matrix.h"

#define MASTER_PROCESS 0
#define OUT_OF_ARRAY -1

#define ALPHA 1
#define BETA 1
#define GAMMA 1

using namespace utils;
using namespace mathf;

namespace parallel_tasks {

    class HW5 {
    public:

        typedef void (*ptr_arraysHandlerDelegate)(int *Z, int *X, int *Y, int arraySize,
                int procRank, int *offsets, int* blocksSizes);

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

        static void task1(int procRank, int procCnt, ptr_arraysHandlerDelegate arraysHandler) {
            int arraySize = 10,
                blockSize;
            int *Z, *X, *Y;
            blockSize = arraySize % procCnt == 0 ? arraySize / procCnt : arraySize / procCnt + 1;

            if (procRank == MASTER_PROCESS) {
                Z = new int[arraySize];
                X = Generator::generateArray(arraySize, 10);
                Y = Generator::generateArray(arraySize, 10);
                Console::printArray("Generated X: ", X, arraySize);
                Console::printArray("Generated Y: ", Y, arraySize);
            }

            int *offsets = new int[procCnt], *blocksSizes = new int[procCnt];
            divideArray(arraySize, blockSize, 1, procCnt, offsets, blocksSizes);

            arraysHandler(Z, X, Y, arraySize, procRank, offsets, blocksSizes);
        }

        static void task1ADelegate(int *Z, int *X, int *Y, int arraySize, int procRank, int *offsets, int* blocksSizes) {
            int blockSize = blocksSizes[procRank];
            int *ZArrayBlock = new int[blockSize],
                *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (int i = 0; i < blockSize; ++i) {
                ZArrayBlock[i] = ALPHA * XArrayBlock[i] + BETA * YArrayBlock[i] + GAMMA;
            }

            MPI_Gatherv(ZArrayBlock, blockSize, MPI_INT, Z, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nGather Z: ", Z, arraySize);
            }
        }

        static void task1BDelegate(int *Z, int *X, int *Y, int arraySize, int procRank, int *offsets, int* blocksSizes) {
            int blockSize = blocksSizes[procRank];
            int *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (int i = 0; i < blockSize; ++i) {
                YArrayBlock[i] = ALPHA * XArrayBlock[i] + BETA * YArrayBlock[i] + GAMMA;
            }

            MPI_Gatherv(YArrayBlock, blockSize, MPI_INT, Y, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nGather Y: ", Y, arraySize);
            }
        }

        static void task1CDelegate(int *Z, int *X, int *Y, int arraySize, int procRank, int *offsets, int* blocksSizes) {
            int blockSize = blocksSizes[procRank];
            int *ZArrayBlock = new int[blockSize],
                *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (int i = 0; i < blockSize; ++i) {
                ZArrayBlock[i] = ALPHA * XArrayBlock[i] * XArrayBlock[i] * YArrayBlock[i];
            }

            MPI_Gatherv(ZArrayBlock, blockSize, MPI_INT, Z, blocksSizes,
                    offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\n\nGather Z: ", Z, arraySize);
            }
        }

        static void task1DDelegate(int *Z, int *X, int *Y, int arraySize, int procRank, int *offsets, int* blocksSizes) {
            int blockSize = blocksSizes[procRank];
            int *ZArrayBlock = new int[blockSize],
                *XArrayBlock = new int[blockSize],
                *YArrayBlock = new int[blockSize];

            MPI_Scatterv(X, blocksSizes, offsets, MPI_INT, XArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);
            MPI_Scatterv(Y, blocksSizes, offsets, MPI_INT, YArrayBlock,
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            for (int i = 0; i < blockSize; ++i) {
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
            divideArray(rows, (rows + 1) / 2, cols, procCnt, offsets, blocksSizes);
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
            divideArray(rows, (rows + 1) / 2, cols, procCnt, offsets, blocksSizes);
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
            divideArray(rows, (rows + 1) / 2, cols, procCnt, offsets, blocksSizes);
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

        static void run(int argc, char **argv) {
            int procRank, procCnt;
            MPI_Init (&argc, &argv);
            MPI_Comm_rank (MPI_COMM_WORLD, &procRank);
            MPI_Comm_size (MPI_COMM_WORLD, &procCnt);

//            task1(procRank, procCnt, task1BDelegate);
//            task2A(procRank, procCnt);
//            task2B(procRank, procCnt);
            task2C(procRank, procCnt);


            MPI_Finalize();
        }

    };
}

#endif