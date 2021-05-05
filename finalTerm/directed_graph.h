#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jrb.h"
#include "dllist.h"

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph g, int id, char* name);
char* getVertex(Graph g, int v);
int hasEdge(Graph g, int v1, int v2);
void addEdge(Graph g, int v1, int v2);
int indegree(Graph g, int v, int* output);
int outdegree(Graph g, int v, int* output);
int DAG(Graph g);
void dropGraph();
void topoSort(Graph g, int* output, int* n);

/*int main(){
    int i, n, output[100];
    Graph g = createGraph();
    addVertex(g, 0, "CS102");
    addVertex(g, 1, "CS140");
    addVertex(g, 2, "CS160");
    addVertex(g, 3, "CS302");
    addVertex(g, 4, "CS311");
    addVertex(g, 5, "MATH300");
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 3);
    addEdge(g, 5, 4);
    addEdge(g, 3, 4);
    if(!DAG(g)){
        printf("This is not a DAG\n");
        return 0;
    }
    topoSort(g, output, &n);
    for(i = 0; i < n; i++)
        printf("%d ", output[i]);
    return 0;
}
*/

void topoSort(Graph g, int* output, int* n){
    int indeg[1000], tmp[100], m, i, u, v, cnt = 0;
    JRB vertex;
    Dllist node, queue;
    queue = new_dllist();
    jrb_traverse(vertex, g.vertices){
        u = jval_i(vertex->key);
        indeg[u] = indegree(g, u, tmp);
        if (!indeg[u])
            dll_append(queue, new_jval_i(u));
    }
    while(!dll_empty(queue)){
        node = dll_first(queue);
        u = jval_i(node->val);
        dll_delete_node(node);
        output[cnt++] = u;
        m = outdegree(g, u, tmp);
        for(i = 0; i < m; i++){
            v = tmp[i];
            indeg[v]--;
            if (!indeg[v])
                dll_append(queue, new_jval_i(v));
        }
    }
    *n = cnt;
}

Graph createGraph(){
    Graph g;
    // tao cay do den luu tru canh
    g.edges = make_jrb();
    // tao cay do den luu tru dinh
    g.vertices = make_jrb();
    return g;
}

void addVertex(Graph g, int v, char* name){
    JRB node = jrb_find_int(g.vertices, v);
    if (node == NULL)
        jrb_insert_int(g.vertices, v, new_jval_s(strdup(name)));
}

char* getVertex(Graph g, int v){
    JRB node = jrb_find_int(g.vertices, v);
    if (node == NULL)
        return NULL;
    return jval_s(node->val);
}

int hasEdge(Graph g, int v1, int v2){
    JRB node = jrb_find_int(g.edges, v1);
    if (node == NULL)
        return 0;
    JRB tree = (JRB) jval_v(node->val);
    if(jrb_find_int(tree, v2) == NULL)
        return 0;
    return 1;
}

void addEdge(Graph g, int v1, int v2){
    JRB node, tree;
    if (!hasEdge(g, v1, v2)){
        node = jrb_find_int(g.edges, v1);
        if (node == NULL){
            tree = make_jrb();
            jrb_insert_int(g.edges, v1, new_jval_v(tree));
        } else tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
}

int indegree(Graph g, int v, int *output){
    JRB node, tree;
    int cnt = 0;
    jrb_traverse(node, g.edges){
        tree = (JRB) jval_v(node->val);
        if(jrb_find_int(tree, v)){
            output[cnt] = jval_i(node->key);
            cnt++;
        }
    }
    return cnt;
}


int outdegree(Graph g, int v, int* output){
    JRB node, tree;
    int cnt = 0;
    node = jrb_find_int(g.edges, v);
    if (node == NULL)
        return 0;
    tree = (JRB) jval_v(node->val);
    jrb_traverse(node, tree){
        output[cnt] = jval_i(node->key);
        cnt++;
    }
    return cnt;
}

int DAG(Graph g){
    // DFS tai tung dinh start. \
    // Neu dinh start nay duoc vieng tham lai
    // thi do thi co chu trinh
    int visited[1000] = {};
    int n, output[100], i, u, v, start;
    Dllist node, stk;
    JRB vertex;
    jrb_traverse(vertex, g.vertices){
        // reset cac phan tu ve 0
        memset(visited, 0, sizeof(visited));
        start = jval_i(vertex->key);
        stk = new_dllist();
        visited[start] = 1;
        dll_append(stk, new_jval_i(start));
        while (!dll_empty(stk)){
            node = dll_last(stk);
            u = jval_i(node->val);
            dll_delete_node(node);
            n = outdegree(g, u, output);
            for(i = 0; i < n; i++){
                v = output[i];
                if (v == start) // tim ra chu trinh
                    return 0;
                if (!visited[v]){
                    dll_append(stk, new_jval_i(v));
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
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(g.edges);
    jrb_free_tree(g.vertices);
}


































