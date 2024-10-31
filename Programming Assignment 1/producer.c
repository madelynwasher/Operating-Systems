//Madelyn Washer
//Operating Systems
//Programming Assignment #1
//producer.c

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

//Function for producer thread
void* producer(void* arg) {
    for (int i = 1; i <= 20; i += 2) {
        sem_wait(&data->empty);
        pthread_mutex_lock(&data->mutex);

        //Produce first item
        data->table[data->in] = i;
        printf("Produced: %d\n", data->table[data->in]);
        data->in = (data->in + 1) % TABLE_SIZE;

        //Produce second item
        if (i + 1 <= 20) {
            sem_wait(&data->empty);
            data->table[data->in] = i + 1;
            printf("Produced: %d\n", data->table[data->in]);
            data->in = (data->in + 1) % TABLE_SIZE;
        }

        pthread_mutex_unlock(&data->mutex);
        sem_post(&data->full);
        sem_post(&data->full);

        sleep(rand() % 2);
    }
    return NULL;
}

int main() {
    signal(SIGINT, handle_sigint);

    // Create shared memory for shared data
    int shm_fd = shm_open("shared_data", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct shared_data));
    data = mmap(0, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    data->in = 0;
    data->out = 0;
    sem_init(&data->empty, 1, TABLE_SIZE);
    sem_init(&data->full, 1, 0);
    pthread_mutex_init(&data->mutex, NULL);

    // Create producer thread
    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);

    // Wait for producer to finish
    pthread_join(producer_thread, NULL);

    //Clean
    cleanup();
    return 0;
}
