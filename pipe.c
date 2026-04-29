#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX 1024

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;
    char filename[100];
    char buffer[MAX];

    // pipe1: client -> server
    // pipe2: server -> client
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    // Child process acts as CLIENT
    if (pid == 0) {
        close(pipe1[0]); // close read end of pipe1
        close(pipe2[1]); // close write end of pipe2

        printf("Client: Enter file name: ");
        scanf("%s", filename);

        // Send file name to server
        write(pipe1[1], filename, strlen(filename) + 1);

        // Read reply from server
        int n = read(pipe2[0], buffer, MAX - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("\nClient received from server:\n%s\n", buffer);
        }

        close(pipe1[1]);
        close(pipe2[0]);
    }
    // Parent process acts as SERVER
    else {
        close(pipe1[1]); // close write end of pipe1
        close(pipe2[0]); // close read end of pipe2

        char recv_filename[100];
        int fd, n;

        // Read file name from client
        read(pipe1[0], recv_filename, sizeof(recv_filename));
        printf("Server: Client requested file: %s\n", recv_filename);

        fd = open(recv_filename, O_RDONLY);

        if (fd < 0) {
            char *msg = "Error: File not found or cannot be opened.";
            write(pipe2[1], msg, strlen(msg));
        } else {
            n = read(fd, buffer, MAX - 1);
            if (n < 0) {
                char *msg = "Error: Unable to read file.";
                write(pipe2[1], msg, strlen(msg));
            } else {
                buffer[n] = '\0';
                write(pipe2[1], buffer, strlen(buffer));
            }
            close(fd);
        }

        close(pipe1[0]);
        close(pipe2[1]);
        wait(NULL);
    }

    return 0;
}