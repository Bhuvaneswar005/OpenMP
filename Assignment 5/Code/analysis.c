#include<stdio.h>
#include<omp.h>
#include<sys/time.h>
#include<stdlib.h>
#define N 100000
int main()
{
int a[N];
int b[N];
int c[N];
struct timeval TimeValue_Start;
struct timezone TimeZone_Start;
struct timeval TimeValue_Final;
struct timezone TimeZone_Final;
long time_start, time_end;
double time_overhead;
for(int i = 0;i < N; i++) {
a[i] =rand() % N;
b[i] =rand() % N;
c[i] =0;
}


gettimeofday(&TimeValue_Start, &TimeZone_Start);
#pragma omp parallel shared(a,b,c) num_threads(8)
{
#pragma omp for schedule(guided)
for(int i = 0; i < N; i++) c[i] = a[i] + b[i];
}
gettimeofday(&TimeValue_Final, &TimeZone_Final);
time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
time_overhead = (time_end - time_start)/1000000.0;
printf("\n\n\t\t Time in Seconds (T) : %lf\n\n", time_overhead);
}