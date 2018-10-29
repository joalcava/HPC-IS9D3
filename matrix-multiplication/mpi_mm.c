#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix_helpers.h"

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

// Performs the multiplication of a given matrix segment
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

    int size = 1000;
    int numtasks, taskid, numworkers;

    double matrix1[size][size],
           matrix2[size][size],
           matrixR[size][size];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    int averow = size/numworkers;
    int extra = size%numworkers;
    int offset = 0;

    if (numtasks < 2) {
        printf("Se necesitan almenos 2 'MPI tasks'");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }

    numworkers = numtaks - 1;

    if (taskid == MASTER)
    {
        printf("MPI ha iniciado con %d tareas.\n", numtasks);

        FillMatrix(matrix1, size);
        FillMatrix(matrix2, size);

        // TODO: Mesure time
        int dest = 1;
        for (dest = 1; dest <= numworkers; dest++) {
            int rowsToSend = (dest <= extra) ? averow + 1 : averow;
            printf("Enviando %d filas a tarea %d - offset=%d\n", rowsToSend, dest, offset);
            MPI_Send(&offset, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&rowsToSend, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&matrix1[offset][0], rowsToSend*size, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&matrix2, size*size, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            offset = offset + rows;
        }

      int rowsRecv, source;
      for (i = 1; i <= numworkers; i++)
      {
          source = i;
          MPI_Recv(&offset, 1, MPI_INT, source, FORM_WORKER, MPI_COMM_WORLD, &status);
          MPI_Recv(&rowsRecv, 1, MPI_INT, source, FORM_WORKER, MPI_COMM_WORLD, &status);
          MPI_Recv(&matrixR[offset][0], rowsRecv*size, MPI_DOUBLE, source, FORM_WORKER, MPI_COMM_WORLD, &status);
      }

      // TODO: Measure time
      // double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);
    }


    if (taskid > MASTER)
    {
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix1, rows*size, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix2, size*size, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);

        for (k=0; k<NCB; k++) {
            for (i=0; i<rows; i++) {
                c[i][k] = 0.0;
                for (j=0; j<NCA; j++) {
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
                }
            }
        }

        MPI_Send(&offset, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&matrixR, rows*size, MPI_DOUBLE, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }

    // SaveMatrixToCsvFile(matrix1, size, "m1.csv");
    // SaveMatrixToCsvFile(matrix2, size, "m2.csv");

    MPI_Finalize();
    return 0;
}