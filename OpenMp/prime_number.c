#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

void find_primes(int n) {
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));

    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }

    is_prime[0] = is_prime[1] = false; // 0 and 1 are not prime numbers

    #pragma omp parallel for
    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    printf("Prime numbers up to %d:\n", n);
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    free(is_prime);
}

int main() {
    int n;

    printf("Enter the value of n: ");
    scanf("%d", &n);

    find_primes(n);

    return 0;
}
