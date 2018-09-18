#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matrix_helpers.h"

float **SerialMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

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
    return result;
}


int main()
{
    srand(time(NULL));

    int size = 1000;

    float **matrix1 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix1[i] = (float *)malloc(size * sizeof(float));

    float **matrix2 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix2[i] = (float *)malloc(size * sizeof(float));

    FillMatrix(matrix1, size);
    FillMatrix(matrix2, size);

    float **matrixResult = SerialMM(matrix1, matrix2, size);
    printf("Ended\n");

    return 0;
}
