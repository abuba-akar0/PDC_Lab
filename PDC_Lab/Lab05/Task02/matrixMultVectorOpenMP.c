#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000

int main()
{
    static double A[N][N];
    static double B[N];
    static double C_seq[N];
    static double C_par[N];

    // Initialize
    for(int i = 0; i < N; i++)
    {
        B[i] = 1.0;
        for(int j = 0; j < N; j++)
            A[i][j] = 1.0;
    }

    // ================= Sequential =================
    double start_seq = omp_get_wtime();

    for(int i = 0; i < N; i++)
    {
        C_seq[i] = 0;
        for(int j = 0; j < N; j++)
            C_seq[i] += A[i][j] * B[j];
    }

    double time_seq = omp_get_wtime() - start_seq;

    // ================= Parallel =================
    double start_par = omp_get_wtime();

#pragma omp parallel for
    for(int i = 0; i < N; i++)
    {
        C_par[i] = 0;
        for(int j = 0; j < N; j++)
            C_par[i] += A[i][j] * B[j];
    }

    double time_par = omp_get_wtime() - start_par;

    // Print only first 5 results (for verification)
    printf("Sample Output (First 5 elements):\n");
    for(int i = 0; i < 5; i++)
        printf("%f ", C_par[i]);

    printf("\n\nSequential Time = %f seconds\n", time_seq);
    printf("Parallel Time   = %f seconds\n", time_par);
    printf("Speedup = %f\n", time_seq / time_par);

    return 0;
}