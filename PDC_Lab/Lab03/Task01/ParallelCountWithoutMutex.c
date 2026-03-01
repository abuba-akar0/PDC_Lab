#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 100000

int counter = 0;

void* increment(void* arg)
{
    for(int i = 0; i < INCREMENTS; i++)
        counter++;   // No protection

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment, NULL);

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Final Counter (No Mutex) = %d\n", counter);
    return 0;
}