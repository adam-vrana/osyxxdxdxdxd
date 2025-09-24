#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* agrv[]){
    char buffer[1000];
    char txt[1000];
    strcpy(txt, agrv[1]);
    char* a;
    while (fgets(buffer, sizeof(buffer), stdin)) {
        a = strstr(buffer, txt);
        if (a != NULL){
            fputs(buffer, stdout);
        }
    }
}