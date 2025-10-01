#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

void printTimestamp() {
    time_t now = time(NULL);
    char timeStr[64];
    struct tm *tm_info = localtime(&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(stdout, "[%s] ", timeStr);
}

struct FileInfo {
    string name;
    long lastSize;
    long currentPos;
};

int is_executable(mode_t mode) {
    return (mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH);
}

int main(int argc, char *argv[]) {

    vector<FileInfo> files;
    struct stat fileStat;

    for(int i = 1; i < argc; i++){
        // Získání informací o souboru
        if (stat(argv[i], &fileStat) < 0) {
            // Soubor neexistuje nebo nelze přistupovat
            continue;
        }
        
        // Vynecháme adresáře
        if (S_ISDIR(fileStat.st_mode)) {
            continue;
        }
        
        // Vynecháme spustitelné soubory
        if (is_executable(fileStat.st_mode)) {
            continue;
        }
        
        // Kontrola, zda lze soubor otevřít pro čtení
        if (access(argv[i], R_OK) != 0) {
            continue;
        }

        FileInfo info;
        info.name = argv[i];
        info.lastSize = 0;
        info.currentPos = 0;
        files.push_back(info);
    }
    
    for (auto& fileInfo : files) {
        const char* filename = fileInfo.name.c_str();
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("Chyba při otevírání souboru");
            continue;
        }

        
        if (stat(filename, &fileStat) < 0) {
            perror("Chyba při získávání informací o souboru");
            fclose(file);
            return 1;
        }

        fileInfo.lastSize = fileStat.st_size;
        fileInfo.currentPos = 0;

        // Přečtení existujícího obsahu
        char buffer[4096];
        long bytesRead;
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            fwrite(buffer, 1, bytesRead, stdout);
            fflush(stdout);
            fileInfo.currentPos += bytesRead;
        }

        printTimestamp();
        fprintf(stderr, "Sledování souboru '%s' (velikost: %ld bajtů)\n",
                filename, fileInfo.lastSize);
        fclose(file);
    }
    // Hlavní smyčka sledování
    while (true) {
        sleep(1);
        for(auto& fileInfo : files){
        const char* filename = fileInfo.name.c_str();
        FILE *file = fopen(filename, "r");
            
        if (stat(fileInfo.name.c_str(), &fileStat) < 0) {
                perror("Chyba při kontrole stavu souboru");
                fclose(file);
                return 1;
        }

        long newSize = fileStat.st_size;

        if (newSize != fileInfo.lastSize) {
            printTimestamp();

            if (newSize > fileInfo.lastSize) {
                // Soubor narostl
                fprintf(stderr, "Velikost souboru %s se změnila: %ld -> %ld bajtů (+%ld)\n",
                        fileInfo.name.c_str(), fileInfo.lastSize, newSize, newSize - fileInfo.lastSize);

                // Přečtení nových dat
                fseek(file, fileInfo.currentPos, SEEK_SET);
                long bytesRead;
                char buffer[4096];
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                    fwrite(buffer, 1, bytesRead, stdout);
                    fflush(stdout);
                    fileInfo.currentPos += bytesRead;
                }
            } else {
                // Soubor byl zkrácen
                fprintf(stderr, "Velikost souboru %s se změnila: %ld -> %ld bajtů (-%ld) - soubor byl zkrácen!\n",
                        fileInfo.name.c_str(), fileInfo.lastSize, newSize, fileInfo.lastSize - newSize);

                // Přesun na začátek nebo na konec zkráceného souboru
                fseek(file, 0, SEEK_END);
                fileInfo.currentPos = ftell(file);
            }

            fileInfo.lastSize = newSize;
        }
    }

    }
    return 0;
}