#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void factorial_kernel(int *input, int *result, int n) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx == 0) {
        int temp_result = 1;
        for (int i = 1; i <= n; i++) {
            temp_result *= i;
        }
        *result = temp_result;
    }
}

int main() {
    int n;

    printf("Enter a number to compute its factorial: ");
    scanf("%d", &n);

    int *d_result, h_result;

    cudaMalloc((void **)&d_result, sizeof(int));

    factorial_kernel<<<1, 1>>>(NULL, d_result, n);

    cudaDeviceSynchronize();

    cudaMemcpy(&h_result, d_result, sizeof(int), cudaMemcpyDeviceToHost);

    printf("Factorial of %d is %d\n", n, h_result);

    cudaFree(d_result);

    return 0;
}
