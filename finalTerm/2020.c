#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dijkstra.h"
#include"jrb.h"
#include"jval.h"

Graph g;

int main(){
    g = createGraph();
    FILE *f1, *f2;
    int menu;
    int n, id, x,i = 0, k = 0,m, a[10], max, min, output[6], b[6], num;
    char url[90], *token, str[200];
    JRB tmp;
    tmp = make_jrb();
    JRB tree = make_jrb();
    JRB node;

    do{
        printf("\n1. \n");
        printf("2. \n");
        printf("3. \n");
        printf("4. \n");
        printf("5. \n");
        printf("6. Thoat\n");

        printf("->Ban chon: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            f1 = fopen("webpages.txt", "r");
            if(f1 == NULL ) {printf("ERROR\n"); return 0;}
            fscanf(f1, "%d", &n);
            printf("\nSo luong webpage: %d\n", n);
            for(i = 0; i < n; i++){
                fscanf(f1,"%s %d", url, &id);
                jrb_insert_int(tmp, id, new_jval_s(strdup(url)));
            }
            printf("URL:\n");
            min = max = 0;
            jrb_traverse(node, tmp){
                printf("%-60s%-10d\n", jval_s(node->val), jval_i(node->key));
            }
            fclose(f1);
            f2 = fopen("pageConnections.txt", "r");
            fscanf(f2,  "%d", &n);
            while(!feof(f2)){
                fgets(str, 200, f2);
                token = strtok(str, " ");
                k = 0;
                while (token != NULL)
                {
                    a[k] = atoi(token);
                    k++;
                   // printf("%s\t", token);
                    token = strtok(NULL, " ");
                }
                if(k > 1) {for(int j = 1; j <= k-1; j++) addEdge(g, a[0], a[j], 1);}
            }
            max = 0;
            printf("\n");
            jrb_traverse(node, tmp){
                id = jval_i(node->key);
                x = indegree(g, id, output);
              //  for(i = 0; i < x; i++) printf("%d\t", output[i]);
                if(max < x) {
                    max = x;
                    k = id;
                };
            }
            min = max;
            jrb_traverse(node, tmp){
                id = jval_i(node->key);
                x = indegree(g, id, output);
                // printf("\n%d-%d\t", id, x);
                if(min > x) {
                    min = x;
                    m = id;}
            }
            printf("\nmin: %d-%d\t max: %d-%d\n", min, m, max,k);
            fclose(f2);
            break;
        case 2:
            num = 1;
            i = 0;
            jrb_traverse(node, tmp){
                id = jval_i(node->key);
                x = outdegree(g, id, output);
                jrb_insert_int(tree, id, new_jval_i(num/x));
            }
            jrb_traverse(node, tmp){
                id = jval_i(node->key);
                x = outdegree(g, id, output);
                for(i = 0; i< x; i++){
                    node = jrb_find_int(tree, output[i]); 
                
                }
            }

            break;
        case 3:
        case 4:
        case 5:
        default: jrb_free_tree(tmp);
            break;
        }
    }while(menu != 6);

    return 0;
}