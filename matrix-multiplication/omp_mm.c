#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matrix_helpers.h"

float **ParallelMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    double startTime = omp_get_wtime();
    int i, j, k;
    #pragma omp parallel for num_threads(32) private(i,j,k)
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

    double time = omp_get_wtime();
    printf("%f\n", time-startTime);
    return result;
}

int main(int argc, char **argv)
{
    if (argc != 2 ) {
        printf("Please, enter the size of the square matrix.\n");
        return 1;
    }

    int size = atoi(argv[1]);

    for(size_t i = 0; i < 10; i++)
    {
        float **matrix1 = (float **)malloc(size * sizeof(float *));
        for (int i = 0; i < size; ++i)
            matrix1[i] = (float *)malloc(size * sizeof(float));

        float **matrix2 = (float **)malloc(size * sizeof(float *));
        for (int i = 0; i < size; ++i)
            matrix2[i] = (float *)malloc(size * sizeof(float));

        FillMatrix(matrix1, size);
        FillMatrix(matrix2, size);

        srand(time(NULL));

        float **matrixResult = ParallelMM(matrix1, matrix2, size);
    }

    return 0;
}
