#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#define V 5


// Bellman-Ford Algorithm
void bellmanFord(int graph[][3], int edges, int vertices, int src) {
    int dist[V];
    for (int i = 0; i < vertices; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    for (int i = 1; i < vertices; i++) {
        for (int j = 0; j < edges; j++) {
            int u = graph[j][0];
            int v = graph[j][1];
            int w = graph[j][2];
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }

    for (int j = 0; j < edges; j++) {
        int u = graph[j][0];
        int v = graph[j][1];
        int w = graph[j][2];
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }

    printf("Bellman-Ford shortest distances from vertex %d:\n", src);
    for (int i = 0; i < vertices; i++)
        printf("%d -> %d = %d\n", src, i, dist[i]);
}

// ------------------------ MAIN ------------------------
int main() {
    int graph[V][V] = {
        {0, 3, INT_MAX, 7, INT_MAX},
        {3, 0, 1, INT_MAX, INT_MAX},
        {INT_MAX, 1, 0, 2, 3},
        {7, INT_MAX, 2, 0, 2},
        {INT_MAX, INT_MAX, 3, 2, 0}
    };

    int bellmanGraph[7][3] = {
        {0, 1, -1}, {0, 2, 4}, {1, 2, 3},
        {1, 3, 2}, {1, 4, 2}, {3, 2, 5}, {4, 3, -3}
    };


    bellmanFord(bellmanGraph, 7, V, 0);

    return 0;
}
