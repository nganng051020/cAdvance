#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dijkstra.h"
#include"jval.h"
#include"jrb.h"


Graph g;
int main(){
    int menu;
    int id, a, b;
    char name[20], *token, str[30];
    g = createGraph();
    FILE *f1, *f2;
    JRB tmp;
    tmp = make_jrb();
    JRB node;

    do{
        printf("\n1. In danh sach san pham\n");
        printf("2. In danh sach giao dich\n");
        printf("3. Hien thi muc do lien quan giua 2 san pham\n");
        printf("4. In danh sach san pham lien quan\n");
        printf("5. Hien thi moi quan he giua hai san pham\n");
        printf("6. Thoat\n\n");
         
        do{
            printf("->Ban chon: ");
            scanf("%d", &menu);
        }while(menu < 1 || menu > 6);
        

        switch (menu)
        {
        case 1:
            f1 = fopen("products.txt", "r");
            while(!feof(f1)){
                fscanf(f1, "%d%*c%s", &id, name);
                jrb_insert_int(tmp, id, new_jval_s(strdup(name)));
            }
            jrb_traverse(node, tmp)
               { printf("\nMa san pham: %d\nTen san pham: %s\n", jval_i(node->key), jval_s(node->val));}
            fclose(f1);
            
            break;
        case 2:
            f2 = fopen("orderhistory.txt", "r");
            
            while(!feof(f2)){
                fgets(str, 10, f2);
                token = strtok(str, " ");
                while(token != NULL)
                    {
                    a = atoi(token);
                    node = jrb_find_int(tmp, a);
                    printf("%s\t", jval_s(node->val));
                    token = strtok(NULL, " ");
                    }
                    printf("\n");
                }
                
            break;

        case 3:
            printf("\nNhap vao ma so 2 san pham muon kiem tra (VD:1 2): ");
            scanf("%d %d", &a, &b);

            break;
        case 4:
        case 5:
        default:
            break;
        }
    }while(menu != 6);
}