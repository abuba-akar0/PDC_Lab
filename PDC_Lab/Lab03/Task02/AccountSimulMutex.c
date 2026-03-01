#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4

int balance = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* deposit(void* arg)
{
    int amount = rand() % 200;

    pthread_mutex_lock(&mutex);
    balance += amount;
    printf("Deposited %d | Balance = %d\n", amount, balance);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* withdraw(void* arg)
{
    int amount = rand() % 200;

    pthread_mutex_lock(&mutex);

    if(balance >= amount)
    {
        balance -= amount;
        printf("Withdrew %d | Balance = %d\n", amount, balance);
    }
    else
    {
        printf("Withdrawal of %d failed | Balance = %d\n", amount, balance);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        if(i % 2 == 0)
            pthread_create(&threads[i], NULL, deposit, NULL);
        else
            pthread_create(&threads[i], NULL, withdraw, NULL);
    }

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Final Balance = %d\n", balance);

    pthread_mutex_destroy(&mutex);
    return 0;
}