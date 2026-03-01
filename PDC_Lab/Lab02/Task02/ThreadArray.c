#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int arr[MAX];
int size;
int target;
int found_index = -1;

typedef struct {
    int start;
    int end;
} ThreadData;

void* search(void* arg)
{
    ThreadData* data = (ThreadData*) arg;

    for(int i = data->start; i < data->end; i++)
    {
        if(arr[i] == target)
        {
            found_index = i;
            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int num_threads;

    printf("Enter array size: ");
    scanf("%d", &size);

    printf("Enter array elements:\n");
    for(int i = 0; i < size; i++)
        scanf("%d", &arr[i]);

    printf("Enter value to search: ");
    scanf("%d", &target);

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    int chunk = size / num_threads;

    for(int i = 0; i < num_threads; i++)
    {
        data[i].start = i * chunk;
        data[i].end = (i == num_threads - 1) ? size : (i + 1) * chunk;

        pthread_create(&threads[i], NULL, search, &data[i]);
    }

    for(int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    if(found_index != -1)
        printf("Value found at index %d\n", found_index);
    else
        printf("Value not found\n");

    return 0;
}