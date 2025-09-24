#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    int n = 0;
    int sum = 0;
    int bin = 0;

    if(strcmp(argv[argc-1], "-b") == 0){
        bin = 1;
    }

    if (!bin){
        while(scanf("%d", &n) == 1){
        sum += n;
        
        }
        printf("soucet: %d\n", sum);
    } else
    {
        while(fread(&n, sizeof(int), 1, stdin)){
            sum += n;
        }
        fwrite(&sum, sizeof(int), 1, stdout);
    }

    
}   