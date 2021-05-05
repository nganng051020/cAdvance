#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dijkstra.h"

Graph g;
Graph g1;

int main(){
    int menu;
    int output[8], path[8], length[8];
    memset(path, 0, sizeof(path));
    int n,m,x[9], a[9], tmp = 0;
    int i, j;
    int id1, id2;
    float time;
    g = createGraph();
    g1 = createGraph();
    FILE* f;

    f = fopen("dothi.txt", "r");
            if(f == NULL){
                printf("ERROR!!!\n");
                exit -1;
            }
            fscanf(f, "%d\t%d", &n, &m);
            for(i = 0; i < m; i++){
                fscanf(f, "%d\t%d\t%f", &id1, &id2, &time);
                addEdge(g, id2, id1, time);
                addEdge(g, id1, id2, time);
                if(time >= 50){
                    addEdge(g1, id1, id2, time);
                    addEdge(g1, id2, id1, time);
                }
            }


    do{
        printf("\n1. In ma tran ke\n");
        printf("2. In danh sach ke cua moi thanh tri\n");
        printf("3. Danh sach thanh tri...\n");
        printf("4. Tim duong di ngan nhat\n");
        printf("5. Duong di bo ngan nhat\n");
        printf("6. Thoat\n");

        printf("\n->Ban chon: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            for(i = 1; i <= n; i++){
                for( j = 1; j <= n; j++){
                    if(getEdgeValue(g, i, j) == -1) printf("0 ");
                    else printf("1 ");
                }
                printf("\n");
            }
            break;
        case 2:
            printf("\nChon ten dinh: ");
            scanf("%d", &id1);
            x[0] = outdegree(g, id1, output);
            printf("Danh sach canh ke cua %d: ", id1);
            for(int i = 0; i < x[0]; i++) {
                printf("%d\t", output[i]);}
            printf("\n");
            break;

        case 3:
            printf("\n-> 3a. Danh sach thanh tri chi co the di bo toi: ");
            for( i = 1; i <= n; i++){
                x[i] = outdegree(g, i, output);
                tmp = 0;
                for( j = 0; j < x[i]; j++){
                    
                    if(getEdgeValue(g,i,output[j]) >= 50) tmp++;
                }
                if(tmp == x[i]) printf("%d\t", i);
            }
            printf("\n");

            printf("\n-> 3b. Thanh tri co nhieu duong di toi no nhat: ");
            tmp = x[1];
            for( i = 2; i <= n; i++){
                if(tmp < x[i]) tmp = x[i];
            }
            for( i = 1; i <= n; i++){
                if(x[i] == tmp) printf("%d\t", i);
            }
            break;
        case 4: 
            printf("\nNhap thanh tri xuat phat: ");
            scanf("%d", &id1);
            printf("Nhap thanh tri ket thuc: ");
            scanf("%d", &id2);
            time = shortestPath(g, id1, id2, path, length);
            printf("->Do dai duong di: %.0f\n", time);
            printf("->Thu tu cac thanh tri phai di qua: ");
            for(int i = 0; i < n; i++){
                if(path[i] != 0)
                printf("%d\t", path[i]);
            }
            printf("\n");
            break;
        case 5: 
            printf("Nhap thanh tri xuat phat: ");
            scanf("%d", &id1);
            printf("Nhap thanh tri ket thuc: ");
            scanf("%d", &id2);
            time = shortestPath(g1, id1, id2, path, length);
            if(time == 10000000) time = 0;
            printf("->Do dai duong di: %.0f\n", time);
            if(time == 0) printf("->Khong co duong di bo tu %d-> %d\n", id1, id2);
            else{
            printf("->Thu tu cac thanh tri phai di qua: ");
            for(int i = 0; i < n; i++){
                if(path[i] != 0)
                printf("%d\t", path[i]);
            }}
            printf("\n");

            break;
        default:
            break;
        }
    }while(menu != 6);

    return 0;
}