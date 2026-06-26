#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9

#define SWAP(x,y,t) ((t)=(x), (x)=(y), (y)=(t))

int partition(int A[], int left, int right)
{
    int pivot = A[left];

    int low = left+1;
    int high = right;
    int tmp; //for SWAP

    while(low<=high)
    {
        while(low<=high && A[low]<=pivot)
            low++;
        while(low<=high && A[high]>pivot) //여기가 A[high]가 아니고 A[low]라는데? 맞음.? 오타겟지?
            high--;
        
        for(int i=0;i<N;i++)
            printf("%d ",A[i]);
        printf("\nlow= %d, high= %d\n", low, high);
        if(low<high)
            SWAP(A[low], A[high], tmp);
    }
    SWAP(A[left], A[high], tmp);
    printf("partition over\n");
    return high;
}

void quickSort(int A[], int left, int right)
{
    if(left<right) //문제를 분할할 수 있으면
    {
        int q = partition(A, left, right); //pivot의 위치
        //순환 호출(분할), pivot의 위치는 고정되어있으므로 포함하지 않는다.
        quickSort(A, left, q-1);
        quickSort(A,q+1,right);

    }
}

int main()
{
    // int A[N];
    // srand(time(NULL));
    // for(int i=0;i<N;i++)
    //     A[i] = rand()%100;
    int A[N] = {5,3,8,4,9,1,6,2,7};
    printf("\nBefore sort: ");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");

    printf("\n<<<<< Quick Sorting... >>>>>\n");

    quickSort(A,0,N-1);

    
    // for(int i=0;i<N;i++)
    //     printf("%d ",A[i]);
    // printf("\n");


    
}