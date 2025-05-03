// BFS test cases
// Time Complexity: O(V + E)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(int start, vector<vector<int>>& adj, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        cout << node << " ";
        for (int neigh : adj[node]) {
            if (!visited[neigh]) {
                visited[neigh] = true;
                q.push(neigh);
            }
        }
    }
    cout << endl;
}

int main() {
    vector<vector<vector<int>>> test_cases = {
        {{1}, {2}, {}},                 // Easy
        {{1,2}, {3}, {3}, {4}, {}},     // Medium
        {{1,2}, {4}, {5}, {}, {6}, {6}, {7}, {}} // Hard
    };

    for (auto& adj : test_cases) {
        vector<bool> visited(adj.size(), false);
        bfs(0, adj, visited);
    }
}
