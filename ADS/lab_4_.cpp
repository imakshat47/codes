#include <bits/stdc++.h>
using namespace std;

// loop counters
size_t bfs_cnt=0, dfs_cnt=0, djk_cnt=0, topo_cnt=0, fw_cnt=0;

// ─── 1) BFS ───
// prints the order of visit
void runBFS(const vector<vector<int>>& adj,int src){
    int n=adj.size();
    vector<bool> vis(n,false); bfs_cnt++;
    queue<int> q; bfs_cnt++;
    vis[src]=true; bfs_cnt++;
    q.push(src); bfs_cnt++;

    cout<<"BFS order: ";
    while(!q.empty()){
        int u=q.front(); q.pop(); bfs_cnt+=2;
        cout<<u<<" ";
        for(int v:adj[u]){ bfs_cnt++;
            if(!vis[v]){
                vis[v]=true; bfs_cnt++;
                q.push(v); bfs_cnt++;
            }
        }
    }
    cout<<"\nBFS loops="<<bfs_cnt<<", Θ(V+E) on list\n";
    // :contentReference[oaicite:0]{index=0}
}

// ─── 2) DFS ───
// prints the order of visit (recursive)
void dfsRec(const vector<vector<int>>& adj,int u,vector<bool>& vis){
    vis[u]=true; dfs_cnt++;
    cout<<u<<" ";                       // print on entry
    for(int v:adj[u]){ dfs_cnt++;
        if(!vis[v]) dfsRec(adj,v,vis), dfs_cnt++;
    }
}
void runDFS(const vector<vector<int>>& adj,int src){
    vector<bool> vis(adj.size(),false);
    cout<<"DFS order: ";
    dfsRec(adj,src,vis);
    cout<<"\nDFS loops="<<dfs_cnt<<", Θ(V+E) on list\n";
    // :contentReference[oaicite:1]{index=1}
}

// ─── 3) Dijkstra’s ───
// prints distances + reconstructs paths
void runDijkstra(const vector<vector<pair<int,int>>>& adj,int src){
    int n=adj.size(), INF=1e9;
    vector<int> dist(n,INF), parent(n,-1);
    dist[src]=0; djk_cnt++;
    using pii=pair<int,int>;
    priority_queue<pii,vector<pii>,greater<pii>> pq; djk_cnt++;
    pq.push({0,src}); djk_cnt++;

    while(!pq.empty()){
        pii top=pq.top(); pq.pop(); djk_cnt+=3;
        int d=top.first, u=top.second;
        if(d>dist[u]) continue;
        for(auto &e:adj[u]){ djk_cnt++;
            int v=e.first, w=e.second;
            if(d+w<dist[v]){
                dist[v]=d+w; parent[v]=u; djk_cnt++;
                pq.push({dist[v],v}); djk_cnt++;
            }
        }
    }
    cout<<"Dijkstra distances from "<<src<<":\n";
    for(int i=0;i<n;i++){
        cout<<"  to "<<i<<": "<<(dist[i]==INF?"∞":to_string(dist[i]))<<"\n";
    }
    // reconstruct each path
    cout<<"Dijkstra paths:\n";
    for(int i=0;i<n;i++){
        if(dist[i]==INF){
            cout<<"  "<<src<<"→"<<i<<": unreachable\n";
        } else {
            vector<int> path;
            for(int v=i; v!=-1; v=parent[v]) path.push_back(v);
            reverse(path.begin(),path.end());
            cout<<"  "<<src<<"→"<<i<<": ";
            for(int x:path) cout<<x<<(x==i?"\n":" ");
        }
    }
    cout<<"Dijkstra loops="<<djk_cnt<<", Θ((V+E) log V)\n";
    // :contentReference[oaicite:2]{index=2}
}

// ─── 4) Topological Sort ───
// prints one valid topo order or reports a cycle
void runTopo(const vector<vector<int>>& adj){
    int n=adj.size();
    vector<int> indeg(n,0); topo_cnt++;
    for(int u=0;u<n;++u){
        for(int v:adj[u]){ topo_cnt++;
            indeg[v]++; topo_cnt++;
        }
    }
    queue<int> q; topo_cnt++;
    for(int i=0;i<n;++i){ topo_cnt++;
        if(indeg[i]==0){ q.push(i); topo_cnt++; }
    }
    vector<int> order;
    while(!q.empty()){
        int u=q.front(); q.pop(); topo_cnt+=2;
        order.push_back(u);
        for(int v:adj[u]){ topo_cnt++;
            if(--indeg[v]==0){ q.push(v); topo_cnt++; }
        }
    }
    if(order.size()<n){
        cout<<"Topological Sort: graph has a cycle\n";
    } else {
        cout<<"Topological order: ";
        for(int x:order) cout<<x<<" ";
        cout<<"\n";
    }
    cout<<"TopoSort loops="<<topo_cnt<<", Θ(V+E)\n";
    // :contentReference[oaicite:3]{index=3}
}

// ─── 5) Floyd–Warshall ───
// prints distance matrix + next‐hop reconstruction
void runFloydWarshall(vector<vector<int>>& dist){
    int n=dist.size(), INF=1e9;
    vector<vector<int>> nxt(n,vector<int>(n,-1)); fw_cnt++;
    // initialize next-hop
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){ fw_cnt++;
            if(dist[i][j]<INF) nxt[i][j]=j;
        }
    }
    // main DP
    for(int k=0;k<n;++k){
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                fw_cnt++;
                if(dist[i][k]<INF && dist[k][j]<INF
                   && dist[i][k]+dist[k][j]<dist[i][j]){
                    dist[i][j]=dist[i][k]+dist[k][j];
                    nxt[i][j]=nxt[i][k];
                }
            }
        }
    }
    // print matrix
    cout<<"Floyd–Warshall distance matrix:\n";
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            if(dist[i][j]==INF) cout<<" INF";
            else cout<<setw(4)<<dist[i][j];
        }
        cout<<"\n";
    }
    // sample path print between all pairs
    cout<<"Floyd–Warshall sample paths:\n";
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            if(i==j||dist[i][j]==INF) continue;
            cout<<"  "<<i<<"→"<<j<<": ";
            int u=i;
            while(u!=j){
                cout<<u<<" ";
                u=nxt[u][j];
            }
            cout<<j<<"\n";
        }
    }
    cout<<"Floyd–Warshall loops="<<fw_cnt<<", Θ(V^3)\n";
    // :contentReference[oaicite:4]{index=4}
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m; 
    cin>>n>>m;  
    vector<vector<int>> adj1(n);
    vector<vector<pair<int,int>>> adj2(n);
    const int INF=1e9;
    vector<vector<int>> dist(n,vector<int>(n,INF));
    for(int i=0;i<n;++i) dist[i][i]=0;

    for(int i=0,u,v,w;i<m;++i){
        cin>>u>>v>>w;
        adj1[u].push_back(v);
        adj2[u].push_back({v,w});
        dist[u][v]=w;
    }

    runBFS(adj1,0);
    runDFS(adj1,0);
    runDijkstra(adj2,0);
    runTopo(adj1);
    runFloydWarshall(dist);

    return 0;
}
