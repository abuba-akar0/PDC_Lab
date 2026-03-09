#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define SIZE 10
#define THREADS 2

int arr[SIZE] = {2, 3, 4, 5, 10, 13, 17, 20, 23, 25};

int isPrime(int n)
{
    if (n < 2)
        return 0;

    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

void* checkPrime(void* arg)
{
    int thread_id = *(int*)arg;

    int chunk = SIZE / THREADS;
    int start = thread_id * chunk;
    int end = (thread_id == THREADS - 1) ? SIZE : start + chunk;

    for (int i = start; i < end; i++)
    {
        if (isPrime(arr[i]))
            printf("Thread %d: %d is Prime\n", thread_id, arr[i]);
        else
            printf("Thread %d: %d is Not Prime\n", thread_id, arr[i]);
    }

    return NULL;
}

int main()
{
    pthread_t threads[THREADS];
    int thread_ids[THREADS];

    for (int i = 0; i < THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, checkPrime, &thread_ids[i]);
    }

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    return 0;
}