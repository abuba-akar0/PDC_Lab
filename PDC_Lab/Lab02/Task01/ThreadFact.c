#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int number;
    long long result;
} ThreadData;

void* factorial(void* arg)
{
    ThreadData* data = (ThreadData*) arg;
    int n = data->number;

    long long fact = 1;
    for(int i = 1; i <= n; i++)
        fact *= i;

    data->result = fact;

    pthread_exit(NULL);
}

int main()
{
    int n;

    printf("Enter number of threads: ");
    scanf("%d", &n);

    pthread_t threads[n];
    ThreadData data[n];

    for(int i = 0; i < n; i++)
    {
        printf("Enter number for thread %d: ", i+1);
        scanf("%d", &data[i].number);

        pthread_create(&threads[i], NULL, factorial, &data[i]);
    }

    for(int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    printf("\nResults:\n");
    for(int i = 0; i < n; i++)
        printf("Factorial of %d = %lld\n", data[i].number, data[i].result);

    return 0;
}