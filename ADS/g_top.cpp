
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <tuple>
#include <algorithm>
using namespace std;

const int INF = numeric_limits<int>::max();

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

int main() {
    vector<vector<int>> adj1 = {{1, 2}, {3}, {3}, {}};
    vector<vector<int>> adj2 = {{1}, {2}, {0, 3}, {}};

    // Test Topological Sort
    TopologicalSort(adj1);
    TopologicalSort(adj2);

    return 0;
}
