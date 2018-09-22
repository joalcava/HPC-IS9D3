#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "matrix_helpers.h"
#include <iostream>
#include <chrono>

using namespace std;

double SerialMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            for(int k = 0; k < size; k++)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}


int main(int argc, char ** argv)
{

    if (argc != 2) {
        cout << "Please, enter the size of the square matrix." << endl;
    }

    int size = atoi(argv[1]);
    double meanElapsedTime = 0;
    double *times = (double *)malloc(10 * sizeof(double *));
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
        times[i] = SerialMM(matrix1, matrix2, size);
    }

    for(int i = 0; i < 10; i++)
    {
        meanElapsedTime += times[i];
    }

    meanElapsedTime /= 10;
    printf("[%d] Mean elapsed time: %f\n", size, meanElapsedTime);

    return 0;
}
