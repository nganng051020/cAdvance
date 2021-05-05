#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"jval.h"
#include"dijkstra.h"

Graph g;

int main(int argc, char* argv[]){
    g = createGraph();
    JRB tmp = make_jrb();
    JRB node;
    FILE *f1, *f2;
    char name[15];
    int id, n, m;

    if(argc == 2 && strcmp(argv[1],"-t") == 0){
    printf("C-Advance, HK20182\n");
    return 0;}
    if(argc == 3 && strcmp(argv[1],"-s") == 0){
            f1 = fopen(argv[2], "r");
            fscanf(f1, "%d", &n);
            for(int i = 0; i < n; i++){
                fscanf(f1, "%s%*c%d", name, &id );
                jrb_insert_str(tmp, strdup(name), new_jval_i(id));}
            jrb_traverse(node, tmp)
                printf("%-20s%-10d\n", jval_s(node->key), jval_i(node->val));
            jrb_free_tree(tmp);
        }
    if(argc == 4 && strcmp(argv[1], "-w")){
            f2 = fopen(argv[2], "r");
            fscanf(f2, "%d", &m);
            
    }
    
    return 0;
}
