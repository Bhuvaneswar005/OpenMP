#include<stdio.h>
#include<omp.h>

void main(void)
{
    int i,tid,minimum=__INT_MAX__;

    int a[10] = { 10, 9, 71, 101, 3, 44, 12, 78, 34, 23};  

    #pragma omp parallel  num_threads(6) 
    {
    int tid=omp_get_thread_num();

    #pragma omp for private(i) reduction(min:minimum)
    for(i=0;i<10;i++)
    {
        if (a[i]<minimum)
        {
            minimum=a[i];
        }
    }
    printf("Minimum= %d\n",minimum);
    }
}