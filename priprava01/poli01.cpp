#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]){

    int binary = 0;

    if (strcmp(argv[argc-1], "-b") == 0) {
        binary = 1;
    }


    if (binary == 1){
        if (argc == 1 || argc == 4 || argc > 5 || argc == 2){
            fprintf(stderr, "spatny pocet argumentu");
            return 1;
        }
    } else 
    {
        if (argc == 1 || argc == 3 || argc > 4){
            fprintf(stderr, "spatny pocet argumentu");
            return 1;
        }
    }

    srand(time(NULL));

    int n = rand();

    for (int i = 0; i < atoi(argv[1]); i++)
    {
        if (argc == 4 || argc == 5){
            n = (n + atoi(argv[2])) % (atoi(argv[3]) - atoi(argv[2]) + 1) + atoi(argv[2]);
        } 
        else {
            n = n % 10000;
        }

        if (binary == 0){
            printf("%d ", n);
        } else 
        {
            fwrite(&n, sizeof(int), 1, stdout);
        }

        n = rand();
    }
    

}