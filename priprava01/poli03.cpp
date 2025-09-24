#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int prvocisla[50] = {
     2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
     31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
     73, 79, 83, 89, 97,101,103,107,109,113,
     127,131,137,139,149,151,157,163,167,173,
     179,181,191,193,197,199,211,223,227,229
    };

int main(int argc, char* argv[]){
    int n = 0;
    int m = 0;
    int rnd = 0;
    int sum = 0;

    int bin = 0;

    for (int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-b") == 0){
            bin = 1;
        }
    }

    srand(time(NULL));
    
    if (argc = 2){
        n = atoi(argv[1]);
        m = n;
    } else 
    {
        n = atoi(argv[1]);
        m = atoi(argv[2]);
    }

    for (int i = 0; i < n; i++)
    {
        if (bin == 0){
            for (int j = 0; j < m - 1; j++)
            {
                rnd = rand() % (sizeof(prvocisla) / sizeof(prvocisla[0]));
                sum += prvocisla[rnd];
                fprintf(stdout, "%d ", prvocisla[rnd]);
            }
            fprintf(stdout,"%d\n", sum);
        } else
        {
            for (int j = 0; j < m - 1; j++)
            {
                rnd = rand() % (sizeof(prvocisla) / sizeof(prvocisla[0]));
                sum += prvocisla[rnd];
                fwrite(&prvocisla[rnd], sizeof(int), 1, stdout);
            }
        fwrite(&sum, sizeof(int), 1, stdout);
        }
        
    }
    
    return 0;
}   