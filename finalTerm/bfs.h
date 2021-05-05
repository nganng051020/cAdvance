#include<stdio.h>
#include<stdlib.h>
#define MAXN 1000
#include "jrb.h"
#include "dllist.h"

typedef JRB Graph;

Graph createGraph();
void addEdge(Graph graph, int v1, int v2);
int adjacent(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int vertex, int* output);
void dropGraph(Graph graph);
void BFS(Graph graph, int start, int stop, void (*func)(int)); //chieu rong
void DFS(Graph graph, int start, int stop, void (*func)(int)); //chieu sau


void print_vertex(int v){
	printf("%4d", v);
}


/*int main()
{
    int i, n, output[100];
    Graph g = createGraph();
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 2, 3);
    addEdge(g, 2, 4);
    addEdge(g, 4, 5);
    printf("DFS: ");
    DFS(g, 1, 4, print_vertex);
    printf("\nBFS: ");
    BFS(g, 1, 4, print_vertex);
    printf("\n");
    dropGraph(g);
}
*/
Graph createGraph()
{
   return make_jrb();
}

void addEdge(Graph graph, int v1, int v2)
{
     JRB node, tree;
     if (!adjacent(graph, v1, v2))
     {
        node = jrb_find_int(graph, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
     }
     if (!adjacent(graph, v2, v1))
     {
        node = jrb_find_int(graph, v2);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph, v2, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v1, new_jval_i(1));   
     }
}

int adjacent(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph, v1);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    if (jrb_find_int(tree, v2)==NULL)
       return 0;
    else
       return 1;       
}

int getAdjacentVertices (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph);
}

void BFS(Graph graph, int start, int stop, void (*func)(int)){
	Dllist node, queue;
	int visited[MAXN] = {};
	int neighbor_num, neighbor_list[MAXN], i, u, v;
	// Khoi tao va dua dinh xuat phat vao hang doi
	queue = new_dllist();
	dll_append(queue, new_jval_i(start));
	while(!dll_empty(queue)){
		// Tro vao phan tu dau tien trong hang doi
		node = dll_first(queue);
		u = jval_i(node->val);
		// Xoa phan tu khoi hang doi
		dll_delete_node(node);
		if(!visited[u]){
			func(u);
			visited[u] = 1;
			if (u == stop) return;
			neighbor_num = getAdjacentVertices(graph, u, neighbor_list);
			for(i = 0; i < neighbor_num; i++){
				v = neighbor_list[i];
				if(!visited[v])
					dll_append(queue, new_jval_i(v));
			}
		}		
	}
}


void DFS(Graph graph, int start, int stop, void (*func)(int)){
	Dllist node, stk;
	int visited[MAXN] = {};
	int neighbor_num, neighbor_list[MAXN], i, u, v;
	stk = new_dllist();
	dll_append(stk, new_jval_i(start));
	visited[start] = 1;
	while(!dll_empty(stk)){
		node = dll_last(stk); // Lay o cuoi hang
		u = jval_i(node->val);
		dll_delete_node(node);
		func(u);
		if (u == stop) return;
		neighbor_num = getAdjacentVertices(graph, u, neighbor_list);
		for(i = 0; i < neighbor_num; i++){
			v = neighbor_list[i];
			if(!visited[v]){
				dll_append(stk, new_jval_i(v));
				visited[v] = 1;
			}
		}			
	}

}
