//Madelyn Washer
//Operating Systems
//Programming Assignment #1
//consumer.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

#define TABLE_SIZE 2 //Shared table size

//Structure for shared memory
struct shared_data {
    int table[TABLE_SIZE];
    int in;
    int out;
    sem_t empty; //Counts empty slots
    sem_t full;  //Counts filled slots
    pthread_mutex_t mutex;
};

struct shared_data* data;

void cleanup() {
    if (data) {
        sem_destroy(&data->empty);
        sem_destroy(&data->full);
        pthread_mutex_destroy(&data->mutex);
        munmap(data, sizeof(struct shared_data));
        shm_unlink("shared_data");
    }
}

//To handle signal
void handle_sigint(int sig) {
    cleanup();
    exit(0);
}

//Function for consumer thread
void* consumer(void* arg) {
    for (int i = 1; i <= 20; i += 2) {
        sem_wait(&data->full);
        pthread_mutex_lock(&data->mutex);

        // Consume first item
        int item1 = data->table[data->out];
        printf("Consumed: %d\n", item1);
        data->out = (data->out + 1) % TABLE_SIZE;

        // Consume second item
        sem_wait(&data->full);
        int item2 = data->table[data->out];
        printf("Consumed: %d\n", item2);
        data->out = (data->out + 1) % TABLE_SIZE;

        pthread_mutex_unlock(&data->mutex);
        sem_post(&data->empty);
        sem_post(&data->empty);

        sleep(rand() % 2);
    }
    return NULL;
}

int main() {
    signal(SIGINT, handle_sigint);

    // Create shared memory for shared data
    int shm_fd = shm_open("shared_data", O_RDWR, 0666);
    data = mmap(0, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    data->out = 0;

    // Create consumer thread
    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for consumer to finish
    pthread_join(consumer_thread, NULL);

    // Clean
    cleanup();
    return 0;
}
