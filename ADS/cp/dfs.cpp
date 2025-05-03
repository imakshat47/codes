// dfs_test.cpp
// DFS: O(V + E)

#include <iostream>
#include <vector>
using namespace std;

void dfs(int u, const vector<vector<int>>& adj, vector<bool>& vis) {
    vis[u] = true;
    cout << u << ' ';
    for (int v : adj[u]) {
        if (!vis[v]) dfs(v, adj, vis);
    }
}

void run_dfs(int start, const vector<vector<int>>& adj) {
    vector<bool> vis(adj.size(), false);
    dfs(start, adj, vis);
    cout << '\n';
}

int main() {
    // Easy
    {
        vector<vector<int>> adj = {{1}, {2}, {}};
        cout << "DFS Easy: "; run_dfs(0, adj); // 0 1 2
    }
    // Medium
    {
        vector<vector<int>> adj = {{1,2}, {3}, {4}, {}, {}};
        cout << "DFS Medium: "; run_dfs(0, adj); // 0 1 3 2 4
    }
    // Hard
    {
        vector<vector<int>> adj = {{1,2}, {4}, {5}, {}, {6}, {6}, {7}, {}};
        cout << "DFS Hard: "; run_dfs(0, adj); // 0 1 4 6 7 2 5
    }
    return 0;
}
