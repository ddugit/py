#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFF 5
#define ITEM 10

int buffer[BUFF];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex; // Lock declared

void* producer(void* arg) {
    for (int i = 1; i <= ITEM; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);
            if (count < BUFF) {
                buffer[in] = i;
                printf("Produced: %d\n", i);
                in = (in + 1) % BUFF;
                count++;
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
    return NULL;
}

void* consumer(void* args) {
    for (int i = 1; i <= ITEM; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);
            if (count > 0) {
                int item = buffer[out];
                printf("Consumed: %d\n", item);
                out = (out + 1) % BUFF;
                count--;
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
            sleep(2);
        }
    }
    return NULL;
}

int main() {
    pthread_t p, c;
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
