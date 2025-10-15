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
        fprintf(stderr, "Usage: %s [-o output_file] search_char\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Pipeline: sort -> nl -> tr -> grep -> tee
    // Potřebujeme 4 roury pro 5 procesů
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || 
        pipe(pipe3) == -1 || pipe(pipe4) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
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
        close(pipe4[0]); close(pipe4[1]);
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
        close(pipe4[0]); close(pipe4[1]);
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
        close(pipe4[0]); close(pipe4[1]);
        execlp("tr", "tr", "a-z", "A-Z", NULL);
        perror("execlp tr");
        exit(EXIT_FAILURE);
    }

    // Proces 4: grep (vlastní implementace)
    if (fork() == 0){
        dup2(pipe3[0], STDIN_FILENO);
        dup2(pipe4[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        close(pipe4[0]); close(pipe4[1]);

        char buf[1024];
        ssize_t n;
        std::string line;

        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            for (ssize_t i = 0; i < n; i++) {
                line += buf[i];
                if (buf[i] == '\n') {
                    bool all_found = true;
                    for (const auto& ch : search_chars) {
                        if (line.find(ch) == std::string::npos) {
                            all_found = false;
                            break;
                        }
                    }
                    if (all_found) {
                        write(STDOUT_FILENO, line.c_str(), line.length());
                    }
                    line.clear();
                }
            }
        }

        exit(0);
    }

    // Proces 5: tee [out_file_name]
    if (fork() == 0){
        dup2(pipe4[0], STDIN_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        close(pipe4[0]); close(pipe4[1]);
        
        int out_fd = -1;
        if (out_file){
            out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd == -1) {
                perror("open output file");
                exit(EXIT_FAILURE);
            }
        }
        
        char buf[1024];
        ssize_t n;
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, n);  // VŽDY na stdout
            if (out_fd != -1) {
                write(out_fd, buf, n);     // NAVÍC do souboru
            }
        }
        
        if (out_fd != -1) close(out_fd);
        exit(0);
    }

    // Zavřít všechny file descriptory v hlavním procesu
    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);
    close(pipe3[0]); close(pipe3[1]);
    close(pipe4[0]); close(pipe4[1]);

    // Čekat na všechny child procesy
    while (wait(NULL) > 0);
    
    return 0;
}