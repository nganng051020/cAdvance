#include<stdio.h>
#include<stdlib.h>
#include"jval.h"
#include"jrb.h"

typedef JRB Graph;

Graph createGraph(){
    return make_jrb();
}

int adjacent(Graph graph, int v1, int v2);
void addEdge(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int v, int* output);
void dropGraph(Graph graph);

int main(){
    int output[100];
    int v1, v2, v;
    Graph g = createGraph();
    addEdge(g, 1, 3);
    addEdge(g, 9x, 5);
    addEdge(g, 4, 7);
    int n, menu;
    do{
        printf("\n--------------Graph_JRB-------------ntn\n");
        printf("1. Add Edge\n");
        printf("2. Get adjecent vertices\n");
        printf("3. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            printf("v1: ");
            scanf("%d", &v1);
            printf("v2: ");
            scanf("%d", &v2);
            addEdge(g, v1, v2);
            break;
        case 2:
            printf("Vertice: ");
            scanf("%d", &v);
            n = getAdjacentVertices(g,v,output);
            if(n == 0) printf("No adjacent vertices\n");
            else{
                printf("Adjacent vertices of node: ");
                for(int i = 0; i < n; i++)
                printf("%d\t", output[i]);
               }
            printf("\n");
            break;

        default:
            dropGraph(g);
            break;
        }
    }while(menu != 3);
    
    return 0;
}

int adjacent(Graph graph, int v1, int v2){
    JRB node, tree;
    node = jrb_find_int(graph, v1);
    if(node == NULL) return 0;
    tree = (JRB)jval_v(node->val);
    if(jrb_find_int(tree, v2) == NULL) return 0;
    else return 1;
}

void addEdge(Graph graph, int v1, int v2){
    JRB node, tree;
    if(!adjacent(graph, v1, v2)){
        node = jrb_find_int(graph, v1);
        if(node == NULL){
            tree = make_jrb();
            jrb_insert_int(graph, v1, new_jval_v(tree));
        }else tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
     if(!adjacent(graph, v2, v1)){
        node = jrb_find_int(graph, v2);
        if(node == NULL){
            tree = make_jrb();
            jrb_insert_int(graph, v2, new_jval_v(tree));
        }else tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
    
}

int getAdjacentVertices(Graph graph, int v, int* output){
    JRB tree, node;
    int total;
    node = jrb_find_int(graph, v);
    if(node == NULL) return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;
    jrb_traverse(node, tree)
        output[total++] = jval_i(node->key);
    
    return total;
}

void dropGraph(Graph graph){
    JRB node, tree;
    jrb_traverse(node, graph){
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph);
}

