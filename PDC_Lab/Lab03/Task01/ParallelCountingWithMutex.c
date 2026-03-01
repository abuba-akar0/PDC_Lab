#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 100000

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void* arg)
{
    for(int i = 0; i < INCREMENTS; i++)
    {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment, NULL);

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Final Counter (With Mutex) = %d\n", counter);

    pthread_mutex_destroy(&mutex);
    return 0;
}