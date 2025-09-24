#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vahy[] = {1, 2, 4, 8, 5, 10, 9, 7 , 3, 6};

int main(int argc, char* args[]){
    int v = 0;
    int b = 0;
    for (int i = 0; i < argc; i++)
    {
        if(strcmp(args[i], "-v") == 0){
            v = 1;
        }   
        if(strcmp(args[i], "-b") == 0){
            b = 1;
        } 
    }
    

    
    long cislo_uctu = 0;
    long copy_cislo_uctu = 0;
    int platny = 0;

    if (b == 0){
        while(scanf("%010ld", &cislo_uctu) == 1){
            copy_cislo_uctu = cislo_uctu;
            int sum = 0;

            for (int i = 0; i < 10; i++ ) { 
                sum += ( cislo_uctu % 10 ) * vahy[ i ]; cislo_uctu /= 10; 
            }

            if (sum % 11 == 0){
                platny = 1;
            }

            if (v == 0){
                fprintf(stdout, "%s %010ld\n", (platny == 1) ? "platny:" : "neplatny:", copy_cislo_uctu);
            } else
            {
                if (platny == 1){
                    fprintf(stdout, "platny: %010ld\n", copy_cislo_uctu);
                }
            }

            platny = 0;
        }
    } else 
    {
        while(fread(&cislo_uctu, sizeof(cislo_uctu), 1, stdin) == 1){            
            copy_cislo_uctu = cislo_uctu;
            int sum = 0;

            for (int i = 0; i < 10; i++ ) { 
                sum += ( cislo_uctu % 10 ) * vahy[ i ]; cislo_uctu /= 10; 
            }

            if (sum % 11 == 0){
                platny = 1;
            }

            if (v == 0){
                fwrite(&copy_cislo_uctu, sizeof(copy_cislo_uctu), 1, stdout);

            } else
            {
                if (platny == 1){
                    fwrite(&copy_cislo_uctu, sizeof(copy_cislo_uctu), 1, stdout);
                }
            }

            platny = 0;
        } 
    }

}