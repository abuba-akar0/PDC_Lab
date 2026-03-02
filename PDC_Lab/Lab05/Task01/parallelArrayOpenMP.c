#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE 100000000

int main()
{
    static int arr[SIZE];
    long long sum_seq = 0;
    long long sum_par = 0;

    // Initialize array
    for(int i = 0; i < SIZE; i++)
        arr[i] = 1;

    // -------------------------
    // Sequential Version
    // -------------------------
    double start_seq = omp_get_wtime();

    for(int i = 0; i < SIZE; i++)
        sum_seq += arr[i];

    double time_seq = omp_get_wtime() - start_seq;

    // -------------------------
    // Parallel Version
    // -------------------------
    double start_par = omp_get_wtime();

#pragma omp parallel for reduction(+:sum_par)
    for(int i = 0; i < SIZE; i++)
        sum_par += arr[i];

    double time_par = omp_get_wtime() - start_par;

    printf("Sequential Sum = %lld\n", sum_seq);
    printf("Parallel Sum   = %lld\n", sum_par);
    printf("Sequential Time = %f seconds\n", time_seq);
    printf("Parallel Time   = %f seconds\n", time_par);
    printf("Speedup = %f\n", time_seq / time_par);

    return 0;
}