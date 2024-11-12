#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void vector_vector_multiplication(int *A, int *B, int *C, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        C[i] = A[i] * B[i];
    }
}

int main() {
    int n;

    printf("Enter the size of the vectors: ");
    scanf("%d", &n);

    int *A = (int *)malloc(n * sizeof(int));
    int *B = (int *)malloc(n * sizeof(int));
    int *C = (int *)malloc(n * sizeof(int));

    printf("Enter elements of vector A:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &A[i]);
    }

    printf("Enter elements of vector B:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &B[i]);
    }

    vector_vector_multiplication(A, B, C, n);

    printf("Resulting vector C after multiplication:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    free(A);
    free(B);
    free(C);

    return 0;
}
