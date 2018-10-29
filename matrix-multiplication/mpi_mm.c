#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

// Performs the multiplication of a given matrix segment
// void MMSegment(double **m1, double **m2, double* result, int from, int to, int size)
// {
//     //printf("[%d] From: %d To: %d\n", getpid(), from, to);
//     for(int i = from; i < to; i++)
//     {
//         for(int j = 0; j < size; j++)
//         {
//             double cellTotal = 0;
//             for(int k = 0; k < size; k++)
//             {
//                 cellTotal += m1[i][k] * m2[k][j];
//             }
//             result[i * size + j] = cellTotal;
//         }
//     }
// }

int main(int argc, char ** argv) {

    printf("Debug 1");

    int size = 1000;
    int numtasks, taskid, numworkers, rows;

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
        int errC;
        MPI_Abort(MPI_COMM_WORLD, errC);
        exit(1);
    }

    numworkers = numtasks - 1;

    if (taskid == MASTER)
    {
        printf("MPI ha iniciado con %d tareas.\n", numtasks);

        for (int i=0; i < size; ++i) {
            for (int j=0; j < size; ++j) {
                matrix1[i][j] = ((double)rand() / (double)(RAND_MAX)) * 9.0;
            }
        }
        for (int i=0; i < size; ++i) {
            for (int j=0; j < size; ++j) {
                matrix2[i][j] = ((double)rand() / (double)(RAND_MAX)) * 9.0;
            }
        }

        double start = MPI_Wtime();

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
      for (int i = 1; i <= numworkers; i++)
      {
          source = i;
          MPI_Recv(&offset, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
          MPI_Recv(&rowsRecv, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
          MPI_Recv(&matrixR[offset][0], rowsRecv*size, MPI_DOUBLE, source, FROM_WORKER, MPI_COMM_WORLD, &status);
      }

      double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);
    }


    if (taskid > MASTER)
    {
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix1, rows*size, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix2, size*size, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);

        for (int k=0; k<size; k++) {
            for (int i=0; i<rows; i++) {
                matrixR[i][k] = 0.0;
                for (int j=0; j<size; j++) {
                    matrixR[i][k] = matrixR[i][k] + matrix1[i][j] * matrix2[j][k];
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