#include<stdio.h>
#include<stdio.h>
#include<string.h>
#include"dijkstra.h"

int main(){
    int i, n, output[100];
    Graph g = createGraph();
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 0, 1);
    addEdge(g, 1, 2, 1);
    addEdge(g, 2, 1, 1);
    addEdge(g, 1, 3, 1);
    addEdge(g, 3, 1, 1);
    addEdge(g, 2, 3, 1);
    addEdge(g, 3, 2, 1);
    addEdge(g, 2, 4, 1);
    addEdge(g, 4, 5, 1);

    printf("DFS: ");
    DFS(g, 1, 4, print_vertex);
    printf("\nBFS: ");
    BFS(g, 1, 4, print_vertex);
    printf("\n");
    dropGraph(g);
}