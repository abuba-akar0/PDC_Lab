// ============================================================
// SCENARIO 3: Multithreaded Matrix Optimization
// TOPIC: Thread Management, pthread_join, Performance
// ============================================================
// WHAT THIS PROGRAM DOES:
//   - Adds two 4x4 matrices (A + B = C)
//   - Each thread handles ONE ROW of the matrix
//   - So 4 threads run SIMULTANEOUSLY, each adding one row
//   - Much faster than doing it row-by-row sequentially
// ============================================================

#include <stdio.h>
#include <pthread.h>

// size of matrix
#define ROWS 4
#define COLS 4

// SHARED MATRICES (global so all threads can access)
int A[ROWS][COLS] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

int B[ROWS][COLS] = {
    {1, 1, 1, 1},
    {2, 2, 2, 2},
    {3, 3, 3, 3},
    {4, 4, 4, 4}
};

int C[ROWS][COLS];

void* add_row(void* arg) {
    int row = *((int*)arg);

    printf("Thread %d: Starting addition of row %d\n", row, row);

    for (int col = 0; col < COLS; col++) {
        C[row][col] = A[row][col] + B[row][col];
    }

    printf("Thread %d: Finished row %d\n", row, row);
    return NULL;
}

void print_matrix(int matrix[ROWS][COLS], char* name) {
    printf("\nMatrix %s:\n", name);
    for (int i = 0; i < ROWS; i++) {
        printf("  [ ");
        for (int j = 0; j < COLS; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("]\n");
    }
}

int main() {
    printf("=== Scenario 3: Multithreaded Matrix Addition ===\n");

    print_matrix(A, "A");
    print_matrix(B, "B");

    printf("\n--- Creating threads (one per row) ---\n");

    //Create one thread per row
    pthread_t threads[ROWS];    // array to hold thread IDs
    int row_ids[ROWS];          // array to hold row numbers (0, 1, 2, 3)

    for (int i = 0; i < ROWS; i++) {
        row_ids[i] = i;         // assign row number to this thread

        // pthread_create(thread_ID, attributes, function, argument)
        // Each thread runs add_row() with its specific row number
        pthread_create(&threads[i], NULL, add_row, &row_ids[i]);

        printf("Main: Created thread for row %d\n", i);
    }

    printf("\n--- Waiting for all threads to finish (pthread_join) ---\n");

    // Wait for ALL threads to complete
    for (int i = 0; i < ROWS; i++) {
        pthread_join(threads[i], NULL);
        printf("Main: Thread %d has completed.\n", i);
    }

    printf("\n--- All threads finished! ---");
    print_matrix(C, "C (Result = A + B)");

    printf("\n=== Performance Benefits of Multithreading ===\n");
    printf("1. SHARED MEMORY: All threads share the same A, B, C arrays.\n");
    printf("   No copying of data needed. saves RAM and time.\n\n");
    printf("2. CPU + I/O OVERLAP: While one thread waits (e.g., reading data),\n");
    printf("   other threads keep computing. CPU never sits idle.\n");

    return 0;
}

// ============================================================
// SEQUENTIAL vs MULTITHREADED:
//   Sequential:     Row 0 → Row 1 → Row 2 → Row 3  (one after another)
//   Multithreaded:  Row 0                            (all at the SAME TIME)
//                   Row 1   ← running simultaneously
//                   Row 2
//                   Row 3
