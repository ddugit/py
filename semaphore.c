#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX 20

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t mutex, empty, full;

// Producer function
void* producer(void* arg) {
    int item;
    int id = *((int*)arg);

    for (int i = 0; i < MAX; i++) {
        item = rand() % 100;

        sem_wait(&empty);   // wait if buffer is full
        sem_wait(&mutex);   // enter critical section

        buffer[in] = item;
        printf("Producer %d produced: %d at index %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);   // exit critical section
        sem_post(&full);    // increase filled slots

        sleep(1);
    }
    pthread_exit(NULL);
}

// Consumer function
void* consumer(void* arg) {
    int item;
    int id = *((int*)arg);

    for (int i = 0; i < MAX; i++) {

        sem_wait(&full);    // wait if buffer is empty
        sem_wait(&mutex);   // enter critical section

        item = buffer[out];
        printf("Consumer %d consumed: %d from index %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);   // exit critical section
        sem_post(&empty);   // increase empty slots

        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod[2], cons[2];
    int id[2] = {1, 2};

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer threads
    pthread_create(&prod[0], NULL, producer, &id[0]);
    pthread_create(&prod[1], NULL, producer, &id[1]);

    // Create consumer threads
    pthread_create(&cons[0], NULL, consumer, &id[0]);
    pthread_create(&cons[1], NULL, consumer, &id[1]);

    // Wait for threads to finish
    pthread_join(prod[0], NULL);
    pthread_join(prod[1], NULL);
    pthread_join(cons[0], NULL);
    pthread_join(cons[1], NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}