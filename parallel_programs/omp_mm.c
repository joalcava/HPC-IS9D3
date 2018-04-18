#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

float **ParallelMM(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    clock_t st = clock();
    int i, j, k;
    #pragma omp parallel for schedule(static, 8) private(i,j,k)
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

    clock_t et = clock();
    float seconds = (float)(et-st)/CLOCKS_PER_SEC;
    printf("Result: %lf\n", seconds);
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

    FillMatrix(matrix1, size);
    FillMatrix(matrix2, size);
    
    float **matrixResult = ParallelMM(matrix1, matrix2, size);
    printf("Ended\n");

    return 0;
}
