#ifndef HW4_CPP
#define HW4_CPP

#include <mpi.h>
#include "../utils/Generator.h"
#include "../utils/Console.h"

#define MASTER_PROCESS 0
#define ALPHA 1
#define BETA 2

using namespace utils;

namespace parallel_tasks {
    class HW4 {
    public:
        static void task1(int argc, char** argv) {
            MPI_Init (&argc, &argv);
            printf("Hello world!\n");
            MPI_Finalize();
        }

        static void task2(int argc, char** argv) {
            int rank, size;
            MPI_Init (&argc, &argv);
            MPI_Comm_rank (MPI_COMM_WORLD, &rank);
            MPI_Comm_size (MPI_COMM_WORLD, &size);
            printf("Hello from process %d of %d\n", rank, size);
            MPI_Finalize();
        }

        static void task3(int argc, char** argv) {
            int processRank, arraySize = 10;;
            MPI_Init (&argc, &argv);
            MPI_Comm_rank (MPI_COMM_WORLD, &processRank);

            if (processRank == MASTER_PROCESS) {
                int* array = Generator::generateArray(arraySize, 10);
                MPI_Send(array, arraySize, MPI_INT, 1, 530, MPI_COMM_WORLD);
            } else {
                int* array = new int[arraySize];
                MPI_Status status;
                MPI_Recv(array, arraySize, MPI_INT, MASTER_PROCESS, 530, MPI_COMM_WORLD, &status);
                Console::printArray("\nRecieved array: ", array, arraySize);
            }

            MPI_Finalize();
        }

        static void task4(int argc, char** argv) {
            int processRank, processesCnt, arraySize = 10;
            MPI_Init(&argc, &argv);
            MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
            MPI_Comm_size(MPI_COMM_WORLD, &processesCnt);

            if (processRank == MASTER_PROCESS) {
                int* array = Generator::generateArray(arraySize, 10);
                for (int process = 1; process < processesCnt; ++process) {
                    MPI_Send(array, arraySize, MPI_INT, process, 0, MPI_COMM_WORLD);
                }
            } else {
                MPI_Status status;
                MPI_Probe(MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
                if (!status.cancelled) {
                    int* array = new int[status.count];
                    MPI_Recv(array, (int)status.count, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
                    printf("\n Array recieved on %d process: \n", processRank);
                    Console::printArray(array, arraySize);
                }
            }

            MPI_Finalize();
        }

        static void BroadcastArray(int *array, int arraySize, int blockSize, int currentProcRank, int procCnt) {
            for (int process = 0; process < procCnt; ++process) {
                int offset = process * blockSize;
                if (offset < arraySize && process != currentProcRank) {
                    int sentElemsCnt = blockSize;
                    if (offset + blockSize > arraySize) {
                        // Determine the number of elements in the block were not fully included in the array
                        // For example: array: {1, 2, 3} block size: 2, offset: 2, then result will be = 1
                        sentElemsCnt = blockSize - (offset + blockSize - arraySize);
                    }
                    MPI_Send(array + offset, sentElemsCnt, MPI_INT, process, 0, MPI_COMM_WORLD);
                }
            }

        }

        static void task5(int argc, char** argv) {
            int procRank,
                procCnt,
                arraySize = 20,
                blockSize;
            MPI_Init(&argc, &argv);
            MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
            MPI_Comm_size(MPI_COMM_WORLD, &procCnt);
            blockSize = arraySize % procCnt == 0 ? arraySize / procCnt : arraySize / procCnt + 1;

            if (procRank == MASTER_PROCESS) {
                int* array = Generator::generateArray(arraySize, 10);
                Console::printArray("Generated array: ", array, arraySize);
                BroadcastArray(array, arraySize, blockSize, MASTER_PROCESS, procCnt);
                Console::printArray("Block on 0 process: ", array, blockSize);
            } else {
                int offset = procRank * blockSize;
                if (offset < arraySize) {
                    int receivedElemsCnt = blockSize;
                    if (offset + blockSize > arraySize) {
                        receivedElemsCnt = blockSize - (offset + blockSize - arraySize);
                    }
                    int *array = new int[blockSize];
                    MPI_Status status;
                    MPI_Recv(array, receivedElemsCnt, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);

                    printf("\n Block recieved on %d process: \n", procRank);
                    Console::printArray(array, receivedElemsCnt);
                }
            }

            MPI_Finalize();
        }

        static void task6(int argc, char** argv) {
            int procRank, procCnt;
            unsigned int vectSize = 12, blockSize;
            MPI_Init(&argc, &argv);
            MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
            MPI_Comm_size(MPI_COMM_WORLD, &procCnt);
            blockSize = vectSize % procCnt == 0 ? vectSize / procCnt : vectSize / procCnt + 1;

            if (procRank == MASTER_PROCESS) {
                printf("\nALPHA: %d, BETA: %d \n\n", ALPHA, BETA);
                vector<int> z(vectSize);
                vector<int> x = Generator::generateVector(vectSize, 10);
                vector<int> y = Generator::generateVector(vectSize, 10);
                printf("Generated vectors: \n");
                printf("X -> "); Console::printVector(x);
                printf("Y -> "); Console::printVector(y);

                // Send X, Y vectors to another processes
                BroadcastArray(&x[0], vectSize, blockSize, MASTER_PROCESS, procCnt);
                BroadcastArray(&y[0], vectSize, blockSize, MASTER_PROCESS, procCnt);

                // Compute part on master process
                for (unsigned int i = 0; i < blockSize; ++i) {
                    z[i] = ALPHA * x[i] + BETA * y[i];
                }
                // Receive parts of Z vector
                MPI_Status status;
                for (int process = 1; process < procCnt; ++process) {
                    unsigned int offset = process * blockSize;
                    if (offset + blockSize <= vectSize) {
                        MPI_Recv(&z[offset], blockSize, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
                    }
                }
                printf("\n Compiled vector: ");
                printf("\n Z -> "); Console::printVector(z);
            } else {
                unsigned int offset = procRank * blockSize;
                if (offset < vectSize) {
                    int receivedElemsCnt = blockSize;
                    if (offset + blockSize > vectSize) {
                        receivedElemsCnt = blockSize - (offset + blockSize - vectSize);
                    }
                    MPI_Status status;
                    vector<int> z(blockSize);
                    vector<int> x(blockSize);
                    vector<int> y(blockSize);
                    MPI_Recv(&x[0], receivedElemsCnt, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);
                    MPI_Recv(&y[0], receivedElemsCnt, MPI_INT, MASTER_PROCESS, 0, MPI_COMM_WORLD, &status);

                    // Compute and send to master process own part of Z vector
                    for (unsigned int i = 0; i < blockSize; ++i) {
                        z[i] = ALPHA * x[i] + BETA * y[i];
                    }
                    MPI_Send(&z[0], receivedElemsCnt, MPI_INT, MASTER_PROCESS, 1, MPI_COMM_WORLD);
                }
            }

            MPI_Finalize();
        }
    };
}

#endif