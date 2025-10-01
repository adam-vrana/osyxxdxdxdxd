#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Použití: %s <soubor>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL){
        fprintf(stderr, "Nelze otevřít soubor %s\n", argv[1]);
        return 1;
    }

    struct stat fileStat;
    if (stat(argv[1], &fileStat) < 0){
        fprintf(stderr, "Nelze získat informace o souboru %s\n", argv[1]);
        fclose(file);
        return 1;
    }

    long size = fileStat.st_size;
    long lastSize = size;

    fseek(file, 0, SEEK_END);

    char buffer[4096];

    while (1){
        sleep(1);

        if (stat(argv[1], &fileStat) < 0){
            fprintf(stderr, "Nelze získat informace o souboru %s\n", argv[1]);
            fclose(file);
            return 1;
        }

        size = fileStat.st_size;

        if (size > lastSize){
            fprintf(stderr, "Soubor %s byl zvětšen z %ld na %ld bajtů\n", argv[1], lastSize, size);
            long toRead = size - lastSize;
            fseek(file, lastSize, SEEK_SET);
            while (toRead > 0){
                size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
                if (bytesRead <= 0){
                    break;
                }
                fwrite(buffer, 1, bytesRead, stdout);
                fflush(stdout);
                toRead -= bytesRead;
            }
            lastSize = size;
        } else if (size < lastSize){
            fprintf(stderr, "Soubor %s byl zkrácen z %ld na %ld bajtů\n", argv[1], lastSize, size);
            fseek(file, 0, SEEK_END);
            lastSize = size;
        }
    }
}