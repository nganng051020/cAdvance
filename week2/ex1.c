#include<stdio.h>
#include<stdlib.h>

void exch(void *buf, int size, int i, int j);
int int_compare(void const* x, void const *y);
void sort(void *buf, int l, int r, int size, int (*compare)(void*, void*));
int search(void *buf, int size, int l, int r, void* iteam, int (*compare)(void*, void*));

int main()
{
    int size = 20;
    int a[20];
    int item = 5, res;

    srand(time(NULL));
    for(int i = 0; i < size; i++) a[i] = rand()%101;
    printf("\n");

    sort(a, 0, size-1, sizeof(int), int_compare);
    for(int i = 0; i < size; i++) printf("%d ", a[i]);
    printf("\n");

    res = search(a, sizeof(int), 0, size -1, &item, int_compare);
    printf("%d\n", res);

    return 0;
}

void exch(void *buf, int size, int i, int j){
    int temp = *((int*)buf+i);
    *((int*)buf+i) = *((int*)buf+j);
    *((int*)buf+j) = temp;
}

int int_compare(void const* x, void const*y){
    int m, n;
    m = *((int*)x);
    n = *((int*)y);
    if(m == n) return 0;
    return m > n ? 1 : -1;
}

void sort(void *buf, int l, int r, int size, int (*compare)(void*, void*)){
    int i = l-1, j = r;
    int p = l-1, q = r;
    while(l){
        while(compare((char*)buf+(++i)*size, (char*)buf + r*size) ==-1);
        while(compare((char*)buf+(--j)*size, (char*)buf + r*size) == 1);
        if(i > j) break;
        exch(buf,size,i,j);
        if(compare((char*)buf+i*size, (char*)buf+r*size) == 0)
        exch(buf, size, ++p, i);
        if(compare((char*)buf+j*size, (char*)buf+r*size) == 0)
        exch(buf, size, --q, j);
    }
    exch(buf, size, i, r);
    j = i - 1;
    i++;
    for(int k = l; k <= p; k++) exch(buf, size, k, j--);
    for(int k = r - 1; k >= q; k++) exch(buf, size, k, i++);
    sort(buf,size,l,j,compare);
    sort(buf,size,i,r,compare);
}

int search(void *buf, int size, int l, int r, void *item, int (*compare)(void*, void*)){
    int i, res;
    if(r < l) return -1;
    i = (l+r)/2;
    res = compare(item, (char*)buf+i*size);
    if(res==0) return i;
    else if(res < 0)
        return search(buf,size,l,i-1,item,compare);
    else
        return search(buf,size,i-1,r,item,compare);
}
