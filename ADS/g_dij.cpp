
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <tuple>
#include <algorithm>
using namespace std;

const int INF = numeric_limits<int>::max();


// ------------------------ Dijkstra's Algorithm ------------------------
// Time Complexity: O((V + E) log V) with min-heap
void Dijkstra(int src, const vector<vector<pair<int, int>>>& adj) {
    int V = adj.size();
    vector<int> dist(V, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > pq;
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


// ------------------------ Test Driver ------------------------
int main() {
    vector<vector<pair<int, int>>> weighted = {
        {{1, 4}, {2, 1}}, {{3, 1}}, {{1, 2}, {3, 5}}, {}};
    // Test Dijkstra
    Dijkstra(0, weighted);

    return 0;
}
