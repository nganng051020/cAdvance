#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dijkstra.h"

int n, m, id[10], id1, id2;
double weight;
char IP[30];
Graph g;

void readfile(char* filename);

int main(int argc, char* argv[]){
    g = createGraph();

    if(argc == 2 && strcmp(argv[1], "-h") == 0)
        printf("\nC-Advanced, HK20152\n\n");

    if(argc == 3 && strcmp(argv[1], "-v") == 0){
        readfile(argv[2]);
        printf("\n");
        for(int i = 0; i < n; i++)
            printf("%d %s\n", id[i], getVertex(g, id[i]));
        printf("\n");
    }

    if(argc == 5 && strcmp(argv[1], "-w") == 0){
        readfile(argv[2]);
        printf("\n%.0lf\n\n", getEdgeValue(g, atoi(argv[3]), atoi(argv[4])));
    }

    if(argc == 5 && strcmp(argv[1], "-p") == 0){
        readfile(argv[2]);
        int x, leng, path[10];
        x = shortestPath(g, atoi(argv[3]), atoi(argv[4]), path, &leng);
        printf("%d\n", x);
        for( int i = 0; i < leng; i++){
            printf("%s\n", getVertex(g, path[i]));
        }
        printf("\n");
    }

    if(argc == 4 && strcmp(argv[1], "-n") == 0){
        readfile(argv[2]);
        int output[10];
        int x;
        x = outdegree(g, atoi(argv[3]), output);
        printf("%d\n", x);
        for(int i = 0; i < x; i++)
            printf("%d %s\n", output[i], getVertex(g, output[i]));
    }

    if(argc == 5 && strcmp(argv[1], "-s") == 0){
        readfile(argv[2]);
        
    }
    return 0;
}

void readfile(char* filename){
    FILE *f;

    f = fopen(filename, "r");
    fscanf(f, "%d", &n);
   // id = (int*)malloc(n* sizeof(int));
    for(int i = 0; i < n; i++){
        fscanf(f, "%d %s", &id[i], IP);
        addVertex(g, id[i], IP);
    }
    fscanf(f, "%d", &m);
    for(int i = 0; i < m; i++){
        fscanf(f, "%d %d %lf", &id1, &id2, &weight);
        addEdge(g, id1, id2, weight);
        addEdge(g, id2, id1, weight);
    }
    fclose(f);
}