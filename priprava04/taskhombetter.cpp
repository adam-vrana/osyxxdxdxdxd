#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2){
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    const char *file_name = argv[1];
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
            dprintf(pipefd[1], "%s\n", names[idx]);
            usleep(delay_us);
        }
        close(pipefd[1]);
        exit(0);
    }

    // Potomek 2
    if (fork() == 0) {
        close(pipefd[1]);
        close(pipefd2[0]);
        close(pipefd3[0]); close(pipefd3[1]);

        FILE *stream = fdopen(pipefd[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }

        char buffer[256];
        int n = 1;
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            char output[256];
            snprintf(output, sizeof(output), "%d. %s", n++, buffer);
            dprintf(pipefd2[1], "%s\n", output);
        }

        fclose(stream);
        close(pipefd2[1]);
        exit(0);
    }

    // Potomek 3
    if (fork() == 0) {
        close(pipefd[0]); close(pipefd[1]);
        close(pipefd2[1]);
        close(pipefd3[0]);

        FILE *stream = fdopen(pipefd2[0], "r");
        if (!stream) {
            perror("fdopen");
            exit(1);
        }

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), stream)) {
            buffer[strcspn(buffer, "\n")] = 0;
            char *space = strchr(buffer, ' ');
            if (space) space++;
            int len = strlen(space);
            dprintf(pipefd3[1], "%s (%d)\n", buffer, len);
        }

        fclose(stream);
        close(pipefd3[1]);
        exit(0);
    }

    // Rodič
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

        char name[256];
        sscanf(buffer, "%*d. %s", name);

        for (int i = 0; i < count; i++) {
            if (strcmp(names[i], name) == 0) {
                printf("%s - %d\n", buffer, i + 1);
                break;
            }
        }
    }

    fclose(stream);
    close(pipefd3[0]);

    while (wait(NULL) > 0);
    return 0;
}
