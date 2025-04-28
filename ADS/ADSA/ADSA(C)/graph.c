#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX 100


int iteration_counter;


// BFS Implementation
void BFS(int graph[MAX][MAX], int vertices, int start) {
    int queue[MAX], front = 0, rear = 0, visited[MAX] = {0};
    queue[rear++] = start;
    visited[start] = 1;
    iteration_counter = 0;
    
    while (front < rear) {
        int node = queue[front++];
        printf("%d ", node);
        iteration_counter++;
        
        for (int i = 0; i < vertices; i++) {
            if (graph[node][i] && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\nBFS Iterations: %d\n", iteration_counter);
}


// DFS Implementation
void DFSUtil(int graph[MAX][MAX], int vertices, int node, int visited[MAX]) {
    visited[node] = 1;
    printf("%d ", node);
    iteration_counter++;
    
    for (int i = 0; i < vertices; i++) {
        if (graph[node][i] && !visited[i]) {
            DFSUtil(graph, vertices, i, visited);
        }
    }
}


void DFS(int graph[MAX][MAX], int vertices, int start) {
    int visited[MAX] = {0};
    iteration_counter = 0;
    DFSUtil(graph, vertices, start, visited);
    printf("\nDFS Iterations: %d\n", iteration_counter);
}


// Dijkstra's Algorithm
void Dijkstra(int graph[MAX][MAX], int vertices, int start) {
    int dist[MAX], visited[MAX] = {0};
    iteration_counter = 0;
    
    for (int i = 0; i < vertices; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    
    for (int count = 0; count < vertices - 1; count++) {
        int min = INT_MAX, u;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v], u = v;
            }
        }
        
        visited[u] = 1;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
            iteration_counter++;
        }
    }
    
    printf("Dijkstra's Algorithm Iterations: %d\n", iteration_counter);
}


// Topological Sort
void TopologicalSortUtil(int graph[MAX][MAX], int vertices, int v, int visited[MAX], int stack[MAX], int *index) {
    visited[v] = 1;
    
    for (int i = 0; i < vertices; i++) {
        if (graph[v][i] && !visited[i]) {
            TopologicalSortUtil(graph, vertices, i, visited, stack, index);
        }
        iteration_counter++;
    }
    stack[(*index)--] = v;
}


void TopologicalSort(int graph[MAX][MAX], int vertices) {
    int visited[MAX] = {0}, stack[MAX], index = vertices - 1;
    iteration_counter = 0;
    
    for (int i = 0; i < vertices; i++) {
        if (!visited[i]) {
            TopologicalSortUtil(graph, vertices, i, visited, stack, &index);
        }
    }
    
    printf("Topological Order: ");
    for (int i = 0; i < vertices; i++) {
        printf("%d ", stack[i]);
    }
    printf("\nTopological Sort Iterations: %d\n", iteration_counter);
}


// Floyd-Warshall Algorithm
void FloydWarshall(int graph[MAX][MAX], int vertices) {
    int dist[MAX][MAX];
    iteration_counter = 0;
    
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            dist[i][j] = graph[i][j];
        }
    }
    
    for (int k = 0; k < vertices; k++) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
                iteration_counter++;
            }
        }
    }
    
    printf("Floyd-Warshall Algorithm Iterations: %d\n", iteration_counter);
}


int main() {
    int graph[MAX][MAX] = { {0, 1, 0, 0, 0, 1},
                             {1, 0, 1, 0, 0, 0},
                             {0, 1, 0, 1, 1, 0},
                             {0, 0, 1, 0, 1, 0},
                             {0, 0, 1, 1, 0, 1},
                             {1, 0, 0, 0, 1, 0} };
    int vertices = 6;
    
    printf("Breadth-First Search:\n");
    BFS(graph, vertices, 0);
    
    printf("\nDepth-First Search:\n");
    DFS(graph, vertices, 0);
    
    printf("\nDijkstra's Algorithm:\n");
    Dijkstra(graph, vertices, 0);
    
    printf("\nTopological Sort:\n");
    TopologicalSort(graph, vertices);
    
    printf("\nFloyd-Warshall Algorithm:\n");
    FloydWarshall(graph, vertices);
    
    return 0;
}