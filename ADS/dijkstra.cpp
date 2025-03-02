#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

// Dijkstra's Algorithm using Priority Queue (Min-Heap)
void dijkstra(int source, vector<vector<pair<int, int>>> &adj, vector<int> &dist) {
    int V = adj.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue; // Skip outdated distance values

        for (auto &[v, weight] : adj[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    int V, E; // Number of vertices and edges
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    vector<vector<pair<int, int>>> adj(V); // Adjacency list (u -> {v, weight})
    vector<int> dist(V, INF); // Stores shortest distances

    cout << "Enter edges (u v weight):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // Uncomment for undirected graph
    }

    int source;
    cout << "Enter source vertex: ";
    cin >> source;

    dijkstra(source, adj, dist);

    cout << "Shortest distances from source " << source << ":" << endl;
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF)
            cout << "Vertex " << i << ": INF" << endl;
        else
            cout << "Vertex " << i << ": " << dist[i] << endl;
    }

    return 0;
}
