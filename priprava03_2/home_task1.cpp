#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[]){
    vector<string> files;
    vector<string> notFoundFiles;
    vector<string> args;

    for(int i = 1; i < argc; i++){
        struct stat fileStat;
        if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-t") == 0){
            args.push_back(argv[i]);
            continue;
        }
        if(stat(argv[i], &fileStat) < 0){
            notFoundFiles.push_back(argv[i]);
            continue;
        }
        files.push_back(argv[i]);
    }
    printf("Not found:\n");
    for(int i = 0; i < notFoundFiles.size(); i++){
        printf("%s ", notFoundFiles[i].c_str());
    }

    printf("\n");
    printf("Found:\n");
    for(int i = 0; i < files.size(); i++){
        struct stat fileStat;
        if(stat(files[i].c_str(), &fileStat) < 0){
            continue;
        }
        
        for(int j = 0; j < args.size(); j++){
            if(args[j] == "-s"){
                printf("size: %12ld  ", fileStat.st_size);
            }
            if(args[j] == "-r"){
                printf("permissions: %04o  ", fileStat.st_mode & 0777);
            }
            if(args[j] == "-t"){
                struct tm *timeinfo = localtime(&fileStat.st_mtime);
                char *timeStr = asctime(timeinfo);
                if(timeStr[strlen(timeStr)-1] == '\n'){
                    timeStr[strlen(timeStr)-1] = '\0'; 
                }
                printf("last_modified: %-24s  ", timeStr);
            }
        }
        printf("%s\n", files[i].c_str());
    }
}