#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dijkstra.h"

Graph g, g2;
int n, m;
char name[15], name2[15];
int id[5], id2[5];
JRB tmp, node;

void readfile1(char* c);
int main(int argc, char* argv[]){
    g = createGraph();
    g2 = createGraph();
    tmp = make_jrb();
    FILE* f;

    if(argc == 2 && strcmp(argv[1], "-t") == 0)
        {printf("C-Adcanced, HK20182\n"); return 0;}
    
    if(argc == 3 && strcmp(argv[1], "-s") == 0){
        readfile1(argv[2]);
        printf("\n");
        jrb_traverse(node, tmp)
            printf("%s %d\n", jval_s(node->val), jval_i(node->key));
    }

    if(argc == 6 && strcmp(argv[2], "-w") == 0){
        readfile1(argv[3]);
        readfile2(argv[2]);
    }
}

void readfile1(char *c){
    FILE* f;

    f = fopen( c, "r");
    fscanf(f, "%d", &m);
    for(int i = 0; i < m; i++){
        fscanf(f, "%s %d", name, &id[i]);
        jrb_insert_int(tmp, id[i], new_jval_s(strdup(name)));
    }
    fclose(f);
}

void readfile2(char *c){
    FILE *f;
    f = fopen(c, "r");
    fscanf(f, "%d", &n);
    for(int i = 0; i < n; i++){
        fscanf(f, "%s %d", name, &id[i]);
        addVertex(g2, id[i], name);
        for(int j = 0; j < m; j++){
            fscanf(f, "%d %d", &id2[i], sl[i]);
            }   
        }
    fclose(f);
}