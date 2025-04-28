#include <bits/stdc++.h>
using namespace std;

// Global counters for each algorithm
size_t bfs_loops = 0, dfs_loops = 0, dijk_loops = 0, topo_loops = 0, fw_loops = 0;

// 1) BFS using adjacency matrix (unweighted shortest-path traversal) 
void runBFS(const vector<vector<int>>& adj, int src) {
    int n = adj.size();
    vector<bool> vis(n,false);
    queue<int> q;
    vis[src] = true;  bfs_loops++;
    q.push(src);     bfs_loops++;
    while (!q.empty()) {
        int u = q.front(); q.pop();  bfs_loops+=2;
        // scan all neighbors  
        for (int v = 0; v < n; ++v, ++bfs_loops) {  
            if (!vis[v] && adj[u][v]) {
                vis[v] = true; ++bfs_loops;
                q.push(v);   ++bfs_loops;
            }
        }
    }
    cout << "BFS loops: " << bfs_loops << ", Θ(n^2) on matrix\n";
}

// 2) DFS using adjacency matrix (recursive) 
void dfsRec(const vector<vector<int>>& adj, int u, vector<bool>& vis) {
    vis[u] = true;  dfs_loops++;
    for (int v = 0; v < (int)adj.size(); ++v, ++dfs_loops) {
        if (!vis[v] && adj[u][v]) {
            dfsRec(adj,v,vis); ++dfs_loops;
        }
    }
}
void runDFS(const vector<vector<int>>& adj, int src) {
    vector<bool> vis(adj.size(),false);
    dfsRec(adj,src,vis);
    cout << "DFS loops: " << dfs_loops << ", Θ(n^2) on matrix\n";
}

// 3) Dijkstra’s algorithm (simple O(n^2) version on adjacency matrix, nonnegative weights) 
void runDijkstra(const vector<vector<int>>& adj, int src) {
    int n = adj.size();
    const int INF = 1e9;
    vector<int> dist(n,INF), vis(n,0);
    dist[src] = 0;  dijk_loops++;
    for (int i = 0; i < n; ++i, ++dijk_loops) {
        // find min unvisited
        int u=-1, best=INF;  dijk_loops+=2;
        for (int v = 0; v < n; ++v, ++dijk_loops) {
            if (!vis[v] && dist[v]<best) { best=dist[v]; u=v; ++dijk_loops; }
        }
        if (u<0) break; vis[u]=1; ++dijk_loops;
        // relax edges
        for (int v = 0; v < n; ++v, ++dijk_loops) {
            if (!vis[v] && adj[u][v] && dist[u]+adj[u][v]<dist[v]) {
                dist[v] = dist[u]+adj[u][v]; dijk_loops++;
            }
        }
    }
    cout << "Dijkstra loops: " << dijk_loops << ", Θ(n^2)\n";
}

// 4) Topological Sort (Kahn’s algorithm with in-degree; only if DAG) 
void runTopo(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> indeg(n,0);
    // compute in-degree
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v, ++topo_loops) {
            if (adj[u][v]) indeg[v]++, topo_loops++;
        }
    }
    queue<int> q;
    for (int i = 0; i < n; ++i,++topo_loops) {
        if (indeg[i]==0) q.push(i), topo_loops++;
    }
    int cnt=0;
    while (!q.empty()) {
        int u = q.front(); q.pop(); ++topo_loops;
        ++cnt;
        for (int v = 0; v < n; ++v, ++topo_loops) {
            if (adj[u][v] && --indeg[v]==0) q.push(v), topo_loops++;
        }
    }
    // if cnt<n, graph had cycle
    cout << "Topological Sort loops: " << topo_loops << ", Θ(n^2)\n";
}

// 5) Floyd–Warshall (all-pairs shortest paths) :contentReference[oaicite:0]{index=0}
void runFloydWarshall(vector<vector<int>>& dist) {
    int n = dist.size();
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                ++fw_loops;
                if (dist[i][k]<1e9 && dist[k][j]<1e9 && dist[i][k]+dist[k][j]<dist[i][j]) {
                    dist[i][j] = dist[i][k]+dist[k][j];
                }
            }
        }
    }
    cout << "Floyd–Warshall loops: " << fw_loops << ", Θ(n^3)\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;  
    cin >> n;                                           // number of vertices
    vector<vector<int>> adj(n, vector<int>(n));
    for (int i = 0; i < n; ++i)                         // read adjacency (0 = no edge; w>0 = weight)
        for (int j = 0; j < n; ++j)
            cin >> adj[i][j];

    // Run each algorithm (using node 0 as source for BFS/DFS/Dijkstra)
    runBFS(adj, 0);                                     // 
    runDFS(adj, 0);                                     // 
    runDijkstra(adj, 0);                                // 
    runTopo(adj);                                       // 

    // For Floyd–Warshall we need a separate dist matrix
    vector<vector<int>> dist = adj;
    const int INF = 1e9;
    for (int i = 0; i < n; ++i)                         // set 0→i self distances to 0
        if (dist[i][i]==0) dist[i][i]=0;
        else if (dist[i][i]==0) dist[i][i]=0;
    for (int i = 0; i < n; ++i)                         // ensure no-edge uses INF
        for (int j = 0; j < n; ++j)
            if (i!=j && dist[i][j]==0) dist[i][j]=INF;

    runFloydWarshall(dist);                             // :contentReference[oaicite:1]{index=1}

    return 0;
}
