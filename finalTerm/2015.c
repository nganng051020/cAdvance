#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dijkstra.h"

Graph g;
int n;
void File(char *d);

int main(int argc, char* argv[]){
    g = createGraph();
    int a, b, d;
    char c[20];
    

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("C-Advanced, HK20152\n");
        return 0;}

    if(argc >= 3){
        if(strcmp(argv[1], "-v") == 0){
        File(argv[2]);
        for(int i = 1; i <= n; i++)
            printf("%d %s\n", i, getVertex(g,i));
        }

        else if(strcmp(argv[1], "-w") == 0){
            printf("%.0lf\n", getEdgeValue(g,1,3));
        }
    }
    return 0;
}

void File(char *d){
     FILE *f;
     int a,b,m;
     double c;
     char s[200];
    f = fopen(d, "r");
    if(f == NULL) {
        printf("Error!\n");
        exit -1;}
    fscanf(f,"%d", &n );
    for(int i = 0; i < n; i++){
        fscanf(f,"%d %s", &a, s);
        addVertex(g, a, s);
    }
    fscanf(f,"%d", &m);
    for(int i = 0; i < m; i++){
        fscanf(f,"%d %d %lf", &a, &b, &c);
        addEdge(g, a, b, c);
    }
   fclose(f);
}
