// Graph Algorithms: BFS, DFS, Dijkstra, Topological Sort, Floyd-Warshall, Bellman-Ford
// C++11 Implementation with test cases and comments
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <tuple>
#include <algorithm>
using namespace std;

const int INF = numeric_limits<int>::max();

// ----------------------------- BFS -----------------------------
// Time Complexity: O(V + E)
void BFS(int start, const vector<vector<int>>& adj) {
    vector<bool> visited(adj.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS starting from vertex " << start << ": ";
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    cout << endl;
}

// ----------------------------- DFS -----------------------------
// Time Complexity: O(V + E)
void DFSUtil(int u, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    cout << u << " ";
    for (int v : adj[u]) {
        if (!visited[v]) DFSUtil(v, adj, visited);
    }
}

void DFS(int start, const vector<vector<int>>& adj) {
    vector<bool> visited(adj.size(), false);
    cout << "DFS starting from vertex " << start << ": ";
    DFSUtil(start, adj, visited);
    cout << endl;
}

// ------------------------ Dijkstra's Algorithm ------------------------
// Time Complexity: O((V + E) log V) with min-heap
void Dijkstra(int src, const vector<vector<pair<int, int>>>& adj) {
    int V = adj.size();
    vector<int> dist(V, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, src});
    dist[src] = 0;

    while (!pq.empty()) {
        int u = pq.top().second; pq.pop();
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    cout << "Dijkstra from " << src << ": ";
    for (int d : dist) cout << (d == INF ? -1 : d) << " ";
    cout << endl;
}

// ------------------------ Topological Sort ------------------------
// Time Complexity: O(V + E)
void TopoSortDFS(int u, const vector<vector<int>>& adj, vector<bool>& visited, stack<int>& st) {
    visited[u] = true;
    for (int v : adj[u]) if (!visited[v]) TopoSortDFS(v, adj, visited, st);
    st.push(u);
}

void TopologicalSort(const vector<vector<int>>& adj) {
    vector<bool> visited(adj.size(), false);
    stack<int> st;
    for (int i = 0; i < adj.size(); ++i) if (!visited[i]) TopoSortDFS(i, adj, visited, st);
    cout << "Topological Order: ";
    while (!st.empty()) { cout << st.top() << " "; st.pop(); }
    cout << endl;
}

// ------------------------ Floyd-Warshall Algorithm ------------------------
// Time Complexity: O(V^3)
void FloydWarshall(vector<vector<int>>& dist) {
    int V = dist.size();
    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    cout << "Floyd-Warshall All-Pairs Shortest Paths:\n";
    for (auto& row : dist) {
        for (int d : row) cout << (d == INF ? -1 : d) << " ";
        cout << endl;
    }
}

// ------------------------ Bellman-Ford Algorithm ------------------------
// Time Complexity: O(V * E)
void BellmanFord(int V, int E, int src, vector<tuple<int, int, int>>& edges) {
    vector<int> dist(V, INF);
    dist[src] = 0;
    for (int i = 1; i < V; ++i) {
        for (auto& [u, v, w] : edges)
            if (dist[u] != INF && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
    }
    cout << "Bellman-Ford from " << src << ": ";
    for (int d : dist) cout << (d == INF ? -1 : d) << " ";
    cout << endl;
}

// ------------------------ Test Driver ------------------------
int main() {
    vector<vector<int>> adj1 = {{1, 2}, {3}, {3}, {}};
    vector<vector<int>> adj2 = {{1}, {2}, {0, 3}, {}};
    vector<vector<pair<int, int>>> weighted = {
        {{1, 4}, {2, 1}}, {{3, 1}}, {{1, 2}, {3, 5}}, {}};

    // Test BFS and DFS
    BFS(0, adj1);
    DFS(0, adj1);
    BFS(0, adj2);
    DFS(0, adj2);

    // Test Dijkstra
    Dijkstra(0, weighted);

    // Test Topological Sort
    TopologicalSort(adj1);
    TopologicalSort(adj2);

    // Test Floyd-Warshall
    vector<vector<int>> dist = {
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0,   1},
        {INF, INF, INF, 0}
    };
    FloydWarshall(dist);

    // Test Bellman-Ford
    vector<tuple<int, int, int>> edges = {
        {0, 1, 4}, {0, 2, 5}, {1, 2, -3}, {2, 3, 4}
    };
    BellmanFord(4, 4, 0, edges);

    return 0;
}
