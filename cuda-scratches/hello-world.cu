#include <stdio.h>

__global__ void helloWorldFromGPU(void)
{
    printf("Running on GPU: threadId (%d,%d) - blockId (%d,%d) - blockDim (%d,%d)\n",
        threadIdx.x, threadIdx.y,
        blockIdx.x, blockIdx.y,
        blockDim.x, blockDim.y
    );
}

int main(void)
{
    printf("Hello World from CPU!\n");
    helloWorldFromGPU<<<5, 10>>>();
    cudaDeviceSynchronize();
    return 0;
}