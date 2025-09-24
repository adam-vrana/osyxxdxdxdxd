#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* agrv[]){
    char buffer[2024];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        char text[1000];
        for (int i = 0; i < atoi(agrv[1]); i++)
        {
            text[i] = buffer[i];
        }
        
        fputs(text, stdout);
    }
}