// ============================================================
// SCENARIO 2: The Fast-Food Kitchen (Producer-Consumer)
// TOPIC: Conditional Variables with POSIX Threads
// ============================================================
// WHAT THIS PROGRAM DOES:
//   - Cooks (producers) put burgers into a chute (max 10)
//   - Cashiers (consumers) take burgers from the chute
//   - If chute is FULL  → cooks WAIT (sleep)
//   - If chute is EMPTY → cashiers WAIT (sleep)
//   - Conditional variables handle the waiting efficiently
// ============================================================

#include <stdio.h>
#include <pthread.h>   // for pthreads, mutex, cond variables
#include <unistd.h>    // for sleep()

// ---- SHARED DATA ----
int chute_count = 0;          // how many burgers are in the chute right now
#define MAX_BURGERS 10        // chute can hold maximum 10 burgers
#define TOTAL_BURGERS 20      // total burgers to produce in this simulation

// ---- SYNCHRONIZATION TOOLS ----
pthread_mutex_t mutex;        // mutex to protect chute_count (one access at a time)
pthread_cond_t  not_full;     // condition: "chute is NOT full" → cooks can add
pthread_cond_t  not_empty;    // condition: "chute is NOT empty" → cashiers can take

// ---- COOK THREAD (Producer) ----
void* cook(void* arg) {
    int cook_id = *((int*)arg);

    for (int i = 0; i < 5; i++) {   // each cook makes 5 burgers
        pthread_mutex_lock(&mutex);  // LOCK: only one person touches chute at a time
        // If chute is full, cook must WAIT
        while (chute_count == MAX_BURGERS) {
            printf("Cook %d: Chute FULL (%d burgers). Waiting...\n", cook_id, chute_count);
            // pthread_cond_wait does TWO things:
            //   1. Releases the mutex (so cashiers can take burgers)
            //   2. Puts this thread to SLEEP until someone signals not_full
            pthread_cond_wait(&not_full, &mutex);
            // When woken up: automatically re-locks the mutex and continues
        }

        // Chute has space — place a burger
        chute_count++;
        printf("Cook %d: Made a burger. Chute now has %d burgers.\n", cook_id, chute_count);

        // Signal a cashier that chute is NOT empty anymore
        // Wakes up ONE waiting cashier thread
        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&mutex); // UNLOCK: done with chute

        sleep(1); // simulate time taken to cook a burger
    }
    return NULL;
}

// ---- CASHIER THREAD (Consumer) ----
void* cashier(void* arg) {
    int cashier_id = *((int*)arg);

    for (int i = 0; i < 5; i++) {   // each cashier serves 5 burgers

        pthread_mutex_lock(&mutex);  // LOCK

        // If chute is empty, cashier must WAIT
        while (chute_count == 0) {
            printf("Cashier %d: Chute EMPTY. Waiting for burgers...\n", cashier_id);
            // Sleep and release mutex until a cook signals not_empty
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Chute has burgers — take one
        chute_count--;
        printf("Cashier %d: Served a burger. Chute now has %d burgers.\n", cashier_id, chute_count);

        // Signal a cook that chute is NOT full anymore
        // Wakes up ONE waiting cook thread
        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&mutex); // UNLOCK

        sleep(2); // simulate time taken to serve a customer
    }
    return NULL;
}

int main() {
    // Step 1: Initialize mutex and BOTH condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);    // for cooks to wait on
    pthread_cond_init(&not_empty, NULL);   // for cashiers to wait on

    printf("=== Scenario 2: Fast-Food Kitchen ===\n");
    printf("Chute capacity: %d burgers\n\n", MAX_BURGERS);

    // Step 2: Create 2 cook threads and 2 cashier threads
    pthread_t cook_threads[2], cashier_threads[2];
    int ids[2] = {1, 2};   // IDs to pass to threads

    for (int i = 0; i < 2; i++) {
        pthread_create(&cook_threads[i],    NULL, cook,    &ids[i]);
        pthread_create(&cashier_threads[i], NULL, cashier, &ids[i]);
    }

    // Step 3: Wait for all threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(cook_threads[i],    NULL);
        pthread_join(cashier_threads[i], NULL);
    }

    // Step 4: Cleanup — destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    printf("\nKitchen closed. All burgers made and served!\n");
    return 0;
}
