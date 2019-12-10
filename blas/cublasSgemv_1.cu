#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include "cublas_v2.h"
#include <time.h>

void generate_random_vector(int N, float **outX)
{
    int i;
    double rMax = (double)RAND_MAX;
    float *X = (float *)malloc(sizeof(float) * N);

    for (i = 0; i < N; i++)
    {
        int r = rand();
        double dr = (double)r;
        X[i] = (dr / rMax) * 100.0;
    }

    *outX = X;
}

void generate_random_dense_matrix(int M, int N, float **outA)
{
    int i, j;
    double rMax = (double)RAND_MAX;
    float *A = (float *)malloc(sizeof(float) * M * N);

    // For each column
    for (j = 0; j < N; j++)
    {
        // For each row
        for (i = 0; i < M; i++)
        {
            double dr = (double)rand();
            A[j * M + i] = (dr / rMax) * 100.0;
        }
    }

    *outA = A;
}

int main(int argc, char **argv)
{
    int i;
    float *A, *dA;
    float *X, *dX;
    float *Y, *dY;
    float beta;
    float alpha;
    cublasHandle_t handle = 0;

    /*
    alpha = 3.0f;
    beta = 16.0f;
    */

    alpha = 1;
    beta = 1;

    /*
    * M = # of rows
    * N = # of columns
    */
    
    int M = 100;
    int N = 5;

    struct timespec startTime, endTime, sleepTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 123;

    // Generate inputs
    srand(9384);
    generate_random_dense_matrix(M, N, &A);
    generate_random_vector(N, &X);
    generate_random_vector(M, &Y);

    cublasCreate(&handle);
    cudaMalloc((void **)&dA, sizeof(float) * M * N);
    cudaMalloc((void **)&dX, sizeof(float) * N);
    cudaMalloc((void **)&dY, sizeof(float) * M);
    cublasSetVector(N, sizeof(float), X, 1, dX, 1);
    cublasSetVector(M, sizeof(float), Y, 1, dY, 1);
    cublasSetMatrix(M, N, sizeof(float), A, M, dA, M);

    cublasSgemv(handle, CUBLAS_OP_N, M, N, &alpha, dA, M, dX, 1, &beta, dY, 1);
    cublasGetVector(M, sizeof(float), dY, 1, Y, 1);

    clock_gettime(CLOCK_REALTIME, &endTime);

    for (i = 0; i < 10; i++)
    {
        printf("%2.2f\n", Y[i]);
    }

    printf("...\n");

    printf("started = %10ld.%09ld\n", startTime.tv_sec, startTime.tv_nsec);
    printf("finished = %10ld.%09ld\n", endTime.tv_sec, endTime.tv_nsec);
    printf("elapsed time = ");
    if (endTime.tv_nsec < startTime.tv_nsec) {
    	 printf("%ld.%09ld", endTime.tv_sec - startTime.tv_sec - 1 ,endTime.tv_nsec + 1000000000 - startTime.tv_nsec);
    } else {
	 printf("%ld.%09ld", endTime.tv_sec - startTime.tv_sec ,endTime.tv_nsec - startTime.tv_nsec);
    }
    printf(" sec\n");

    free(A);
    free(X);
    free(Y);

    cudaFree(dA);
    cudaFree(dY);
    cublasDestroy(handle);

    return 0;
}
