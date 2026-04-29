#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SHM_NAME "/daytime_shm"
#define SIZE 1024

int main() {
    int shm_fd;
    char *ptr;
    time_t t;
    struct tm *tm_info;
    char time_str[100];

    // Create shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    // Set size of shared memory
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate failed");
        exit(1);
    }

    // Map shared memory into process address space
    ptr = (char *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Get current date and time
    time(&t);
    tm_info = localtime(&t);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    // Write to shared memory
    sprintf(ptr, "Current date and time: %s", time_str);

    printf("Server: Date and time written to shared memory.\n");
    printf("Server: %s\n", ptr);

    printf("Server is waiting for client to read...\n");
    sleep(20);   // Give client time to read

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    printf("Server: Shared memory removed.\n");

    return 0;
}








___________________________________________


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/daytime_shm"
#define SIZE 1024

int main() {
    int shm_fd;
    char *ptr;

    // Open existing shared memory object
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    // Map shared memory into process address space
    ptr = (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Read from shared memory
    printf("Client: Reading from shared memory...\n");
    printf("%s\n", ptr);

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);

    return 0;
}



//gcc server.c -o server -lrt
//gcc client.c -o client -lrt
//./server
//./client