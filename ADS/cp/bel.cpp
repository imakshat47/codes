// bellman_ford_test.cpp
// Bellman-Ford: O(V * E)

#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
using namespace std;
const int INF = numeric_limits<int>::max();

void bellmanFord(int V, int src, vector<tuple<int,int,int>>& edges) {
    vector<int> dist(V, INF);
    dist[src]=0;
    for (int i=1;i<V;++i)
        for (auto& e: edges) {
            int u,v,w; tie(u,v,w)=e;
            if (dist[u]!=INF && dist[u]+w<dist[v])
                dist[v]=dist[u]+w;
        }
    // check negative cycle
    bool negCycle=false;
    for (auto& e: edges) {
        int u,v,w; tie(u,v,w)=e;
        if (dist[u]!=INF && dist[u]+w<dist[v])
            negCycle=true;
    }
    if (negCycle) {
        cout<<"Negative cycle detected\n";
    } else {
        for (int d: dist) cout<<(d==INF?-1:d)<<' ';
        cout<<'\n';
    }
}

int main() {
    // Easy
    {
        vector<tuple<int,int,int>> edges={{0,1,5},{1,2,3}};
        cout<<"BF Easy: "; bellmanFord(3,0,edges); // 0 5 8
    }
    // Medium
    {
        vector<tuple<int,int,int>> edges={{0,1,4},{0,2,5},{1,2,-2}};
        cout<<"BF Medium: "; bellmanFord(3,0,edges); // 0 4 2
    }
    // Hard
    {
        vector<tuple<int,int,int>> edges={{0,1,1},{1,2,-1},{2,0,-1}};
        cout<<"BF Hard: "; bellmanFord(3,0,edges); // Negative cycle
    }
    return 0;
}
