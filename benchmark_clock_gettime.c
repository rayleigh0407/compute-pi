#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define TEST_TIME 100

double count_avg(double data[])
{
    double avg = 0.0, pow_sum = 0.0, sd = 0.0, new_avg = 0.0;
    int i, num = 0;
    for(i = 0; i < TEST_TIME; i++) {
        avg += data[i];
        pow_sum += pow(data[i], 2);
    }
    avg /= (double) TEST_TIME;
    pow_sum /= (double) TEST_TIME;
    sd = sqrt(pow_sum - pow(avg, 2));
    for(i = 0; i < TEST_TIME; i++) {
        if((avg - 2 * sd < data[i]) && (data[i] < avg + 2 * sd)) {
            new_avg += data[i];
            num++;
        }
    }
    return (new_avg / (double) num);
}


int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i, j, loop = 200, test_time = TEST_TIME;
    double *time_baseline, *time_omp2, *time_omp4,
           *time_avx, *time_avxu;

    time_baseline = (double*)malloc(sizeof(double) * test_time);
    time_omp2 = (double*)malloc(sizeof(double) * test_time);
    time_omp4 = (double*)malloc(sizeof(double) * test_time);
    time_avx = (double*)malloc(sizeof(double) * test_time);
    time_avxu = (double*)malloc(sizeof(double) * test_time);

    for (j = 0; j < test_time; j++) {
        // Baseline
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_baseline(N);
        }
        clock_gettime(CLOCK_ID, &end);
        time_baseline[j] = (double) (end.tv_sec - start.tv_sec) +
                           (end.tv_nsec - start.tv_nsec)/ONE_SEC;

        // OpenMP with 2 threads
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_openmp(N, 2);
        }
        clock_gettime(CLOCK_ID, &end);
        time_omp2[j] = (double) (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec)/ONE_SEC;

        // OpenMP with 4 threads
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_openmp(N, 4);
        }
        clock_gettime(CLOCK_ID, &end);
        time_omp4[j] = (double) (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec)/ONE_SEC;

        // AVX SIMD
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_avx(N);
        }
        clock_gettime(CLOCK_ID, &end);
        time_avx[j] = (double) (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec)/ONE_SEC;

        // AVX SIMD + Loop unrolling
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_avx_unroll(N);
        }
        clock_gettime(CLOCK_ID, &end);
        time_avxu[j] = (double) (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    double average = 0.0;
    average = count_avg(time_baseline);
    printf("%lf ", average);
    average = count_avg(time_omp2);
    printf("%lf ", average);
    average = count_avg(time_omp4);
    printf("%lf ", average);
    average = count_avg(time_avx);
    printf("%lf ", average);
    average = count_avg(time_avxu);
    printf("%lf\n", average);
    free(time_baseline);
    free(time_omp2);
    free(time_omp4);
    free(time_avx);
    free(time_avxu);
    return 0;
}
