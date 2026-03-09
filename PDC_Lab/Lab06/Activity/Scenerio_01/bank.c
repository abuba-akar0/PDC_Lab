// ============================================================
// SCENARIO 1: The Shared Corporate Bank Account
// TOPIC: Race Condition → Fixed with POSIX Mutex
// ============================================================
// WHAT THIS PROGRAM DOES:
//   - 5 threads each deposit $1000 into a shared bank account
//   - WITHOUT mutex: the final balance is WRONG (race condition)
//   - WITH mutex: the final balance is CORRECT ($5000)
// ============================================================

#include <stdio.h>
#include <pthread.h>   // for pthreads (threads, mutex)

int total_balance = 0;
pthread_mutex_t lock;            // the mutex "lock" — like a key to a room

// ---- THREAD FUNCTION: each thread runs this ----
void* deposit(void* arg) {
    int amount = *((int*)arg);   // cast the argument back to int pointer, get value

    // ------ CRITICAL SECTION START ------
    // Only ONE thread can be inside here at a time
    pthread_mutex_lock(&lock);   // LOCK: grab the key, others must wait outside

        int temp = total_balance;     // Step 1: Read current balance
        temp = temp + amount;         // Step 2: Add deposit
        total_balance = temp;         // Step 3: Write back new balance

    pthread_mutex_unlock(&lock); // UNLOCK: return the key, next thread can enter
    // ------ CRITICAL SECTION END ------

    printf("Thread deposited $%d | New Balance: $%d\n", amount, total_balance);
    return NULL;
}

int main()
{
    // Step 1: Initialize the mutex BEFORE creating threads
    pthread_mutex_init(&lock, NULL);   // NULL = default settings

    // Step 2: Create 5 threads
    pthread_t threads[5];              // array to hold 5 thread IDs
    int deposit_amount = 1000;         // each thread deposits $1000

    printf("=== Scenario 1: Bank Account with Mutex ===\n");
    printf("Starting Balance: $%d\n\n", total_balance);

    for (int i = 0; i < 5; i++) {
        // pthread_create(thread_id, attributes, function, argument)
        pthread_create(&threads[i], NULL, deposit, &deposit_amount);
    }

    // Step 3: Wait for ALL threads to finish before continuing
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);  // main waits here until thread[i] is done
    }

    // Step 4: Destroy the mutex when done (cleanup)
    pthread_mutex_destroy(&lock);

    printf("\nFinal Balance: $%d\n", total_balance);
    printf("Expected:      $5000\n");

    if (total_balance == 5000)
        printf("Result: CORRECT! Mutex prevented race condition.\n");
    else
        printf("Result: WRONG! Race condition occurred.\n");

    return 0;
}