// dijkstra_test.cpp
// Dijkstra: O((V+E) log V)

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;
using pii = pair<int,int>;
const int INF = numeric_limits<int>::max();

void dijkstra(int src, const vector<vector<pii>>& adj) {
    int V = adj.size();
    vector<int> dist(V, INF);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d,u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v,w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    for (int i = 0; i < V; ++i)
        cout << (dist[i]==INF ? -1 : dist[i]) << (i+1<V?' ':'\n');
}

int main() {
    // Easy
    {
        vector<vector<pii>> adj = {
            {{1,1},{2,4}}, {{2,2}}, {}
        };
        cout << "Dijkstra Easy: "; dijkstra(0, adj); // 0 1 3
    }
    // Medium
    {
        vector<vector<pii>> adj = {
            {{1,4},{2,1}}, {{3,1}}, {{1,2},{3,5}}, {}
        };
        cout << "Dijkstra Medium: "; dijkstra(0, adj); // 0 3 1 4
    }
    // Hard
    {
        vector<vector<pii>> adj = {
            {{1,3},{2,2}}, {{3,5},{4,1}}, {{3,2}}, {{5,1}}, {{5,2}}, {}
        };
        cout << "Dijkstra Hard: "; dijkstra(0, adj); // 0 3 2 4 4 5
    }
    return 0;
}
