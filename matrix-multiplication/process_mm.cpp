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

// Performs the multiplication of a segment of the matrix
void MMSegment(float **m1, float **m2, float* result, int from, int to, int size)
{
    //printf("[%d] From: %d To: %d\n", getpid(), from, to);
    for(int i = from; i < to; i++)
    {
        for(int j = 0; j < size; j++)
        {
            float cellTotal = 0;
            for(int k = 0; k < size; k++)
            {
                cellTotal += m1[i][k] * m2[k][j];
            }
            result[i * size + j] = cellTotal;
        }
    }
}

int main(int argc, char ** argv) {
    if (argc != 3) {
        cout << "Enter the matrix size (it's square)" << endl;
        cout << "Enter the number of processes" << endl;
        return 1;
    }

    int size = atoi(argv[1]);
    int processNumber = atoi(argv[2]);

    float **matrix1 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix1[i] = (float *)malloc(size * sizeof(float));

    float **matrix2 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix2[i] = (float *)malloc(size * sizeof(float));

    FillMatrix(matrix1, size);
    FillMatrix(matrix2, size);
    // SaveMatrixToCsvFile(matrix1, size, "m1.csv");
    // SaveMatrixToCsvFile(matrix2, size, "m2.csv");

    // Shared memory
    float *resultMmap = (float *) malloc(size * size * sizeof(float));
    resultMmap = (float *) mmap(0, size * size * sizeof(float), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (resultMmap == MAP_FAILED)
    {
        perror("Error mmapping result");
        exit(EXIT_FAILURE);
    }

    double meanElapsedTime = 0;
    double *times = (double *)malloc(10 * sizeof(double *));
    int normalStep = size/processNumber;
    int finalStep = size - (normalStep*processNumber);
    int processCount = processNumber;
    if (finalStep != 0) processCount++;
    //cout << "Number of process to be launched: " << processCount << endl;

    for(size_t i = 0; i < 10; i++)
    {
        // Clean result matrix
        for(size_t i = 0; i < size * size; i++)
        {
            resultMmap[i] = 0;
        }
        //cout << "Filled." << endl;

        int normalStep = size/processNumber;
        int finalStep = size - (normalStep*processNumber);
        int *childsPids = (int *) malloc(processCount * sizeof(int));
        int lastPosition = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i < processCount; i++)
        {
            pid_t pid = fork();

            // Child process
            if (pid == 0) {
                //cout << "[" << getpid() << "] " << lastPosition << " - " << lastPosition + normalStep << endl;
                MMSegment(matrix1, matrix2, resultMmap, lastPosition, lastPosition + normalStep, size);
                //cout << "[" << getpid() << "] " << "Finished" << endl;
                return 0;
            }

            // Father process
            else if (pid > 0) {
                childsPids[i] = pid;
                // Is the final step different of normal step and is this the final step?
                if (finalStep != 0 && i == processCount) {
                    normalStep = finalStep;
                }
                lastPosition = lastPosition + normalStep;
            }

            // Failed to fork
            else {
                cout << "Failed to Fork()" << endl;
                return 1;
            }
        }

        int status = 0;
        for(size_t k = 0; k < processCount; k++)
        {
            waitpid(childsPids[k], &status, 0);
            //cout << childsPids[k] << ": Finished with status code: " << status << endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times[i] = elapsed.count();
        //cout << "Elapsed time: " << elapsed.count() << endl;
    }

    //cout << "result[0][0]: " <<  resultMmap[0] << endl;
    //cout << "result[size][size] : " <<  resultMmap[(size*size) - 1] << endl;

    for(int i = 0; i < 10; i++)
    {
        meanElapsedTime += times[i];
    }

    meanElapsedTime /= 10;
    printf("[%d] Mean elapsed time: %f\n", size, meanElapsedTime);

    return 0;
}