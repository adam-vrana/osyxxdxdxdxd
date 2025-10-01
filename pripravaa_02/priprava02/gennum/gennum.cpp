#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s M N [-f]\n", argv[0]);
        return 1;
    }
    
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    
    if (M <= 0 || N <= 0) {
        fprintf(stderr, "M and N must be positive integers\n");
        return 1;
    }
    
    int use_floats = 0;
    if (argc == 4 && strcmp(argv[3], "-f") == 0) {
        use_floats = 1;
    }
    
    if (use_floats) {
        generate_floats(M, N);
    } else {
        generate_integers(M, N);
    }
    
    return 0;
}