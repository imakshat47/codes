// topo_sort_test.cpp
// Topological Sort: O(V + E)

#include <iostream>
#include <vector>
#include <stack>
using namespace std;

void dfsTopo(int u, const vector<vector<int>>& adj, vector<bool>& vis, stack<int>& st) {
    vis[u] = true;
    for (int v : adj[u]) if (!vis[v]) dfsTopo(v, adj, vis, st);
    st.push(u);
}

void topoSort(const vector<vector<int>>& adj) {
    int V = adj.size();
    vector<bool> vis(V,false);
    stack<int> st;
    for (int i = 0; i < V; ++i)
        if (!vis[i]) dfsTopo(i, adj, vis, st);
    while (!st.empty()) {
        cout << st.top() << ' ';
        st.pop();
    }
    cout << '\n';
}

int main() {
    // Easy
    {
        vector<vector<int>> adj = {{1}, {2}, {}};
        cout << "Topo Easy: "; topoSort(adj); // 0 1 2
    }
    // Medium
    {
        vector<vector<int>> adj = {
            /*0*/{}, /*1*/{}, /*2*/{3}, /*3*/{1}, /*4*/{0,1}, /*5*/{0,2}
        };
        cout << "Topo Medium: "; topoSort(adj); // e.g. 5 4 2 3 1 0
    }
    // Hard
    {
        vector<vector<int>> adj(8);
        adj[5]={2,0}; adj[4]={0,1};
        adj[2]={3}; adj[3]={1}; // nodes 6,7 isolated
        cout << "Topo Hard: "; topoSort(adj); // one valid order
    }
    return 0;
}
