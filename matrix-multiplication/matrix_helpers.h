#include <stdio.h>
#include <stdlib.h>

void SaveMatrixToCsvFile(float **matrix, int size, char const *fileName)
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