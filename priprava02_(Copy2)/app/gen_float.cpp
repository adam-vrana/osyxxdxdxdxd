#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

void generate_floats(int M, int N)
{
    srand(time(NULL));
    
    for (int i = 0; i < N; i++) {
        int count = (rand() % M) + 1;  // 1 to M numbers per line
        float sum = 0;
        
        for (int j = 0; j < count; j++) {
            float num = (rand() % 9999) + 10;  // 2-4 digit numbers (10-9999)
            printf("%.2f ", num);
            sum += num;
        }
        printf("%.2f\n", sum);
    }
}