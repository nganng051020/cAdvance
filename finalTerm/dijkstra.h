
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXN 1000
#include "jrb.h"
#include "dllist.h"

#define INFINITIVE_VALUE  10000000


typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
void dropGraph(Graph graph);
double shortestPath(Graph graph, int s, int t, int* path, int*length);
void BFS(Graph graph, int start, int stop, void (*func)(int)); //chieu rong
void DFS(Graph graph, int start, int stop, void (*func)(int)); //chieu sau
void print_vertex(int v){
	printf("%4d", v);
}
int DAG(Graph graph);

/*int main()
{
    int i, length, path[100], s, t;
    double w;
    Graph g = createGraph();
    addVertex(g, 0, "V0");
    addVertex(g, 1, "V1");
    addVertex(g, 2, "V2");
    addVertex(g, 3, "V3");
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 3);
    addEdge(g, 2, 0, 3);
    addEdge(g, 1, 3, 1);
    addEdge(g, 3, 2, 1);
    s = 0;
    t = 2;
    w = shortestPath(g, s, t, path, &length);
    if (w == INFINITIVE_VALUE)
    {
         printf("No path from %s to %s\n", getVertex(g, s), getVertex(g, t));
    }else
    {
         printf("Path from %s to %s (with total distance %f)\n", getVertex(g, s), getVertex(g, t), w);
         for (i=0; i<length; i++)
             printf(" => %s", getVertex(g, path[i]));
    }
    
    dropGraph(g);
    getch();   
}
 */

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

void addVertex(Graph g, int id, char* name)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) // only add new vertex 
         jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));            
}

char *getVertex(Graph g, int id)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}     

void addEdge(Graph graph, int v1, int v2, double weight)
{
     JRB node, tree;
     if (getEdgeValue(graph, v1, v2)== -1)
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
     }
}

double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return -1;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return -1;
    else
       return jval_d(node->val);       
}

int indegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph.edges)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}

int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
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



double shortestPath(Graph g, int s, int t, int* path, int*length)
{
   double distance[1000], min, w, total;
   int previous[1000], tmp[1000];
   int n, output[100], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<1000; i++)
       distance[i] = INFINITIVE_VALUE;
   distance[s] = 0;
   previous[s] = s;
       
   queue = new_dllist();
   dll_append(queue, new_jval_i(s));

   while ( !dll_empty(queue) )
   {
      // get u from the priority queue   
      min = INFINITIVE_VALUE;   
      dll_traverse(ptr, queue)
      {
          u = jval_i(ptr->val);              
          if (min > distance[u])
          {
             min = distance[u];
             node = ptr;
          }                 
      }
      u = jval_i(node->val);
      dll_delete_node(node);
      
      if (u == t) break; // stop at t
      
      n = outdegree(g, u, output);
      for (i=0; i<n; i++)
      {
          v = output[i];
          w = getEdgeValue(g, u, v);
          if ( distance[v] > distance[u] + w )
          {    
              distance[v] = distance[u] + w;
              previous[v] = u;
              dll_append(queue, new_jval_i(v));
          }     
          
      }
   }
   total = distance[t]; 
   if (total != INFINITIVE_VALUE)
   {
       tmp[0] = t;
       n = 1;              
       while (t != s)
       {
             t = previous[t];
             tmp[n++] = t;
       }
       for (i=n-1; i>=0; i--)
           path[n-i-1] = tmp[i];
       *length = n;                
   }
   return total;   
}



void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
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
			neighbor_num = indegree(graph, u, neighbor_list);
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
		neighbor_num = indegree(graph, u, neighbor_list);
		for(i = 0; i < neighbor_num; i++){
			v = neighbor_list[i];
			if(!visited[v]){
				dll_append(stk, new_jval_i(v));
				visited[v] = 1;
			}
		}			
	}

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



