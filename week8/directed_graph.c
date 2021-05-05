#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"jrb.h"
#include"dllist.h"

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph g, int id, char *name);
char* getVertex(Graph g, int v);
int hasEdge(Graph g, int v1, int v2);
void addEdge(Graph g, int v1, int v2);
int indegree(Graph g, int v, int *output);
int outdegree(Graph g, int v, int* output);
int DAG(Graph g);
void dropGraph();

int main(){
    int output[100];
    Graph g = createGraph();
    addVertex(g, 0, "NTN");
    addVertex(g, 1, "NCL");
    addVertex(g, 2, "NMH");
    addVertex(g, 3, "PTH");
    addVertex(g, 4, "BLQ");
    addEdge(g, 0, 4);
    addEdge(g, 0, 1);
    addEdge(g, 1, 3);
    addEdge(g, 2, 3);
    if(!DAG(g)) printf("This is not a DAG!\n\n");
    else printf("This is a DAG\n\n");
    return 0;
}

Graph createGraph(){
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g; 
}

void addVertex(Graph g, int id, char *name){
    JRB node = jrb_find_int(g.vertices, id);
    if(node == NULL)
       jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char* getVertex(Graph g, int v){
    JRB node = jrb_find_int(g.edges, v);
    if(node == NULL) return NULL;
    return jval_s(node->val);
}

int hasEdge(Graph g, int v1, int v2){
    JRB node = jrb_find_int(g.edges, v1);
    if(node == NULL) return 0;
    JRB tree = (JRB) jval_v(node->val);
    if(jrb_find_int(tree,v2)==NULL) return 0;
    return 1;
}

void addEdge(Graph g, int v1, int v2){
    JRB node, tree;
    if(!hasEdge(g,v1,v2)){
        node = jrb_find_int(g.edges, v1);
        if(node == NULL){
            tree = make_jrb();
            jrb_insert_int(g.edges, v1, new_jval_v(tree));
        } else tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
}

int indegree(Graph g, int v, int *output){
    JRB node, tree;
    int i = 0;
    jrb_traverse(node, g.edges){
        tree = (JRB)jval_v(node->val);
        if(jrb_find_int(tree,v)){
            output[i] = jval_i(node->key);
            i++;
        }
    }
    return i;
}

int outdegree(Graph g, int v, int* output){
    JRB node, tree;
    int i = 0;
    node = jrb_find_int(g.edges, v);
    if(node == NULL) return 0;
    tree = (JRB) jval_v(node->val);
    jrb_traverse(node, tree){
        output[i] = jval_i(node->key);
        i++;
    }
    return i;
}

int DAG(Graph g){
    int visited[1000] = {};
    int output[100], start, u, v, n, i;
    Dllist node, queue;
    JRB vertex;
    jrb_traverse(vertex, g.vertices){
        // for(int a = 0; a < 1000; a++) visited[a] = 0;
        memset(visited, 0, sizeof(visited)); //cho toan bo phan tu bang 0
        start = jval_i(vertex->key);
        queue = new_dllist(); //khoi tao
        visited[start] = 1;
        dll_append(queue, new_jval_i(start)); //dua start vao queue
        while(!dll_empty(queue)){
            //tro vao phan tu dau tien
            node = dll_last(queue);
            u = jval_i(node->val);
            dll_delete_node(node); //xoa khoi queue
            n = outdegree(g, u, output);
            for(i = 0; i < n; i++){
                v = output[i];
                if(v == start) return 0; //co chu trinh 
                if(!visited[v]){
                    //tiep tuc tim lan luot
                    dll_append(queue, new_jval_i(v));
                    visited[v] = 1;
                }
            }
        }
    }
    return 1;
}

void dropGraph(Graph g){
    JRB node, tree;
    jrb_traverse(node, g.edges){
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(g.edges);
    jrb_free_tree(g.vertices);
}