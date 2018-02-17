#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float *SumVectors(float *vec1, float *vec2, int size)
{
    float *result = malloc(sizeof(float) * size);

    for (int i=0; i < size; i++)
    {
        result[i] = vec1[i] + vec2[i];
    }
    return result;
}

void FillVector(float *vec, int size)
{
    float a = 9.0;
    for (int i=0; i < size; i++)
    {
        vec[i] = ((float)rand() / (float)(RAND_MAX)) * a;
    }
}

void SaveVectorToCsvFile(float *vector, int size, char *fileName)
{
    FILE *f = fopen(fileName, "w");

    for (int i=0; i < size; i++)
    {
        fprintf(f, "%f,", vector[i]);
    }

    long currentPos = ftell(f);
    fseek(f, currentPos - 1, SEEK_SET);
    fprintf(f, "\n");
    
    fclose(f);
}

float **MatrixMultiplication(float **m1, float **m2, int size)
{
    float **result = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        result[i] = (float *)malloc(size * sizeof(float));

    for(int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size; ++j)
        {
            float resultAux = 0;
            for(int k = 0; k < size; ++k)
            {
                resultAux += m1[i][k] * m2[k][j];
            }
            result[i][j] = resultAux;
        }
    }
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

    int size;
    printf("Enter the size: ");
    scanf("%d", &size);

    float *vec1 = malloc(sizeof(float) * size);
    float *vec2 = malloc(sizeof(float) * size);
    FillVector(vec1, size);
    FillVector(vec2, size);
    float *result = SumVectors(vec1, vec2, size);

    SaveVectorToCsvFile(vec1, size, "vector1.csv");
    SaveVectorToCsvFile(vec2, size, "vector2.csv");
    SaveVectorToCsvFile(result, size, "sumVectorsResult.csv");

    float **matrix1 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix1[i] = (float *)malloc(size * sizeof(float));

    float **matrix2 = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; ++i)
        matrix2[i] = (float *)malloc(size * sizeof(float));

    FillMatrix(matrix1, size);
    FillMatrix(matrix2, size);
    
    float **matrixResult = MatrixMultiplication(matrix1, matrix2, size);
    
    SaveMatrixToCsvFile(matrix1, size, "matrix1.csv");
    SaveMatrixToCsvFile(matrix2, size, "matrix2.csv");
    SaveMatrixToCsvFile(matrixResult, size, "multMatrixResult.csv");

    return 0;
}
