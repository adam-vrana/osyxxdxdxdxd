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
    int number;
    int m = atoi(argv[1]);
    int bin = 0;

    for (int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-b") == 0){
            bin = 1;
        }
    }

    int counter = 0;
    int sum = 0;
    int found = 0;
    

    if(bin == 0){
        while(scanf("%d", &number) == 1){
            counter++;
            if (counter != m){
                for (int i = 0; i < sizeof(prvocisla) / sizeof(prvocisla[0]); i++)
                {
                    if (prvocisla[i] == number){
                        found = 1;
                        break;
                    }
                }

                if (found == 0){
                    fprintf(stderr, "nenalezeno prvocislo");
                    return 1;
                }
                sum += number;
            }


            if (m == counter){
                if (sum != number){
                    fprintf(stderr, "spatny soucet");
                    return 2;
                }
                counter = 0;
            }
            
        }
    } else 
    {
        while(fread(&number, sizeof(int), 1, stdin) > 0){
        counter++;
        if (counter != m){
            for (int i = 0; i < sizeof(prvocisla) / sizeof(prvocisla[0]); i++)
            {
                if (prvocisla[i] == number){
                    found = 1;
                    break;
                }
            }

            if (found == 0){
                fprintf(stderr, "nenalezeno prvocislo");
                return 1;
            }
            sum += number;
        }


        if (m == counter){
            if (sum != number){
                fprintf(stderr, "spatny soucet");
                return 2;
            }
            counter = 0;
        }
        
    }
    }
    fprintf(stdout, "vse spravne");
    return 0;
}   