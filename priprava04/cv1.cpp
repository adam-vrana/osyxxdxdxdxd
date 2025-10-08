#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3){
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
    
    int pipefd[2], pipefd2[2], pipefd3[2];
    if (pipe(pipefd) == -1 || pipe(pipefd2) == -1 || pipe(pipefd3) == -1) {
        perror("pipe");
        return 1;
    }
    
    // Potomek 1
    if (fork() == 0) {
        srand(getpid());
        close(pipefd[0]);
        close(pipefd2[0]); close(pipefd2[1]);
        close(pipefd3[0]); close(pipefd3[1]);
        
        int lines_per_sec = 15;
        useconds_t delay_us = 1000000 / lines_per_sec;
        int n = (getpid() % 100) + 10;
        
        for (int i = 0; i < n; i++) {
            int idx = rand() % count;
            dprintf(pipefd[1], "%d. %s (%d)\n", i + 1, names[idx], getpid());
            usleep(delay_us);
        }
        
        close(pipefd[1]);
        exit(0);
    }
    
    // Potomek 2
    if (fork() == 0) {
        close(pipefd[1]);
        close(pipefd2[0]);
        close(pipefd3[0]);
        
        FILE *stream = fdopen(pipefd[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Extrakce jména z formátu "číslo. jméno (PID)"
            char *dot = strchr(buffer, '.');
            char *paren = strrchr(buffer, '(');
            
            if (dot && paren && paren > dot) {
                char name[256];
                int start = dot - buffer + 2;
                int end = paren - buffer - 1;
                int len = end - start;
                
                strncpy(name, buffer + start, len);
                name[len] = '\0';
                
                int name_len = strlen(name);
                
                if (name_len <= N) {
                    // Krátké jméno - druhá roura (pipefd2)
                    dprintf(pipefd2[1], "%s (%d)\n", buffer, getpid());
                } else {
                    // Dlouhé jméno - třetí roura (pipefd3)
                    dprintf(pipefd3[1], "%s (%d)\n", buffer, getpid());
                }
            }
        }
        
        fclose(stream);
        close(pipefd[0]);
        close(pipefd2[1]);
        close(pipefd3[1]);
        exit(0);
    }
    
    // Potomek 3 - krátká jména
    if (fork() == 0) {
        close(pipefd[0]); close(pipefd[1]);
        close(pipefd2[1]);
        close(pipefd3[0]); close(pipefd3[1]);
        
        FILE *stream = fdopen(pipefd2[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Extrakce čísla, jména a všech PID
            char *dot = strchr(buffer, '.');
            if (dot) {
                char number[10];
                strncpy(number, buffer, dot - buffer);
                number[dot - buffer] = '\0';
                
                char *first_paren = strchr(buffer, '(');
                char name[256];
                int name_start = dot - buffer + 2;
                int name_end = first_paren - buffer - 1;
                strncpy(name, buffer + name_start, name_end - name_start);
                name[name_end - name_start] = '\0';
                
                // Extrakce všech PID
                char pids[256] = "";
                char *current = first_paren;
                while (current) {
                    char *close_paren = strchr(current, ')');
                    if (close_paren) {
                        char pid[20];
                        strncpy(pid, current, close_paren - current + 1);
                        pid[close_paren - current + 1] = '\0';
                        strcat(pids, pid);
                        current = strchr(close_paren + 1, '(');
                    } else {
                        break;
                    }
                }
                
                printf("%s. kratke %s%s\n", number, pids, pids);
                fflush(stdout);
            }
        }
        
        fclose(stream);
        close(pipefd2[0]);
        exit(0);
    }
    
    // Potomek 4 - dlouhá jména
    if (fork() == 0) {
        close(pipefd[0]); close(pipefd[1]);
        close(pipefd2[0]); close(pipefd2[1]);
        close(pipefd3[1]);
        
        FILE *stream = fdopen(pipefd3[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }
        
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Extrakce čísla, jména a všech PID
            char *dot = strchr(buffer, '.');
            if (dot) {
                char number[10];
                strncpy(number, buffer, dot - buffer);
                number[dot - buffer] = '\0';
                
                char *first_paren = strchr(buffer, '(');
                char name[256];
                int name_start = dot - buffer + 2;
                int name_end = first_paren - buffer - 1;
                strncpy(name, buffer + name_start, name_end - name_start);
                name[name_end - name_start] = '\0';
                
                // Extrakce všech PID
                char pids[256] = "";
                char *current = first_paren;
                while (current) {
                    char *close_paren = strchr(current, ')');
                    if (close_paren) {
                        char pid[20];
                        strncpy(pid, current, close_paren - current + 1);
                        pid[close_paren - current + 1] = '\0';
                        strcat(pids, pid);
                        current = strchr(close_paren + 1, '(');
                    } else {
                        break;
                    }
                }
                
                printf("                   | %s. dlouhe %s%s\n", number, pids, pids);
                fflush(stdout);
            }
        }
        
        fclose(stream);
        close(pipefd3[0]);
        exit(0);
    }
    
    // Rodič
    close(pipefd[0]); close(pipefd[1]);
    close(pipefd2[0]); close(pipefd2[1]);
    close(pipefd3[0]); close(pipefd3[1]);
    
    while (wait(NULL) > 0);
    
    return 0;
}