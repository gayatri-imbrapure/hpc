#include <stdio.h>
#include <omp.h>

void fibonacci(int n) {
    long long int fib[n];
    fib[0] = 0;
    fib[1] = 1;

    #pragma omp parallel for
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    printf("Fibonacci Series up to %d terms:\n", n);
    for (int i = 0; i < n; i++) {
        printf("%lld ", fib[i]);
    }
    printf("\n");
}

int main() {
    int n;

    printf("Enter the number of terms in the Fibonacci series: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    fibonacci(n);

    return 0;
}
