#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int* createArray(int size);
int* dumpArray(int *A, int size);
void exch(int *A, int i, int j);
void sort3way(int A[], int l, int r);
void sort2way(int A[], int l, int r);


int main(){
  int size = 20;
  int *A1, *A2;
  clock_t start, end, start2, end2;
  A1 = createArray(size);
  A2 = dumpArray(A1, size);

  for(int i = 0; i < size; i++) printf("%d ", *(A1+i));

  start = clock();
  sort3way(A1,0,size-1);
  end = clock();
  for(int i = 0; i < size; i++) printf("%d\t", *(A1+i));
  printf("\nTime running: %.8f seconds (3ways)\n\n", (double)(end-start)/CLOCKS_PER_SEC);

  start2 = clock();
  sort2way(A2,0,size-1);
  end2 = clock();
  for(int i = 0; i < size; i++) printf("%d\t", *(A2+i));
  printf("\n running: %.8f seconds (2ways)\n\n",(double)(end2-start2)/CLOCKS_PER_SEC);

  return 0;
}

int* createArray(int size){
  int *A;
  A = (int*)malloc(size*sizeof(int));
  srand(time(NULL));
  for(int i = 0; i < size; i++)
    *(A+i)= rand()%1000;
  return A;
}

int* dumpArray(int *A, int size){
  int *B;
  B = (int*)malloc(size*sizeof(int));
  for(int i = 0; i < size; i++) *(B+i) = *(A+i);
  return B;
}

void exch(int *A, int i, int j){
  int temp= *(A+i);
  *(A+i) = *(A+j);
  *(A+j) = temp;
}

void sort3way(int A[], int l, int r){
  if(r<=l) return;
  int i = l-1, j = r;
  int p = l-1, q = r;
  while(l){
    while(A[++i] < A[r]);
    while(A[--j] > A[r]) if(j == l) break;
    if(i>=j) break;
    exch(A,i,r);
    if(A[i]==A[r]) exch(A,++p,r);
    if(A[j]==A[r]) exch(A,--q,r);
  }
  exch(A,i,r);
  j = i-1;
  i = i-1;
  for(int k = l; k <= p; k++) exch(A, k,j--);
  for(int k = r-1; k >= q; k--) exch(A, k, i++);
  sort3way(A,l,j);
  sort3way(A,i,r);
}

void sort2way(int A[], int l, int r){
int pivot, i, j;
  int temp;

  if(l < r){
    i = l;
    j = r+1;
    pivot = A[l];

    do{
      do i++; while(i<=r && A[i]<pivot);
      
      do j--; while(j>=l && A[j]>pivot);
      
      if(i<j) exch(A, i, j);
    }while(i<j);
    exch(A, l, j);
    sort2way(A, l, j-1);
    sort2way(A, j+1, r);
  }
}