#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctime>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int n = 0;
    int m = 0;

    if(argc != 3){
        fprintf(stderr, "./home_tasks2 <n> <m>\n");
        return 1;
    }
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    srand(time(NULL));

    while (1){
        int delay = (60 * 1000000) / n;

        for (int i = 0; i < n; i++){
            int count = rand() % m + 1;
            for(int j = 0; j < count; j++){
                int num = rand() % (9999 - 10 + 1) + 10;
                printf("%d ", num);
                
            }
            printf("\n");
            fflush(stdout);
            usleep(delay);
        }
    }
}