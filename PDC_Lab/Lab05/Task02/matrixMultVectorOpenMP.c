#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000

int main()
{
    static double A[N][N];
    static double B[N];
    static double C[N];

    // Initialize matrix and vector
    for(int i = 0; i < N; i++)
    {
        B[i] = 1.0;
        for(int j = 0; j < N; j++)
            A[i][j] = 1.0;
    }

    // -------------------------
    // Sequential Version
    // -------------------------
    double start_seq = omp_get_wtime();

    for(int i = 0; i < N; i++)
    {
        C[i] = 0;
        for(int j = 0; j < N; j++)
            C[i] += A[i][j] * B[j];
    }

    double time_seq = omp_get_wtime() - start_seq;

    // -------------------------
    // Parallel Version
    // -------------------------
    double start_par = omp_get_wtime();

#pragma omp parallel for
    for(int i = 0; i < N; i++)
    {
        C[i] = 0;
        for(int j = 0; j < N; j++)
            C[i] += A[i][j] * B[j];
    }

    double time_par = omp_get_wtime() - start_par;

    printf("Sequential Time = %f seconds\n", time_seq);
    printf("Parallel Time   = %f seconds\n", time_par);
    printf("Speedup = %f\n", time_seq / time_par);

    return 0;
}