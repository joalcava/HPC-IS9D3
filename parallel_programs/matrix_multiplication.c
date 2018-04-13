#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

float **SerialMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    clock_t st = clock();
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
    clock_t et = clock();
    float seconds = (float)(et-st)/CLOCKS_PER_SEC;
    printf("Result: %lf\n", seconds);
    return result;
}

float **ParallelMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));
    
    clock_t st = clock();
    #pragma omp parallel for schedule(dynamic, 50) collapse(2) private(i,j,k) shared(a,b,c)
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
    clock_t et = clock();
    float seconds = (float)(et-st)/CLOCKS_PER_SEC;
    printf("Result: %lf\n", seconds);
    return result;
}

float **_ParallelMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    clock_t st = clock();
    #pragma omp parallel for schedule(dynamic, 50) collapse(2) private(i,j,k) shared(a,b,c)
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
    clock_t et = clock();
    float seconds = (float)(et-st)/CLOCKS_PER_SEC;
    printf("Elapsed time: %lf\n", seconds);
    return result;
}

void FillMatrix(float **vec, int size)
{
    float a = 9.0;
    for (int i=0; i < size; ++i)
    {
        for (int j=0; j < size; ++j)
        {
            vec[i][j] = ((float)rand() / (float)(RAND_MAX)) * a;
        }
    }
}

void SaveMatrixToCsvFile(float **matrix, int size, char *fileName)
{
    FILE *f = fopen(fileName, "w");

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            fprintf(f, "%f,", matrix[i][j]);
        }
        long currentPos = ftell(f);
        fseek(f, currentPos - 1, SEEK_SET);
        fprintf(f, "\n");
    }

    fclose(f);
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

    printf("Filling matrices ...\n");
    FillMatrix(matrix1, size);
    FillMatrix(matrix2, size);
    
    printf("Performing serial multiplication\n");
    float **matrixResult1 = SerialMM(matrix1, matrix2, size);
    printf("Ended\n");

    printf("Performing parallel multiplication\n");
    float **matrixResult2 = ParallelMM(matrix1, matrix2, size);
    printf("Ended\n");

    printf("Performing second parallel multiplication\n");
    float **matrixResult3 = _ParallelMM(matrix1, matrix2, size);
    printf("Ended\n");
    // SaveMatrixToCsvFile(matrix1, size, "matrix1.csv");
    // SaveMatrixToCsvFile(matrix2, size, "matrix2.csv");
    // SaveMatrixToCsvFile(matrixResult, size, "multMatrixResult.csv");

    return 0;
}
