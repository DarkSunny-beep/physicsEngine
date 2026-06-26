#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

void shellSort(int A[])
{
    printf("\nBefore sort: ");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");

    printf("\n<<<<< Shell Sorting... >>>>>\n");

    int gap = N/2;
    while(gap>0)
    {
        for(int i=gap;i<N;i++)
        {
            int key = A[i]; //오른손에 카드를 쥔다.
            int j=i-gap;
        
            while(j>=0 && A[j]>key)
            {
                A[j+gap] = A[j];
                j-=gap;
            }
            A[j+gap] = key;
        }
        printf("%d pass >> ", gap);
        for(int i=0;i<N;i++)
            printf("%d ",A[i]);
        printf("\n");
        
        gap/=2;
    }
    
}

int sorted[N]; //합병을 위한 전역 변수 배열

void merge(int A[], int left, int mid, int right)
{
    int i=left;
    int j=mid+1;
    int k=left;

    while(i<=mid && j<=right)
    {
        if(A[i] <= A[j])
            sorted[k++] = A[i++];
        else
            sorted[k++] = A[j++];
    }

    if(i>mid) //오른쪽 리스트에만 원소가 남아있음
    {
        for(int l=j;l<=right;l++)
            sorted[k++] = A[l];
    }
    else    //왼쪽 리스트에만 원소가 남아있음
    {
        for(int l=i;l<=mid;l++)
            sorted[k++] = A[l];
    }

    for(int l=left;l<=right;l++)
        A[l]=sorted[l]; //원본 배열에 sorted 집어넣어주기
}

void mergeSort(int A[], int left, int right)
{
    if(left<right) //문제를 분할할 수 있으면
    {
        int mid =(left+right)/2;
        //순환 호출(분할)
        mergeSort(A, left, mid);
        mergeSort(A,mid+1,right);
        merge(A,left,mid,right); //합병

    }
}

int main()
{
    int A[N];
    srand(time(NULL));
    for(int i=0;i<N;i++)
        A[i] = rand()%100;

    printf("\nBefore sort: ");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");
    // shellSort(A);

    mergeSort(A,0,N-1);

    printf("\n<<<<< Merge Sorting... >>>>>\n");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");


    
}