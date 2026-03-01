#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_VEHICLES 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t green_light = PTHREAD_COND_INITIALIZER;

int light = 0; // 0 = RED, 1 = GREEN

void* traffic_light(void* arg)
{
    while(1)
    {
        sleep(5);

        pthread_mutex_lock(&mutex);
        light = 1;
        printf("\nTraffic Light: GREEN\n");
        pthread_cond_broadcast(&green_light);
        pthread_mutex_unlock(&mutex);

        sleep(5);

        pthread_mutex_lock(&mutex);
        light = 0;
        printf("Traffic Light: RED\n");
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* vehicle(void* arg)
{
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);

    while(light == 0)
    {
        printf("Vehicle %d waiting at RED light\n", id);
        pthread_cond_wait(&green_light, &mutex);
    }

    printf("Vehicle %d crossing intersection\n", id);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t light_thread;
    pthread_t vehicles[NUM_VEHICLES];
    int ids[NUM_VEHICLES];

    pthread_create(&light_thread, NULL, traffic_light, NULL);

    sleep(1);

    for(int i = 0; i < NUM_VEHICLES; i++)
    {
        ids[i] = i;
        pthread_create(&vehicles[i], NULL, vehicle, &ids[i]);
        sleep(1);
    }

    pthread_join(light_thread, NULL);

    for(int i = 0; i < NUM_VEHICLES; i++)
        pthread_join(vehicles[i], NULL);

    return 0;
}