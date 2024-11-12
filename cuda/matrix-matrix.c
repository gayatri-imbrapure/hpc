#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define TILE_WIDTH 16  // Define tile size for block multiplication

// Kernel function to multiply two matrices A and B, storing the result in C
__global__ void matrixMultiply(int *A, int *B, int *C, int m, int n, int p) {
    __shared__ int tileA[TILE_WIDTH][TILE_WIDTH];
    __shared__ int tileB[TILE_WIDTH][TILE_WIDTH];

    int row = blockIdx.y * TILE_WIDTH + threadIdx.y;
    int col = blockIdx.x * TILE_WIDTH + threadIdx.x;

    int Cvalue = 0;
    for (int i = 0; i < (n + TILE_WIDTH - 1) / TILE_WIDTH; i++) {
        if (row < m && i * TILE_WIDTH + threadIdx.x < n)
            tileA[threadIdx.y][threadIdx.x] = A[row * n + i * TILE_WIDTH + threadIdx.x];
        else
            tileA[threadIdx.y][threadIdx.x] = 0;

        if (col < p && i * TILE_WIDTH + threadIdx.y < n)
            tileB[threadIdx.y][threadIdx.x] = B[(i * TILE_WIDTH + threadIdx.y) * p + col];
        else
            tileB[threadIdx.y][threadIdx.x] = 0;

        __syncthreads();

        for (int j = 0; j < TILE_WIDTH; j++) {
            Cvalue += tileA[threadIdx.y][j] * tileB[j][threadIdx.x];
        }

        __syncthreads();
    }

    if (row < m && col < p)
        C[row * p + col] = Cvalue;
}

int main() {
    int m, n, p;

    printf("Enter matrix dimensions (m x n) and (n x p): ");
    scanf("%d %d %d", &m, &n, &p);

    int *A = (int *)malloc(m * n * sizeof(int));
    int *B = (int *)malloc(n * p * sizeof(int));
    int *C = (int *)malloc(m * p * sizeof(int));

    printf("Enter elements of matrix A (%d x %d):\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i * n + j]);
        }
    }

    printf("Enter elements of matrix B (%d x %d):\n", n, p);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            scanf("%d", &B[i * p + j]);
        }
    }

    int *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, m * n * sizeof(int));
    cudaMalloc((void **)&d_B, n * p * sizeof(int));
    cudaMalloc((void **)&d_C, m * p * sizeof(int));

    cudaMemcpy(d_A, A, m * n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, n * p * sizeof(int), cudaMemcpyHostToDevice);

    dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
    dim3 dimGrid((p + TILE_WIDTH - 1) / TILE_WIDTH, (m + TILE_WIDTH - 1) / TILE_WIDTH);

    matrixMultiply<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, m, n, p);

    cudaDeviceSynchronize();

    cudaMemcpy(C, d_C, m * p * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Resulting matrix C after multiplication:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            printf("%d ", C[i * p + j]);
        }
        printf("\n");
    }

    free(A);
    free(B);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
