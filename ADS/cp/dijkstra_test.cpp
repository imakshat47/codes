// Dijkstra's Algorithm test
// Time Complexity: O((V + E) log V)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> pii;

void dijkstra(int start, vector<vector<pii>>& graph) {
    vector<int> dist(graph.size(), INT_MAX);
    dist[start] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : graph[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    for (int i = 0; i < dist.size(); ++i) cout << "Node " << i << ": " << dist[i] << endl;
    cout << endl;
}

int main() {
    vector<vector<vector<pii>>> test_cases = {
        {{{1,1},{2,4}},{{2,2}},{}},  // Easy
        {{{1,4},{2,1}},{{3,1}},{{1,2},{3,5}},{}}, // Medium
        {{{1,2},{2,3}},{{3,2}},{{4,1}},{{5,2}},{{5,2}},{{}} // Hard
    }
};

    for (auto& g : test_cases) dijkstra(0, g);
}
