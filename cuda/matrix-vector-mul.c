#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void matrix_vector_multiply(int *A, int *B, int *C, int m, int n) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < m) {
        int temp = 0;
        for (int i = 0; i < n; i++) {
            temp += A[row * n + i] * B[i];
        }
        C[row] = temp;
    }
}

int main() {
    int m, n;

    printf("Enter the number of rows (m) and columns (n) of the matrix: ");
    scanf("%d %d", &m, &n);

    int *A = (int *)malloc(m * n * sizeof(int));
    int *B = (int *)malloc(n * sizeof(int));
    int *C = (int *)malloc(m * sizeof(int));

    printf("Enter elements of matrix A (%d x %d):\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i * n + j]);
        }
    }

    printf("Enter elements of vector B (%d elements):\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &B[i]);
    }

    int *d_A, *d_B, *d_C;

    cudaMalloc((void **)&d_A, m * n * sizeof(int));
    cudaMalloc((void **)&d_B, n * sizeof(int));
    cudaMalloc((void **)&d_C, m * sizeof(int));

    cudaMemcpy(d_A, A, m * n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, n * sizeof(int), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (m + blockSize - 1) / blockSize;
    matrix_vector_multiply<<<numBlocks, blockSize>>>(d_A, d_B, d_C, m, n);

    cudaDeviceSynchronize();

    cudaMemcpy(C, d_C, m * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Resulting vector C after matrix-vector multiplication:\n");
    for (int i = 0; i < m; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    free(A);
    free(B);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
