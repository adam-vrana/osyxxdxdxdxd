#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* agrv[]){
    
    

    FILE *f = fopen(agrv[1], "r");
    if (f == NULL){
        fprintf(stderr, "Nelze otevrit");
        return 1;
    }
    char text[1024];
    while(fgets(text, sizeof(text),f)){
        fputs(text, stdout);
    }
    fclose(f);
}