#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void serial_search(int argc, char *argv[]) {
    char search_chars[100];
    int num_chars = 0;
    
    for (int i = 1; i < argc; i++) {
        search_chars[num_chars++] = argv[i][0];
    }


    for (int i = 0; i < num_chars; i++) {
        int pipe1[2], pipe2[2];
        
        if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
            perror("pipe");
        }

        if (fork() == 0) {
            close(pipe1[0]);
            dup2(pipe1[1], STDOUT_FILENO);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);
            
            char pattern[3];
            pattern[0] = '^';
            pattern[1] = search_chars[i];
            pattern[2] = '\0';
            
            execlp("grep", "grep", pattern, "names.txt", NULL);
            perror("execlp grep");
        }

        if (fork() == 0) {
            close(pipe1[1]);
            dup2(pipe1[0], STDIN_FILENO);
            close(pipe1[0]);
            
            dup2(pipe2[1], STDOUT_FILENO);
            close(pipe2[0]);
            close(pipe2[1]);
            
            execlp("wc", "wc", "-l", NULL);
            perror("execlp wc");
            exit(EXIT_FAILURE);
        }

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        char buf[256];
        int n = read(pipe2[0], buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("Znak %c: %s", search_chars[i], buf);
        }
        close(pipe2[0]);
        
        wait(NULL);
        wait(NULL);
    }
}

void parallel_search() {
    char search_chars[26];
    int num_chars = 0;
    
    for (char c = 'A'; c <= 'Z'; c++) {
        search_chars[num_chars++] = c;
    }

    int (*pipe1)[2] = (int (*)[2])malloc(num_chars * sizeof(int[2]));
    int (*pipe2)[2] = (int (*)[2])malloc(num_chars * sizeof(int[2]));
    
    if (!pipe1 || !pipe2) {
        perror("malloc");
    }

    for (int i = 0; i < num_chars; i++) {
        if (pipe(pipe1[i]) == -1 || pipe(pipe2[i]) == -1) {
            perror("pipe");
        }
    }

    for (int i = 0; i < num_chars; i++) {
        if (fork() == 0) {
            for (int j = 0; j < num_chars; j++) {
                if (j != i) {
                    close(pipe1[j][0]);
                    close(pipe1[j][1]);
                    close(pipe2[j][0]);
                    close(pipe2[j][1]);
                }
            }
            
            close(pipe1[i][0]);
            close(pipe2[i][0]);
            close(pipe2[i][1]);
            
            dup2(pipe1[i][1], STDOUT_FILENO);
            close(pipe1[i][1]);
            
            char pattern[3];
            pattern[0] = '^';
            pattern[1] = search_chars[i];
            pattern[2] = '\0';
            
            execlp("grep", "grep", pattern, "names.txt", NULL);
            perror("execlp grep");
        }
    }

    for (int i = 0; i < num_chars; i++) {
        if (fork() == 0) {
            for (int j = 0; j < num_chars; j++) {
                if (j != i) {
                    close(pipe1[j][0]);
                    close(pipe1[j][1]);
                    close(pipe2[j][0]);
                    close(pipe2[j][1]);
                }
            }
            
            close(pipe1[i][1]);
            close(pipe2[i][0]);
            
            dup2(pipe1[i][0], STDIN_FILENO);
            close(pipe1[i][0]);
            
            dup2(pipe2[i][1], STDOUT_FILENO);
            close(pipe2[i][1]);
            
            execlp("wc", "wc", "-l", NULL);
            perror("execlp wc");
        }
    }

    for (int i = 0; i < num_chars; i++) {
        close(pipe1[i][0]);
        close(pipe1[i][1]);
        close(pipe2[i][1]);
    }

    for (int i = 0; i < num_chars; i++) {
        char buf[256];
        int n = read(pipe2[i][0], buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("Znak %c: %s", search_chars[i], buf);
        }
        close(pipe2[i][0]);
    }

    for (int i = 0; i < num_chars * 2; i++) {
        wait(NULL);
    }

    free(pipe1);
    free(pipe2);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s char [char2 ...] | AZ\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2 && strcmp(argv[1], "AZ") == 0) {
        parallel_search();
    } else {
        serial_search(argc, argv);
    }

    return 0;
}