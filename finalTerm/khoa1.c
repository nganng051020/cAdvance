#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dijkstra.h"

int a[10][10];
int size;

void readfile(char* filename);

int main(){
    int menu;

    do{
        printf("1. In ma tran\n");
        printf("2. In thong tin\n");
        printf("3. Tim diem lien thong\n");
        printf("4. Danh sach nut co lien thong lon nhat\n");
        printf("5. Danh sach nut dao\n");
        printf("6. Duong di ngan nhat\n");
        printf("7. So cach co the di\n");
        printf("8. Thoat!\n");

        printf("->Ban chon: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            printf("\n");
            readfile("matran.txt");
            for(int i = 0; i < size; i++){
                {for(int j = 0; j < size; j++)
                    printf("%-2d", a[i][j]);}
                printf("\n");}
            break;
        case 2:
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    if(a[i][j] == 0 && a[i][j+1] == 0)
                        addEdge(a[i][j], a[])
                }
            }
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        default:
            break;
        }
    }while(menu != 8);
    return 0;
}

void readfile(char* filename){
    FILE* f;
    char* token;
    char str[200];
    f = fopen(filename, "r");
    fgets(str, 200, f);
        token = strtok(str, " ");
    size = 0;
    while (token != NULL)
        {
        size++;
        token = strtok(NULL, " ");
        }
    fclose(f);
    f = fopen(filename, "r");
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            fscanf(f,"%d", &a[i][j]);
        }
    }
}