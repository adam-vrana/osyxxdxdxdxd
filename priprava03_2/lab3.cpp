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

typedef struct {
    char name[256];
    struct stat st;
    long lastSize;
} FileInfo;

int compare_size(const void *a, const void *b){
    FileInfo *fa = (FileInfo *)a;
    FileInfo *fb = (FileInfo *)b;

    if(fa->st.st_size < fb->st.st_size) return -1;
    if(fa->st.st_size > fb->st.st_size) return 1;
    return 0;
}

void print_mode(FileInfo fileInfo){
    char str[11];

    mode_t mode = fileInfo.st.st_mode;

    if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else str[0] = '-';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';

    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';

    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';

    printf("%s", str);
}

void first_lines(const char *filename, int n){
    FILE *file = fopen(filename, "r");
    if(file == NULL) return;

    char line[1024];
    int count = 0;

    while(count < n && fgets(line, sizeof(line), file) != NULL){
        printf("%s", line);
        count++;
    }

    fclose(file);
}

void last_lines(const char *filename, int n){
    FILE *file = fopen(filename, "r");
    if(file == NULL) return;

    fseek(file, 0, SEEK_END);
    
    struct stat st;
    stat(filename, &st);
    long fileSize = st.st_size;
    
    if (fileSize == 0){
        fclose(file);
        return;
    }

    long pos = fileSize - 1;
    int lineCount = 0;

    while(pos >= 0 && lineCount < n){
        fseek(file, pos, SEEK_SET);
        char c = fgetc(file);
        if(c == '\n' && pos != fileSize - 1){
            lineCount++;
        }
        pos--;
    }

    long start_pos = 0;
    if(lineCount == n){
        start_pos = pos + 2;
    }

    fseek(file, start_pos, SEEK_SET);

    char line[1024];
    while(fgets(line, sizeof(line), file) != NULL){
        printf("%s", line);
    }

    fclose(file);
}

void print_new_lines(const char *filename, long start_pos){
    FILE *file = fopen(filename, "r");
    if(file == NULL) return;

    fseek(file, start_pos, SEEK_SET);

    char line[1024];
    while(fgets(line, sizeof(line), file) != NULL){
        printf("%s", line);
    }

    fclose(file);
}

int main(int argc, char *argv[]){
    vector<string> files;
    vector<string> notFoundFiles;

    for(int i = 1; i < argc; i++){
        struct stat fileStat;
        if(stat(argv[i], &fileStat) < 0){
            notFoundFiles.push_back(argv[i]);
            continue;
        }
        if(S_ISDIR(fileStat.st_mode)){
            continue;
        }
        if(S_IXUSR & fileStat.st_mode || S_IXGRP & fileStat.st_mode || S_IXOTH & fileStat.st_mode){
            continue;
        }
        if(access(argv[i], R_OK) != 0){
            continue;
        }
        files.push_back(argv[i]);
    }

    FileInfo *fileInfos = (FileInfo *)malloc(files.size() * sizeof(FileInfo));
    for(int i = 0; i < files.size(); i++){
        struct stat fileStat;
        strncpy(fileInfos[i].name, files[i].c_str(), sizeof(fileInfos[i].name) - 1);
        fileInfos[i].name[sizeof(fileInfos[i].name) - 1] = '\0';
        fileInfos[i].st = fileStat;
    }

    qsort(fileInfos, files.size(), sizeof(FileInfo), compare_size);

    printf("Not found:\n");
    for(int i = 0; i < notFoundFiles.size(); i++){
        printf("%s ", notFoundFiles[i].c_str());
    }

    printf("\n");
    printf("Found:\n");
    for(int i = 0; i < files.size(); i++){
        //modes
        print_mode(fileInfos[i]);
        printf("\t");

        //size
        printf("%10ld", fileInfos[i].st.st_size);
        printf("\t");

        //time
        struct tm *timeinfo = localtime(&fileInfos[i].st.st_mtime);
        char *timeStr = asctime(timeinfo);
        if(timeStr[strlen(timeStr)-1] == '\n'){
            timeStr[strlen(timeStr)-1] = '\0';
        }
        printf("%-24s", timeStr);
        printf("\t");

        //name
        printf("%s", fileInfos[i].name);
        printf("\n");

        printf("-----------------\n");
        first_lines(fileInfos[i].name, 5);  
        printf("-----------------\n");
        last_lines(fileInfos[i].name, 5);
        printf("\n-----------------\n");

        
    }

    printf("\n");
    printf("Monitoring files:\n");

    for (int i = 0; i < files.size(); i++){
        struct stat fileStat;
        stat(fileInfos[i].name, &fileStat);
        fileInfos[i].lastSize = fileStat.st_size;
    }

    while(1){
        sleep(1);
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char *timeStr = asctime(tm_info);

        if(timeStr[strlen(timeStr)-1] == '\n'){
            timeStr[strlen(timeStr)-1] = '\0';
        }

        for(int i = 0; i < files.size(); i++){
            struct stat fileStat;
            stat(fileInfos[i].name, &fileStat);
            if(fileStat.st_size != fileInfos[i].lastSize){
                if(fileStat.st_size > fileInfos[i].lastSize){
                    printf("%s: File %s increased from %ld to %ld bytes (+%ld)\n", timeStr, fileInfos[i].name, fileInfos[i].lastSize, fileStat.st_size, fileStat.st_size - fileInfos[i].lastSize);
                    print_new_lines(fileInfos[i].name, fileInfos[i].lastSize);
                } else {
                    printf("%s: File %s decreased from %ld to %ld bytes (-%ld)\n", timeStr, fileInfos[i].name, fileInfos[i].lastSize, fileStat.st_size, fileInfos[i].lastSize - fileStat.st_size);
                }
                fileInfos[i].lastSize = fileStat.st_size;
            }
        }
    }
}