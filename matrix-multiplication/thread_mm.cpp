/* g++ thread_mm.cpp -pthread -std=c++11 */

#include <iostream>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include "matrix_helpers.h"

using namespace std;

void ParallelMM(float **m1, float **m2, float **result, int from, int to, int size)
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
        cout << "Please, enter the size of square matrix." << endl;
        cout << "Please, enter the number of threads." << endl;
        return 1;
    }

    //cout << "Hardware capacity: " << std::thread::hardware_concurrency() << " threads." << endl;

    int size = atoi(argv[1]);
    int threadNumber = atoi(argv[2]);
    double meanElapsedTime = 0;
    double *times = (double *)malloc(10 * sizeof(double *));

    float **matrix1 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix1[i] = (float *)malloc(size * sizeof(float));

    float **matrix2 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix2[i] = (float *)malloc(size * sizeof(float));

    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    FillMatrix(matrix1, size);
    FillMatrix(matrix2, size);

    int threadCount = threadNumber;
    int normalStep = size/threadNumber;
    int finalStep = size - (normalStep*threadNumber);
    if (finalStep != 0) threadCount++;
    //cout << "Thread count: " << threadCount << endl;

    for(size_t j = 0; j < 10; j++)
    {
        thread t[threadCount];

        int normalStep = size/threadNumber;
        int finalStep = size - (normalStep*threadNumber);
        int lastPosition = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i < threadCount; i++)
        {
            if (finalStep != 0 && i == threadNumber) {
                normalStep = finalStep;
            }
            //cout << "Launching from: " << lastPosition << " To: " << lastPosition + normalStep << endl;
            t[i] = thread(ParallelMM, matrix1, matrix2, result, lastPosition, lastPosition + normalStep, size);
            lastPosition = lastPosition + normalStep;
        }

        for(size_t i = 0; i < threadCount; i++)
        {
            t[i].join();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times[j] = elapsed.count();
    }

    for(int i = 0; i < 10; i++)
    {
        meanElapsedTime += times[i];
    }

    meanElapsedTime /= 10;
    printf("[%d] Mean elapsed time: %f\n", size, meanElapsedTime);

    return 0;
}