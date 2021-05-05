#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"jrb.h"
#include"dllist.h"
#define INFINITIVE_VALUE 10000000

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph g, int id, char *name);
char* getVertex(Graph g, int v);
double getEdgeValue(Graph g, int v1, int v2);
void addEdge(Graph g, int v1, int v2, double weight);
int indegree(Graph g, int v, int *output);
int outdegree(Graph g, int v, int* output);
void dropGraph(Graph g);
double shorstestPath(Graph g, int s,int t, int* path, int *length);

int main(){
    int output[100];
    Graph g = createGraph();
    addVertex(g, 0, "NTN");
    addVertex(g, 1, "NCL");
    addVertex(g, 2, "NMH");
    addVertex(g, 3, "PTH");
    addVertex(g, 4, "BLQ");
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

double getEdgeValue(Graph g, int v1, int v2){
    JRB node = jrb_find_int(g.edges, v1);
    if(node == NULL) return INFINITIVE_VALUE;
    JRB tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree,v2);
    if(node == NULL) return INFINITIVE_VALUE;
    else return jval_d(node->val);
}

void addEdge(Graph g, int v1, int v2, double weight){
    JRB node, tree;
    if(hasEdge(g,v1,v2) == INFINITIVE_VALUE){
        node = jrb_find_int(g.edges, v1);
        if(node == NULL){
            tree = make_jrb();
            jrb_insert_int(g.edges, v1, new_jval_v(tree));
        } else tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
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
double shorstestPath(Graph g, int s ,int t, int* path, int *length){ //tu 's' toi 't'
    double distance[1000], min, w, total;
    int previous[1000], tmp[1000];
    int n, output[100], u, v, start;
    Dllist ptr, queue, node;
    
    for(int i = 0; i < 1000; i++)
        distance[i] = INFINITIVE_VALUE;
    distance[s] = 0;
    previous[s] = s;

    queue = new_dllist();
    dll_append(queue, new_jval_i(s));

    while(!dll_empty(queue)){
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue){
            u = jval_i(ptr->val);
            if(min > distance[u]){
                min = distance[u]; 
                node = ptr;
            }
        }
    dll_delete_node(node);
    if(u == t) break; //toi dich

    n = outdegree(g,u,output); //so canh ke cua u
    for(int i = 0; i < n; i++){
        v = output[i];
        w = getEdgeValue(g, u, v);
        if(distance[v] > distance[u] + w){
            distance[v] = distance[u] + w;
            previous[v] = u;
        }
        dll_append(queue, new_jval_i(v)); // tiep tuc di toi v de chon duong
    }
    }
    total = distance[t]; 
    if( total != INFINITIVE_VALUE){ //co duong di ngan nhat
        tmp[0] = t;
        n = 1;
        while(t != s){
            t = previous[t];
            tmp[n++] = t; //qua dinh nao
        }
        for(int i = n-1; i >= 0; i--)
            path[n-i-1] = tmp[i]; //dao lai duong di
        *length = n; //qua bao nhieu dinh
      }
    return total;
        
 }



