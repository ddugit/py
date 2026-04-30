#include <stdio.h>
#include <fcntl.h>   // For open() and O_flags
#include <unistd.h>  // For read(), write(), close()

int main() {
    int fd_src, fd_dest;
    char buffer[1024];
    int bytes_read;

    // 1. OPEN Source (Read Only)
    fd_src = open("source.txt", O_RDONLY);
    if (fd_src < 0) { 
        perror("Error opening source"); 
        return 1; 
    }

    // 2. OPEN/CREATE Destination (Write Only, Create if missing, Clear if exists)
    // 0644 sets file permissions (read/write for owner, read for others)
    fd_dest = open("destination.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest < 0) {
        perror("Error opening destination");
        close(fd_src);
        return 1;
    }

    // 3. READ and WRITE in a loop
    while ((bytes_read = read(fd_src, buffer, sizeof(buffer))) > 0) {
        write(fd_dest, buffer, bytes_read);
    }

    // 4. CLOSE
    close(fd_src);
    close(fd_dest);

    printf("File copy completed using system calls.\n");
    return 0;
}
