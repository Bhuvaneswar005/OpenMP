#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

void merge(int A[], int l, int m, int r)
{
    int i, j, k;
    int left = m - l + 1;    int right = r - m;
    int L[left], R[right];

    for (i = 0; i < left; i++){     L[i] = A[l + i];}
    for (j = 0; j < right; j++){    R[j] = A[m + 1 + j];}

    i = 0;    j = 0;    k = l;

    while (i < left && j < right) 
    {
        if (L[i] <= R[j]){   A[k] = L[i];    i++;}
        else{   A[k] = R[j];    j++;}
        k++;
    }
    while (i < left) 
    {   A[k] = L[i];    i++;    k++;}
    while (j < right) 
    {   A[k] = R[j];    j++;    k++;}
}

void mergesort(int A[], int l, int r)
{
    if (l < r) 
    {
        int mid = l + (r - l) / 2;
        mergesort(A, l, mid);
        mergesort(A, mid + 1, r);
        merge(A, l, mid, r);
    }
}


int parallel(int A[], int l, int r, int element)
{
    if (l > r)
    {   return -1;}
    if (l == r)
    {
        if (A[l] == element)
        {   return l;}
        return -1;
    }
    int mid = l + (r-l)/2;

    if (A[mid] == element)
    {   return mid;}
    int result = -1;

    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        {
            if (A[mid] > element)
            {
                #pragma omp task   
                {   result = parallel(A, l, mid-1, element);}
            }
            else
            {   
                #pragma omp task
                {   result = parallel(A, mid+1, r, element);}
            }
        #pragma omp taskwait
        }
    }
    return result;
}

int seq(int A[], int n, int element)
{
    int l = 0, r = n-1, mid;
    while (l < r)
    {
        mid = l + (r-l)/2;
        if (A[mid] == element){   return mid;}
        if (A[mid] > element){   r = mid-1;}
        else{   l = mid+1;}
    }
    if (A[l] == element)
    {   return l;}
    return -1;
}

double timecalc(struct timeval TimeValue_Start,struct timeval TimeValue_Final)
{
    long start, end;    double time_total;
    start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_total = (end - start)/1000000.0;
    return time_total;
}

int main()
{
    srand(time(0));
    int n;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int A[n];
    for (int i = 0; i < n; i++) {   A[i] = rand()%n;}
    mergesort(A, 0, n-1);

    int element = A[342], result;
    printf("\tSearching for: %d\n", element);

    struct timeval TimeValue_Start,TimeValue_Final;     struct timezone TimeZone_Start,TimeZone_Final;
   
    //Sequential
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    result = seq(A, n, element);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);

    printf("Sequential Binary Search: %lf\n", timecalc(TimeValue_Start,TimeValue_Final));
    if (result == -1){   printf("\tElement not found\n");}    else{   printf("\tElement found at: %d\n", result);}

    //Parallel
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    result = parallel(A, 0, n-1, element);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);

    printf(" Binary Search: %lf\n", timecalc(TimeValue_Start,TimeValue_Final));
    if (result == -1){   printf("\tElement not found\n");}    else{   printf("\tElement found at: %d\n", result);}
}