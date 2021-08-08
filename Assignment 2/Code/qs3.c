#include<stdio.h>
#include<omp.h>
int main()
{
    int i,N=5;
    int a[5],b[5],c[N];

    //Initializing the array
    for(i=0;i<5;i++)
    {
        a[i]=i; 
        b[i]=i;
    }

    //compute parallel
    #pragma omp parallel 
        #pragma omp for 
        for(i=0;i<5;i++)
        {
            int tid=omp_get_thread_num();
            c[i]=a[i]+b[i];
            printf("Computation at Thread [%d]=%d\n",tid,c[i]);
        }
}