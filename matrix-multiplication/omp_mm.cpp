#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <chrono>
#include "matrix_helpers.h"

using namespace std;

double ParallelMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    int i, j, k;
    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for num_threads(10) private(i,j,k)
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            for(k = 0; k < size; k++)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main(int argc, char **argv)
{
    if (argc != 2 ) {
        printf("Please, enter the size of the square matrix.\n");
        return 1;
    }

    int size = atoi(argv[1]);
    double meanElapsedTime = 0;
    double *times   = (double *)malloc(10 * sizeof(double *));
    float **matrix1 = (float **)malloc(size * sizeof(float *));
    float **matrix2 = (float **)malloc(size * sizeof(float *));

    for (int i = 0; i < size; ++i)
        matrix1[i] = (float *)malloc(size * sizeof(float));
    FillMatrix(matrix1, size);

    for (int i = 0; i < size; ++i)
        matrix2[i] = (float *)malloc(size * sizeof(float));
    FillMatrix(matrix2, size);

    for(size_t i = 0; i < 10; i++)
    {
        times[i] = ParallelMM(matrix1, matrix2, size);
    }

    for(int i = 0; i < 10; i++)
    {
        meanElapsedTime += times[i];
    }

    meanElapsedTime /= 10;
    printf("[%d] Mean elapsed time: %f\n", size, meanElapsedTime);

    return 0;
}
