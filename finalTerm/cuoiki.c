#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dijkstra.h"

int m, n;
int id[15], id1, id2;
double weight;
char name[20];
Graph g;

void readfile(char* filename);

int main(){
    int search, output[15], path[15], *length, output2[15];
    int menu, x, y;
    double max;
    char filename[20], *s;
    g = createGraph();

    do{
        printf("\n--------------MENU-------------\n");
        printf("1. In danh sach dinh\n");
        printf("2. Tim ban than thiet nhat\n");
        printf("3. Kiem tra quan he bac cau\n");
        printf("4. Tim ban chung\n");
        printf("5. Thoat\n");

        do{
        printf("->Ban chon: ");
        scanf("%d", &menu);
        }while(menu < 1 || menu > 5);

        switch (menu)
        {
        case 1:
            printf("Ten file: ");
            scanf("%s", filename);
            readfile(filename);
            for(int i = 0; i < n; i++)
                printf("%d %s\n", id[i], getVertex(g, id[i]));
            break;
        case 2:
            printf("ID nguoi muon tim kiem: ");
            scanf("%d", &search);
            s = getVertex(g, search);
            printf("%s\n", s);
             max = 0;
            if(s == NULL) {printf("-1\n"); exit -1;}
            x = indegree(g, search, output);
            //printf("%d\n", x);
                for(int i = 0; i < x; i++){
                    if(getEdgeValue(g, search, output[i]) > max)
                     max = getEdgeValue(g, search, output[i]);
                }
                printf("%.1lf\n", max);
                for(int i = 0; i < x; i++){
                    if(getEdgeValue(g, search, output[i]) == max){
                        printf("%d\n%s\n",output[i], getVertex(g, output[i]));
                        }
                }
            break;
        case 3:
            printf("Nhap ID 2 nguoi muon kiem tra (VD: 1 2): ");
            scanf("%d %d", &id1, &id2);
            printf("%d %d\n", id1, id2);
            x = indegree(g, id1, output);
            //printf("%d\n",x);
            int k = 0;
            max = 0;
            for(int i = 0; i < x; i++){
                if(id2 == output[i]) k++;
                else{
                    max = shortestPath(g, id1, id2, path, &length);
                    if( length == 0) k++;
                }
            }
            if(k == 0) printf("1\n");
            else printf("-1\n");
            break;
        case 4:
            printf("\nNhap ID 2 nguoi muon kiem tra(VD: 1 2): ");
            scanf("%d %d",&id1, &id2);
            x = indegree(g, id1, output);
            y = indegree(g, id2, output2);
            k = 0;
            if(x > y){
                for(int i = 0; i < y; i++){
                    for(int j = 0; j < x; i++){
                        if(output[j] == output2[i]) {
                            k++;
                            printf("%d %s\n", output[i], getVertex(g, output[i]));}
                    }
                }
            }
            else{
                for(int i = 0; i < x; i++){
                    for(int j = 0; j < y; j++){
                        if(output[i] == output2[j]) {k++;
                        printf("%d %s\n", output[i], getVertex(g, output[i]));}
                    }
                }
            }
            printf("%d\n", k);
            break;

        default:
            break;
        }
    }while(menu != 5);

    return 0;
}

void readfile(char* filename){
    FILE* f;

    f = fopen(filename, "r");
    fscanf(f, "%d %d", &n, &m);
    for(int i = 0; i < n; i++){
        fscanf(f, "%d %s", &id[i], name);
        addVertex(g, id[i], name);

    }

    for(int i = 0; i < m; i++){
        fscanf(f, "%d %d %lf", &id1, &id2, &weight);
        addEdge(g, id1, id2, weight);
        addEdge(g, id2, id1, weight);
    }
    fclose(f);
}