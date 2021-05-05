#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define HUGE_NUMBER 20
#define LIMIT 100

void sort3way(int a[], int l, int r);
void sort2way(int list[], int left, int right);
void exch(int *a, int i, int j);
int* createArray(int n);
int* dumpArray(int *a, int n);


int main(){
  int *a1, *a2;
  clock_t start, end, start2, end2;
  
  a1 = createArray(HUGE_NUMBER);
  a2 = dumpArray(a1, HUGE_NUMBER);

  printf("\n");
  for(int i=0; i<HUGE_NUMBER; i++) printf("%d ", *(a2+i));
  printf("\n");

  start=clock();
  sort3way(a2, 0, HUGE_NUMBER-1);
  end=clock();
  printf("\n");
  for(int i=0; i<HUGE_NUMBER; i++) printf("%d ", *(a2+i));
  printf("\n");
  printf("RUNNING TIME: %.8f seconds (3 ways)\n\n", (double)(end-start)/CLOCKS_PER_SEC);

  start2=clock();
  sort2way(a1, 0, HUGE_NUMBER-1);
  end2=clock();
  printf("\n");
  for(int i=0; i<HUGE_NUMBER; i++) printf("%d ", *(a1+i));
  printf("\n");
  printf("RUNNING TIME: %.8f seconds (2 ways)\n", (double)(end2-start2)/CLOCKS_PER_SEC);

  return 0;
}

void sort3way(int a[], int l, int r){
  if(r<l) return;
  int i=l-1, j=r;
  int p=i, q=j;

  while(1){
    while(a[++i] < a[r]);
    while(a[r] < a[--j]) if(j==l) break;
    if(i>=j) break;
    exch(a, i, j);
    if(a[i] == a[r]) exch(a, ++p, i);
    if(a[j] == a[r]) exch(a, --q, j);
  }
  exch(a, i, r);
  j=i-1;
  i++;
  for(int k=l; k<p; k++) exch(a, k, j--);
  for(int k=r-1; k>q; k--) exch(a, k, i++);
  sort3way(a, l, j);
  sort3way(a, i, r);
}

void exch(int *a, int i, int j){
  int temp=*(a+i);
  *(a+i)=*(a+j);
  *(a+j)=temp;
}

int* createArray(int n){
  int *a=(int*)malloc(n*sizeof(int));
  srand(time(NULL));
  for(int i=0; i<n; i++) *(a+i)=rand()%(LIMIT+1);
  return a;
}

int* dumpArray(int *a, int n){
  int* b=(int*)malloc(n*sizeof(int));
  for(int i=0; i<n; i++) *(b+i)=*(a+i);
  return b;
}

void sort2way(int list[], int left, int right){
  int pivot, i, j;
  int temp;

  if(left < right){
    i = left;
    j = right+1;
    pivot = list[left];

    do{
      do i++; while(i<=right && list[i]<pivot);
      
      do j--; while(j>=left && list[j]>pivot);
      
      if(i<j) exch(list, i, j);
    }while(i<j);
    exch(list, left, j);
    sort2way(list, left, j-1);
    sort2way(list, j+1, right);
  }
}

