#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_name> <N>\n", argv[0]);
        return 1;
    }
    
    const char *file_name = argv[1];
    int N = atoi(argv[2]);
    
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }
    
    char names[1000][256];
    int count = 0;
    while (fgets(names[count], sizeof(names[count]), file)) {
        names[count][strcspn(names[count], "\n")] = 0;
        count++;
    }
    fclose(file);
    
    int pipefd[2], pipefd2[2], pipefd3[2], pipefd4[2];
    if (pipe(pipefd) == -1 || pipe(pipefd2) == -1 || 
        pipe(pipefd3) == -1 || pipe(pipefd4) == -1) {
        perror("pipe");
        return 1;
    }
    
    // Potomek 1 - přijímá náhodná čísla s PID rodiče, vybírá jména
    if (fork() == 0) {
        close(pipefd[1]);
        close(pipefd2[0]);
        close(pipefd3[0]); close(pipefd3[1]);
        close(pipefd4[0]); close(pipefd4[1]);
        
        FILE *stream = fdopen(pipefd[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Formát: "číslo PID_rodiče"
            char *space = strchr(buffer, ' ');
            if (space) {
                *space = '\0';
                int idx = atoi(buffer);
                char *parent_pid = space + 1;
                
                if (idx >= 0 && idx < count) {
                    dprintf(pipefd2[1], "%s %s (%d)\n", names[idx], parent_pid, getpid());
                }
            }
        }
        
        fclose(stream);
        close(pipefd[0]);
        close(pipefd2[1]);
        exit(0);
    }
    
    // Potomek 2 - čísluje řádky
    if (fork() == 0) {
        close(pipefd[0]); close(pipefd[1]);
        close(pipefd2[1]);
        close(pipefd3[0]);
        close(pipefd4[0]); close(pipefd4[1]);
        
        FILE *stream = fdopen(pipefd2[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[512];
        int n = 1;
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            dprintf(pipefd3[1], "%d. %s (%d)\n", n++, buffer, getpid());
        }
        
        fclose(stream);
        close(pipefd2[0]);
        close(pipefd3[1]);
        exit(0);
    }
    
    // Potomek 3 - rozhoduje podle délky jména N
    if (fork() == 0) {
        close(pipefd[0]); close(pipefd[1]);
        close(pipefd2[0]); close(pipefd2[1]);
        close(pipefd3[1]);
        close(pipefd4[0]);
        
        FILE *stream = fdopen(pipefd3[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Extrakce jména (mezi "číslem." a prvním PID)
            char *dot = strchr(buffer, '.');
            if (dot) {
                char *start = dot + 2;
                char *space = strchr(start, ' ');
                if (space) {
                    int name_len = space - start;
                    
                    if (name_len < N) {
                        // Krátké jméno - vypíše s přidáním svého PID
                        printf("%s (%d)\t|\n", buffer, getpid());
                        fflush(stdout);
                    } else {
                        // Dlouhé jméno - pošle dál s přidáním svého PID
                        dprintf(pipefd4[1], "%s (%d)\n", buffer, getpid());
                    }
                }
            }
        }
        
        fclose(stream);
        close(pipefd3[0]);
        close(pipefd4[1]);
        exit(0);
    }
    
    // Potomek 4 - vypíše dlouhá jména
    if (fork() == 0) {
        close(pipefd[0]); close(pipefd[1]);
        close(pipefd2[0]); close(pipefd2[1]);
        close(pipefd3[0]); close(pipefd3[1]);
        close(pipefd4[1]);
        
        FILE *stream = fdopen(pipefd4[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            printf("\t\t\t\t\t\t|%s (%d)\n", buffer, getpid());
            fflush(stdout);
        }
        
        fclose(stream);
        close(pipefd4[0]);
        exit(0);
    }
    
    // Rodič - uzavře nepotřebné konce rour a posílá náhodná čísla s PID
    close(pipefd[0]);
    close(pipefd2[0]); close(pipefd2[1]);
    close(pipefd3[0]); close(pipefd3[1]);
    close(pipefd4[0]); close(pipefd4[1]);
    
    srand(time(NULL));
    int lines_per_sec = 15;
    useconds_t delay_us = 1000000 / lines_per_sec;
    int num_lines = (getpid() % 100) + 10;
    pid_t parent_pid = getpid();
    
    for (int i = 0; i < num_lines; i++) {
        int idx = rand() % count;
        dprintf(pipefd[1], "%d (%d)\n", idx, parent_pid);
        usleep(delay_us);
    }
    
    close(pipefd[1]);
    
    while (wait(NULL) > 0);
    
    return 0;
}