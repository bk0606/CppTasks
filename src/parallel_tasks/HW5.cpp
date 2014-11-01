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

    class ArrayDivider {
    public:
        int *getOffsets() {
            return offsets_;
        }

        int *getBlocksSizes() {
            return blocksSizes_;
        }

        ArrayDivider(int arraySize, int blockSize, int partsCnt)
                : arraySize_(arraySize), blockSize_(blockSize), partsCnt_(partsCnt) {
            offsets_ = new int[partsCnt];
            blocksSizes_ = new int[partsCnt];
            computeDivision();
        }

        virtual ~ArrayDivider() {
            delete[] offsets_;
            delete[] blocksSizes_;
        }

    private:
        int *offsets_, *blocksSizes_;
        int arraySize_, blockSize_, partsCnt_;

        void computeDivision() {
            for (int process = 0; process < partsCnt_; ++process) {
                int offset = process * blockSize_;
                if (offset < arraySize_) {
                    int sentElemsCnt = blockSize_;
                    if (offset + blockSize_ > arraySize_) {
                        sentElemsCnt = blockSize_ - (offset + blockSize_ - arraySize_);
                    }
                    offsets_[process] = offset;
                    blocksSizes_[process] = sentElemsCnt;
                } else {
                    offsets_[process] = OUT_OF_ARRAY;
                    blocksSizes_[process] = OUT_OF_ARRAY;
                }
            }
        }
    };
    
    class HW5 {
    public:
        typedef void (*ptr_arraysHandlerDelegate)(int *Z, int *X, int *Y, int arraySize,
                int procRank, int *offsets, int* blocksSizes);

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

            ArrayDivider divider(arraySize, blockSize, procCnt);

            arraysHandler(Z, X, Y, arraySize, procRank, divider.getOffsets(), divider.getBlocksSizes());
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

        static void task2(int procRank, int procCnt) {
            unsigned rows = 3, cols = 3, elemsCnt = rows * cols;
            Matrix mtxA(rows, cols), mtxB(rows, cols), mtxC(rows, cols), resultMtx(rows, cols);
            int *offsets = new int[procCnt],
                *blocksSizes = new int[procCnt];

            divideArray(rows, (rows + 1) / 2, cols, procCnt, offsets, blocksSizes);
            int blockSize = blocksSizes[procRank];
            Matrix mtxAPart(blockSize/cols, cols);

            if (procRank == MASTER_PROCESS) {
                mtxA.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxB.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                mtxC.assignData(Generator::generateArray(elemsCnt, 3), rows, cols);
                for (int process = 1; process < procCnt; ++process) {
                    MPI_Send(&mtxB(0, 0), elemsCnt, MPI_INT, process, 0, MPI_COMM_WORLD);
                    MPI_Send(&mtxC(0, 0), elemsCnt, MPI_INT, process, 1, MPI_COMM_WORLD);
                }
            } else {
                MPI_Status status;
                MPI_Recv(&mtxB(0, 0), elemsCnt, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&mtxC(0, 0), elemsCnt, MPI_INT, MASTER_PROCESS, 1, MPI_COMM_WORLD, &status);
            }
            MPI_Scatterv(&mtxA(0, 0), blocksSizes, offsets, MPI_INT, &mtxAPart(0, 0),
                    blockSize, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            Matrix resultMtxPart = mtxAPart * mtxB * mtxC;

            MPI_Gatherv(&resultMtxPart(0, 0), blockSize, MPI_INT, &resultMtx(0, 0),
                    blocksSizes, offsets, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

            if (procRank == MASTER_PROCESS) {
                Console::printArray("\nResult mtx: ", &resultMtx(0, 0), elemsCnt);
            }

        }

        static void run(int argc, char **argv) {
            int procRank, procCnt;
            MPI_Init (&argc, &argv);
            MPI_Comm_rank (MPI_COMM_WORLD, &procRank);
            MPI_Comm_size (MPI_COMM_WORLD, &procCnt);

            task2(procRank, procCnt);

            MPI_Finalize();
        }

    };
}

#endif