#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void* producer(void* arg)
{
    int id = *(int*)arg;

    while(1)
    {
        int item = rand() % 100;
        sleep(1);

        pthread_mutex_lock(&mutex);

        while(count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        buffer[count++] = item;
        printf("Producer %d produced %d\n", id, item);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg)
{
    int id = *(int*)arg;

    while(1)
    {
        sleep(2);

        pthread_mutex_lock(&mutex);

        while(count == 0)
            pthread_cond_wait(&not_empty, &mutex);

        int item = buffer[--count];
        printf("Consumer %d consumed %d\n", id, item);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    int p_ids[NUM_PRODUCERS];
    int c_ids[NUM_CONSUMERS];

    for(int i = 0; i < NUM_PRODUCERS; i++)
    {
        p_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &p_ids[i]);
    }

    for(int i = 0; i < NUM_CONSUMERS; i++)
    {
        c_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &c_ids[i]);
    }

    for(int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    for(int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    return 0;
}