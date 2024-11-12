#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void vectorDotProduct(int *A, int *B, int *C, int n) {
    __shared__ int sharedData[256];  // Adjust size based on block size
    int idx = threadIdx.x + blockIdx.x * blockDim.x;

    if (idx < n) {
        sharedData[threadIdx.x] = A[idx] * B[idx];
    } else {
        sharedData[threadIdx.x] = 0;
    }

    __syncthreads();

    // Perform reduction within the block
    int stride = blockDim.x / 2;
    while (stride > 0) {
        if (threadIdx.x < stride) {
            sharedData[threadIdx.x] += sharedData[threadIdx.x + stride];
        }
        __syncthreads();
        stride /= 2;
    }

    if (threadIdx.x == 0) {
        atomicAdd(C, sharedData[0]);
    }
}

int main() {
    int n;

    printf("Enter the size of the vectors: ");
    scanf("%d", &n);

    int *A = (int *)malloc(n * sizeof(int));
    int *B = (int *)malloc(n * sizeof(int));
    int *C = (int *)malloc(sizeof(int));

    printf("Enter elements of vector A:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &A[i]);
    }

    printf("Enter elements of vector B:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &B[i]);
    }

    *C = 0;  // Initialize the result to 0

    int *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, n * sizeof(int));
    cudaMalloc((void **)&d_B, n * sizeof(int));
    cudaMalloc((void **)&d_C, sizeof(int));

    cudaMemcpy(d_A, A, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_C, C, sizeof(int), cudaMemcpyHostToDevice);

    int blockSize = 256;  // Number of threads per block
    int gridSize = (n + blockSize - 1) / blockSize;  // Number of blocks

    vectorDotProduct<<<gridSize, blockSize>>>(d_A, d_B, d_C, n);

    cudaDeviceSynchronize();

    cudaMemcpy(C, d_C, sizeof(int), cudaMemcpyDeviceToHost);

    printf("Dot Product of vectors A and B: %d\n", *C);

    free(A);
    free(B);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
