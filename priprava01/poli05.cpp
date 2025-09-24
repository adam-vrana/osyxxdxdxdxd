#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* args[]){
    long n = 0;
    int m = 1000;
    int b = 0;
    int argi = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(args[i], "-b") == 0) {
            b = 1;
        } else {
            if (argi == 1) {
                n = atol(args[i]);
                argi++;
            }
            else if (argi == 2) {
                m = atoi(args[i]);
                argi++;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (b == 1)
        {
            long current = n + i;
            fwrite(&current, sizeof(current), 1, stdout);
        } else 
        {
            fprintf(stdout, "%010ld ", n + i);
        }
        
    }
    
}