#include <stdio.h>
#include <omp.h>

int is_palindrome(int num) {
    int original_num = num;
    int reversed_num = 0;
    
    #pragma omp parallel
    {
        int thread_reversed_num = 0;
        #pragma omp for
        for (int i = 0; num > 0; num /= 10) {
            thread_reversed_num = thread_reversed_num * 10 + num % 10;
        }
        #pragma omp critical
        reversed_num = thread_reversed_num;
    }

    return original_num == reversed_num;
}

int main() {
    int num;

    printf("Enter a number: ");
    scanf("%d", &num);

    if (is_palindrome(num)) {
        printf("%d is a palindrome.\n", num);
    } else {
        printf("%d is not a palindrome.\n", num);
    }

    return 0;
}
