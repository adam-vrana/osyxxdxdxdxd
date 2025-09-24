#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

void generate_bin(int M, int N) {
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        int count = (rand() % M) + 1;  // 1 až M čísel na řádek
        int sum = 0;
        for (int j = 0; j < count; j++) {
            int num = (rand() % 9990) + 10;  // čísla 10-9999
            // print_binary(num);
            // printf(" ");
            printf("0b%b ", num);
            sum += num;
        }
        // print_binary(sum);
        printf("0b%b", sum);
        printf("\n");
    }
}