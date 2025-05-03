// DFS test cases
// Time Complexity: O(V + E)

#include <iostream>
#include <vector>
using namespace std;

void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited) {
    visited[node] = true;
    cout << node << " ";
    for (int neigh : adj[node]) {
        if (!visited[neigh]) dfs(neigh, adj, visited);
    }
}

int main() {
    vector<vector<vector<int>>> test_cases = {
        {{1}, {2}, {}},                 // Easy
        {{1,2}, {3}, {4}, {}, {}},     // Medium
        {{1,2}, {4}, {5}, {}, {6}, {6}, {7}, {}} // Hard
    };

    for (auto& adj : test_cases) {
        vector<bool> visited(adj.size(), false);
        dfs(0, adj, visited);
        cout << endl;
    }
}
