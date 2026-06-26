#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

void selectionSort(int A[])
{
    printf("\nBefore sort: ");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");

    printf("\n<<<<< Selection Sorting... >>>>>\n");
    for(int i=0;i<N-1;i++)
    {
        int minIdx = i;
        for(int j=i+1;j<N;j++)
        {
            if(A[j]<A[minIdx])  minIdx = j;
        }
        //SWAP
        if(minIdx!=i)
        {
            int tmp = A[minIdx];
            A[minIdx] = A[i];
            A[i] = tmp;
            printf("%d pass >> ", i+1);
            for(int i=0;i<N;i++)
                printf("%d ",A[i]);
            printf("\n");
        }
        
    }

}

void insertionSort(int A[])
{
    printf("\nBefore sort: ");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");

    printf("\n<<<<< insertion Sorting... >>>>>\n");
    for(int i=1;i<N;i++)
    {
        int key = A[i]; //오른손에 카드를 쥔다.
        int j=i-1;
        
        while(j>=0 && A[j]>key)
        {
            A[j+1] = A[j];
            j=j-1;
        }
        A[j+1] = key;

        printf("%d pass >> ", i);
        for(int i=0;i<N;i++)
            printf("%d ",A[i]);
        printf("\n");
    }
}
void bubbleSort(int A[])
{
    printf("\nBefore sort: ");
    for(int i=0;i<N;i++)
        printf("%d ",A[i]);
    printf("\n");

    printf("\n<<<<< bubble Sorting... >>>>>\n");
    for(int i=N-1;i>0;i--)
    {
        //최적화(혼자 보고 이해하기)를 위한 flag(깃발) 변수
        int flag=0;
        for(int j=0;j<i;j++)
            if(A[j] > A[j+1])
            {
                int tmp = A[j];
                A[j] = A[j+1];
                A[j+1] = tmp;
                flag =1;
            }
        if(flag==0) break;
        printf("%d pass >> ", N-i);
        for(int i=0;i<N;i++)
            printf("%d ",A[i]);
        printf("\n");
        
        
    }

}


int main()
{
    int A[N];
    srand(time(NULL));
    for(int i=0;i<N;i++)
        A[i] = rand()%100;

    // selectionSort(A);
    // insertionSort(A);
    bubbleSort(A);
    
    
}