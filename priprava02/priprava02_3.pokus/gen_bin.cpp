#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

void print_binary(int n) {
    printf("0b");
    int started = 0;
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        int bit = (n >> i) & 1;
        if (bit) started = 1;
        if (started) printf("%d", bit);
    }
    if (!started) printf("0");
}

void generate_bin(int M, int N) {
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        int count = (rand() % M) + 1;  // 1 až M čísel na řádek
        int sum = 0;
        printf(" ");
        for (int j = 0; j < count; j++) {
            int num = (rand() % 9990) + 10;  // čísla 10-9999
            print_binary(num);
            printf(" ");
            sum += num;
        }
        print_binary(sum);
        printf("\n");
    }
}