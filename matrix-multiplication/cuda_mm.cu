#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <iostream>

using namespace std;

__global__
void gpu_matrix_mult(float *d_a, float *d_b, float *d_c, int m, int n, int k)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if ((col < k) && (row < m))
    {
        float sum = 0.0;
        for (int i = 0; i < n; i++)
        {
            sum += d_a[row * n + i] * d_b[i * k + col];
        }
        d_c[row * k + col] = sum;
    }
}

void SaveMatrixToCsvFile(float *matrix, int m, int n, char *fileName)
{
    FILE *f = fopen(fileName, "w");

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            fprintf(f, "%f,", matrix[i * n + j]);
        }
        long currentPos = ftell(f);
        fseek(f, currentPos - 1, SEEK_SET);
        fprintf(f, "\n");
    }

    fclose(f);
}

int main(int argc, char** argv)
{
    /*
       Execute on CPU
       Matrix h_a = host matrix A, size: m*n
       Matrix h_b = host matrix B, size: n*k
       Matrix h_c = host matrix C, size: m*k, is result of h_a * h_b

       Execute on GPU
       Matrix d_a = device matrix A, size: m*n
       Matrix d_b = device matrix B, size: n*k
       Matrix d_c = device matrix C, size: m*k, is result of d_a * d_b
    */

    // Matrices sizes:
    int m = 1024;
    int n = 5012;
    int k = 1024;

    // Host memory allocation
    float *h_a, *h_b, *h_c;
    h_a = (float*)malloc(m * n * sizeof(float)); // Host matrix A
    h_b = (float*)malloc(n * k * sizeof(float)); // Host matrix B

    // Random value initialization of host matrix A
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            h_a[i * n + j] = rand() % 1024;
        }
    }

    // Random value initialization of host matrix B
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            h_b[i * k + j] = rand() % 1024;
        }
    }

    printf("\nMatrix initalized.");

    // Device memory allocation
    float *d_a, *d_b, *d_c;
    cudaMalloc((void **) &d_a, m * n * sizeof(float)); // Device matrix A
    cudaMalloc((void **) &d_b, n * k * sizeof(float)); // Device matrix B
    cudaMalloc((void **) &d_c, m * k * sizeof(float)); // Device matrix C (result)

    printf("\nMemory allocated in device");

    // Copy matrixes from host to device memory
    cudaMemcpy(d_a, h_a, sizeof(float)*m*n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, sizeof(float)*n*k, cudaMemcpyHostToDevice);

    printf("\nData copyed to device");

    int block_size = 32;
    dim3 dimBlock(block_size, block_size, 1);
    dim3 dimGrid(ceil((float)k / block_size), ceil((float)m / block_size), 1);

    printf("\nBlocks asigned");

    // Launch Kernel
    gpu_matrix_mult<<<dimGrid, dimBlock>>>(d_a, d_b, d_c, m, n, k);
    cudaDeviceSynchronize();

    printf("\nKernel launched.");

    // Copy result from device to host
    h_c = (float*)malloc(m * k * sizeof(float)); // Host matrix C (result)
    cudaMemcpy(h_c, d_c, sizeof(float)*m*k, cudaMemcpyDeviceToHost);

    SaveMatrixToCsvFile(h_a, m, n, "matrix_a");
    SaveMatrixToCsvFile(h_b, n, k, "matrix_b");
    SaveMatrixToCsvFile(h_c, m, k, "matrix_c");

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaFreeHost(h_a);
    cudaFreeHost(h_b);
    cudaFreeHost(h_c);
    return 0;
}

