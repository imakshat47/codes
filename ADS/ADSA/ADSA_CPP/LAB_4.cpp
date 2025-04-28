#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
using namespace std;

const int MAX = 100;
int iteration_counter = 0;

// BFS
void BFS(vector< vector<int> > & graph, int vertices, int start) {
    vector<bool> visited(vertices, false);
    queue<int> q;
    q.push(start);
    visited[start] = true;
    iteration_counter = 0;

    cout << "BFS Traversal: ";
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";
        iteration_counter++;

        for (int i = 0; i < vertices; ++i) {
            if (graph[node][i] && !visited[i]) {
                q.push(i);
                visited[i] = true;
            }
        }
    }
    cout << "\nBFS Iterations: " << iteration_counter << "\n";
}

// DFS
void DFSUtil(vector< vector<int> > & graph, int vertices, int node, vector<bool>& visited) {
    visited[node] = true;
    cout << node << " ";
    iteration_counter++;

    for (int i = 0; i < vertices; ++i) {
        if (graph[node][i] && !visited[i]) {
            DFSUtil(graph, vertices, i, visited);
        }
    }
}

void DFS(vector< vector<int> > & graph, int vertices, int start) {
    vector<bool> visited(vertices, false);
    iteration_counter = 0;

    cout << "DFS Traversal: ";
    DFSUtil(graph, vertices, start, visited);
    cout << "\nDFS Iterations: " << iteration_counter << "\n";
}

// Dijkstra
void Dijkstra(vector< vector<int> > & graph, int vertices, int start) {
    vector<int> dist(vertices, INT_MAX);
    vector<bool> visited(vertices, false);
    dist[start] = 0;
    iteration_counter = 0;

    for (int count = 0; count < vertices - 1; ++count) {
        int min = INT_MAX, u = -1;

        for (int v = 0; v < vertices; ++v) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < vertices; ++v) {
            if (!visited[v] && graph[u][v] &&
                dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
            iteration_counter++;
        }
    }

    cout << "Dijkstra's Shortest Distances from Source " << start << ":\n";
    for (int i = 0; i < vertices; ++i) {
        cout << "To " << i << ": " << dist[i] << "\n";
    }
    cout << "Dijkstra's Iterations: " << iteration_counter << "\n";
}

// Topological Sort (Using DFS)
void TopoSortUtil(vector< vector<int> > & graph, int v, vector<bool>& visited, stack<int>& Stack) {
    visited[v] = true;

    for (int i = 0; i < graph.size(); ++i) {
        if (graph[v][i] && !visited[i]) {
            TopoSortUtil(graph, i, visited, Stack);
        }
        iteration_counter++;
    }

    Stack.push(v);
}

void TopologicalSort(vector< vector<int> > & graph, int vertices) {
    stack<int> Stack;
    vector<bool> visited(vertices, false);
    iteration_counter = 0;

    for (int i = 0; i < vertices; ++i) {
        if (!visited[i]) {
            TopoSortUtil(graph, i, visited, Stack);
        }
    }

    cout << "Topological Order: ";
    while (!Stack.empty()) {
        cout << Stack.top() << " ";
        Stack.pop();
    }
    cout << "\nTopological Sort Iterations: " << iteration_counter << "\n";
}

// Floyd–Warshall
void FloydWarshall(vector< vector<int> > & graph, int vertices) {
    vector< vector<int> > dist = graph;
    iteration_counter = 0;

    for (int k = 0; k < vertices; ++k) {
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
                iteration_counter++;
            }
        }
    }

    cout << "Floyd–Warshall Shortest Distance Matrix:\n";
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (dist[i][j] == INT_MAX)
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "Floyd–Warshall Iterations: " << iteration_counter << "\n";
}

int main() {
    int vertices = 6;
    vector< vector<int> > graph = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0} };

    cout << "Breadth First Search:\n";
    BFS(graph, vertices, 0);

    cout << "\nDepth First Search:\n";
    DFS(graph, vertices, 0);

    cout << "\nDijkstra's Algorithm:\n";
    Dijkstra(graph, vertices, 0);

    cout << "\nTopological Sort (Only valid for DAGs):\n";
    TopologicalSort(graph, vertices);

    cout << "\nFloyd–Warshall Algorithm:\n";
    // Replace 0s with INT_MAX for non-edges
    vector< vector<int> > fw_graph(vertices, vector<int> (vertices, INT_MAX));
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i == j) fw_graph[i][j] = 0;
            else if (graph[i][j]) fw_graph[i][j] = graph[i][j];
        }
    }
    FloydWarshall(fw_graph, vertices);

    return 0;
}
