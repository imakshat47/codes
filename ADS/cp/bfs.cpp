// bfs_test.cpp
// BFS: O(V + E)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(int start, const vector<vector<int>>& adj) {
    vector<bool> vis(adj.size(), false);
    queue<int> q;
    q.push(start);
    vis[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cout << u << ' ';
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    cout << '\n';
}

int main() {
    // Easy: simple chain 0-1-2
    {
        vector<vector<int>> adj = {{1}, {2}, {}};
        cout << "BFS Easy: "; bfs(0, adj); // 0 1 2
    }
    // Medium: branching
    {
        vector<vector<int>> adj = {{1,2}, {3}, {3}, {4}, {}};
        cout << "BFS Medium: "; bfs(0, adj); // 0 1 2 3 4
    }
    // Hard: more levels
    {
        vector<vector<int>> adj = {{1,2}, {4}, {5}, {}, {6}, {6}, {7}, {}};
        cout << "BFS Hard: "; bfs(0, adj); // 0 1 2 4 5 6 7
    }
    return 0;
}
