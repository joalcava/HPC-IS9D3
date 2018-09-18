#include <iostream>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "matrix_helpers.h"

using namespace std;

void ParallelMM(float **m1, float **m2, float* result, int from, int to, int size)
{
    for(int i = from; i < to; i++)
    {
        for(int j = 0; j < size; j++)
        {
            for(int k = 0; k < size; k++)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main(int argc, char ** argv) {
    if (argc != 3) {
        cout << "Debe ingresar el tamaño de la matriz cuadrada" << endl;
        cout << "Debe ingresar el numero de procesos." << endl;
        return 1;
    }

    int size = atoi(argv[1]);
    int processNumber = atoi(argv[2]);

    // for(size_t i = 0; i < 10; i++)
    // {
        float **matrix1 = (float **)malloc(size * sizeof(float *));
        for (int i = 0; i < size; ++i)
            matrix1[i] = (float *)malloc(size * sizeof(float));

        float **matrix2 = (float **)malloc(size * sizeof(float *));
        for (int i = 0; i < size; ++i)
            matrix2[i] = (float *)malloc(size * sizeof(float));

        // Shared memory
        float *result = (float *)malloc(size * sizeof(float *));
        int ARRAY_SIZE = (size * sizeof(float *));
        result = mmap(0, ARRAY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
        if (result == MAP_FAILED) {
            cout << "Error mapping result" << endl;
        }

        FillMatrix(matrix1, size);
        FillMatrix(matrix2, size);

        // SaveMatrixToCsvFile(matrix1, size, "m1.csv");
        // SaveMatrixToCsvFile(matrix2, size, "m2.csv");

        int normalStep = size/processNumber;
        int finalStep = size - (normalStep*processNumber);

        int processCount = processNumber;
        if (finalStep != 0) processCount++;
        cout << "Thread count: " << processCount << endl;

        auto start = std::chrono::high_resolution_clock::now();

        int lastPosition = 0;
        for(size_t i = 0; i < processCount; i++)
        {
            pid_t pid = fork();
            if (pid == 0) {
                if (finalStep != 0 && i == processCount) {
                    normalStep = finalStep;
                }
                lastPosition = lastPosition + normalStep;
                continue;
            }
            else if (pid > 0) {
                cout << "Launching from: " << lastPosition << " To: " << lastPosition + normalStep << endl;
                ParallelMM(matrix1, matrix2, result, lastPosition, lastPosition + normalStep, size);
                return 0;
            }
            else {
                cout << "Failed to Fork()" << endl;
                return 1;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        cout << "Elapsed time: " << elapsed.count() << endl;
    // }

    //SaveMatrixToCsvFile(result, size, "mr.csv");

    return 0;
}