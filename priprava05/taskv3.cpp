#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    char *out_file = NULL;
    std::vector<char*> search_chars;

    // Parsování argumentů
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc){
            out_file = argv[i + 1];
            i++;
        } else {
            search_chars.push_back(argv[i]);
        }
    }

    if (search_chars.empty()) {
        fprintf(stderr, "Usage: %s [-o output_file] search_char1 [search_char2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Pipeline: sort -> nl -> tr -> grep1 -> grep2 -> ... -> grepN -> tee
    // Potřebujeme 3 + N rour (N = počet grep procesů)
    int pipe1[2], pipe2[2], pipe3[2];
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Vytvoříme další roury pro grep procesy
    std::vector<int*> grep_pipes;
    for (size_t i = 0; i < search_chars.size(); i++) {
        int* p = new int[2];
        if (pipe(p) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        grep_pipes.push_back(p);
    }

    // Proces 1: sort < names.txt
    if (fork() == 0){
        int file = open("names.txt", O_RDONLY);
        if (file == -1) {
            perror("open names.txt");
            exit(EXIT_FAILURE);
        }
        dup2(file, STDIN_FILENO);
        dup2(pipe1[1], STDOUT_FILENO);
        close(file);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        for (auto p : grep_pipes) {
            close(p[0]); close(p[1]);
        }
        execlp("sort", "sort", NULL);
        perror("execlp sort");
        exit(EXIT_FAILURE);
    }

    // Proces 2: nl -s ". "
    if (fork() == 0){
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        for (auto p : grep_pipes) {
            close(p[0]); close(p[1]);
        }
        execlp("nl", "nl", "-s", ". ", NULL);
        perror("execlp nl");
        exit(EXIT_FAILURE);
    }

    // Proces 3: tr a-z A-Z
    if (fork() == 0){
        dup2(pipe2[0], STDIN_FILENO);
        dup2(pipe3[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        for (auto p : grep_pipes) {
            close(p[0]); close(p[1]);
        }
        execlp("tr", "tr", "a-z", "A-Z", NULL);
        perror("execlp tr");
        exit(EXIT_FAILURE);
    }

    // Procesy 4 až 4+N: grep chain (logical AND)
    for (size_t i = 0; i < search_chars.size(); i++) {
        if (fork() == 0) {
            // První grep čte z pipe3, ostatní z předchozího grep_pipe
            if (i == 0) {
                dup2(pipe3[0], STDIN_FILENO);
            } else {
                dup2(grep_pipes[i-1][0], STDIN_FILENO);
            }
            
            // Poslední grep píše do grep_pipes[i], který čte tee
            dup2(grep_pipes[i][1], STDOUT_FILENO);
            
            // Zavřít všechny pipes
            close(pipe1[0]); close(pipe1[1]);
            close(pipe2[0]); close(pipe2[1]);
            close(pipe3[0]); close(pipe3[1]);
            for (auto p : grep_pipes) {
                close(p[0]); close(p[1]);
            }
            
            execlp("grep", "grep", search_chars[i], NULL);
            perror("execlp grep");
            exit(EXIT_FAILURE);
        }
    }

    // Poslední proces: tee [out_file_name]
if (fork() == 0){
    // Čte z posledního grep pipe
    dup2(grep_pipes[search_chars.size()-1][0], STDIN_FILENO); 
    
    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);
    close(pipe3[0]); close(pipe3[1]);
    
    // Zavřít VŠECHNY grep pipes
    for(auto p : grep_pipes) {
        close(p[0]); close(p[1]);
    }
    
    if (out_file) {
        // own implementation of tee
        int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        char buf[1024];
        int n;
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {  
            write(STDOUT_FILENO, buf, n);
            write(out_fd, buf, n);
        }
        close(out_fd);
        exit(0);
    } else {
        char buf[1024];
        int n;
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, n);
        }
        exit(0);
    }
}

    // Zavřít všechny file descriptory v hlavním procesu
    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);
    close(pipe3[0]); close(pipe3[1]);
    for (auto p : grep_pipes) {
        close(p[0]); close(p[1]);
    }

    // Čekat na všechny child procesy
    while (wait(NULL) > 0);
    
    // Uvolnit paměť
    for (auto p : grep_pipes) {
        delete[] p;
    }
    
    return 0;
}