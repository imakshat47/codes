#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits.h>
using namespace std;

class Graph {
    int V;
    vector<vector<pair<int, int>>> adj; // Adjacency list for weighted graph

public:
    Graph(int v) : V(v) { adj.resize(v); }

    void addEdge(int u, int v, int weight = 1, bool directed = false) {
        adj[u].push_back({v, weight});
        if (!directed) adj[v].push_back({u, weight});
    }

    // BFS for shortest path in unweighted graph
    void BFS(int src) {
        vector<int> dist(V, INT_MAX);
        queue<int> q;
        dist[src] = 0;
        q.push(src);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [v, _] : adj[u]) {
                if (dist[v] == INT_MAX) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        
        cout << "BFS Shortest Paths from " << src << ":\n";
        for (int i = 0; i < V; i++)
            cout << "Node " << i << " -> Distance: " << dist[i] << endl;
    }

    // DFS traversal
    void DFS(int src, vector<bool> &visited) {
        cout << src << " ";
        visited[src] = true;
        for (auto [v, _] : adj[src]) {
            if (!visited[v]) DFS(v, visited);
        }
    }
    void DFS(int src) {
        vector<bool> visited(V, false);
        cout << "DFS Traversal: ";
        DFS(src, visited);
        cout << endl;
    }

    // Dijkstra's algorithm for weighted graph
    void dijkstra(int src) {
        vector<int> dist(V, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto [v, weight] : adj[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Dijkstra Shortest Paths from " << src << ":\n";
        for (int i = 0; i < V; i++)
            cout << "Node " << i << " -> Distance: " << dist[i] << endl;
    }

    // Topological Sort for DAG
    void topologicalSort() {
        vector<int> in_degree(V, 0);
        for (int u = 0; u < V; u++)
            for (auto [v, _] : adj[u])
                in_degree[v]++;

        queue<int> q;
        for (int i = 0; i < V; i++)
            if (in_degree[i] == 0)
                q.push(i);

        cout << "Topological Sort: ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";
            for (auto [v, _] : adj[u]) {
                if (--in_degree[v] == 0)
                    q.push(v);
            }
        }
        cout << endl;
    }

    // Floyd-Warshall Algorithm (All-pairs shortest path)
    void floydWarshall() {
        vector<vector<int>> dist(V, vector<int>(V, INT_MAX));
        for (int i = 0; i < V; i++) dist[i][i] = 0;
        for (int u = 0; u < V; u++)
            for (auto [v, w] : adj[u])
                dist[u][v] = w;

        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }

        cout << "Floyd-Warshall Shortest Paths:\n";
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][j] == INT_MAX) cout << "INF ";
                else cout << dist[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    Graph g(V);

    cout << "Enter number of edges: ";
    cin >> E;
    cout << "Enter " << E << " edges (u v weight):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g.addEdge(u, v, w, true); // Directed graph
    }

    int src;
    cout << "Enter source vertex: ";
    cin >> src;

    g.BFS(src);
    g.DFS(src);
    g.dijkstra(src);
    g.topologicalSort();
    g.floydWarshall();
    
    return 0;
}
