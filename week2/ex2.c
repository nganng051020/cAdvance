#include<stdio.h>
#include<stdlib.h>
#include"jval.h"

void sort_gen(Jval a[], int l, int r, int (*compare)(Jval*, Jval*));
int search_gen(Jval a[], int l, int r, Jval item, int (*compare)(Jval*, Jval*));
int compare_i(Jval *a, Jval* b);

int main(){

}