#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* agrv[]){
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        char text[1000];
        for (int i = 0; i < strlen(buffer); i++)
        {
            if(strcmp(agrv[1], "-i") == 0){
                text[i] = tolower(buffer[i]);
            }
            if(strcmp(agrv[1], "-u") == 0){
                text[i] = toupper(buffer[i]);
            }
            if(strcmp(agrv[1], "-x") == 0){
                if (buffer[i] >= 'a' && buffer[i] <= 'z'){
                    text[i] = toupper(buffer[i]);
                }
                if (buffer[i] >= 'A' && buffer[i] <= 'Z'){
                    text[i] = tolower(buffer[i]);
                }
            }
        }
        
        fputs(text, stdout);
    }
}