#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_vector_multiplication(int **A, int *B, int *C, int m, int n) {
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        C[i] = 0;
        for (int j = 0; j < n; j++) {
            C[i] += A[i][j] * B[j];
        }
    }
}

int main() {
    int m, n;

    printf("Enter the number of rows (m) and columns (n) of the matrix: ");
    scanf("%d %d", &m, &n);

    int **A = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        A[i] = (int *)malloc(n * sizeof(int));
    }

    int *B = (int *)malloc(n * sizeof(int));
    int *C = (int *)malloc(m * sizeof(int));

    printf("Enter elements of matrix A (%d x %d):\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter elements of vector B (%d elements):\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &B[i]);
    }

    matrix_vector_multiplication(A, B, C, m, n);

    printf("Resulting vector C after matrix-vector multiplication:\n");
    for (int i = 0; i < m; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    for (int i = 0; i < m; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
