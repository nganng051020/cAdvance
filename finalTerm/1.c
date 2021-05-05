#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"jrb.h"
#include"directed_graph.h"

Graph g;
JRB tmp, chuyen_id;
int id = 1;

int main(){
    g = createGraph();
    chuyen_id = make_jrb();
    int id = 1;
    char str_id1[10], str_id2[10], name[30];
    char c;
    double gia_ve = 0.0;
    int v1,v2;  
    int menu;
    

    do{
        printf("\n1. In thong tin\n");
        printf("2. So diem bay va chuyen bay\n");
        printf("3. Tim kiem chuyen bay\n");
        printf("4. Tinh co duong bay nhieu nhat\n");
        printf("5. Ton tai\n");
        printf("6. Thanh phan lien thong\n");
        printf("7. Lo trinh bay toi uu\n");
        printf("8. Thoat\n\n");

        printf("->chon: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            FILE *f;
            f = fopen("Flight.txt", "r");
            if(f == NULL){
                printf("Khong mo duoc file\n");         
                return 0;
            }
            
            while(!feof(f)){
            fscanf(f, "%[^= ]s",str_id1);
            fscanf(f, "%c",&c);
            if(c=='='){
                fscanf(f,"%*c%[\n]s",name);
                name[strlen(name)]='\0';

                jrb_insert_str(chuyen_id, str_id1, new_jval_i(id));

                addVertex(g, id, name);
                id++;
            }
            else{
                
                fscanf(f, "%s %f",str_id2,gia_ve);
                tmp = jrb_find_str(chuyen_id, str_id1);
                v1 = jval_i(tmp->val);
                tmp = jrb_find_str(chuyen_id, str_id2);
                v2 = jval_i(tmp->val);
                addEdge(g, v1, v2, gia_ve);
            }
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        default:printf("...\n"); break;
        }
    }while(menu != 8);
}