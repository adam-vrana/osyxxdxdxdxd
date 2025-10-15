#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    int fd2[2];
    char *name = argv[1];


    if (pipe(fd) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
// sort < names.txt | nl -s ". " | tr a-z A-Z
    if (fork() == 0){
        close(fd2[0]);
        close(fd2[1]);
        close(fd[0]);

        int file = open(name, O_RDONLY);
        // 0 stdin 1 stdout 2 stderr
        dup2(file, 0);
        dup2(fd[1], 1);
        
        close(fd[1]);
        close(file);
        execlp("sort", "sort", NULL);
    }
    // if (fork() == 0){
    //     close(fd[1]);
    //     close(fd2[0]);
        

    //     dup2(fd[0], 0);
    //     dup2(fd2[1], 1);
    //     close(fd2[1]);
    //     close(fd[0]);
        
    //     execlp("nl", "nl", "-s", ". ", NULL);
    // }

    if (fork() == 0){
        close(fd[1]);
        close(fd2[0]);
        
        FILE *stream = fdopen(fd[0], "r");
        char buf[1024];

        int line_number = 1;
        while (fgets(buf, sizeof(buf), stream) != NULL) {
            dprintf(fd2[1], "%d. %s", line_number++, buf);
        }

        close(fd2[1]);
        close(fd[0]);
        fclose(stream);
        exit(0);
        

    }

    if (fork() == 0){
        close(fd[0]);
        close(fd[1]);
        close(fd2[1]);

        dup2(fd2[0], 0);
        close(fd2[0]);
        int file = open("out2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (argc == 3){
            int file2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(file2, 1);
            close(file2);
        } else {
            dup2(file, 1);
        }
        close(file);
        execlp("tr", "tr", "a-z", "A-Z", NULL);
    }

    // Zavřít file descriptory v hlavním procesu!
    close(fd[0]);
    close(fd[1]);
    close(fd2[0]);
    close(fd2[1]);

    while (wait(NULL) > 0);
    return 0;
}