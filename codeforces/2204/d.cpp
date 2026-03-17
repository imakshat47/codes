#include <bits/stdc++.h>
using namespace std;

int_fast32_t main() {

    int_fast32_t T; 
    if (!(cin >> T)) return 0;
    while (T--) {
        int_fast32_t n, m;
        cin >> n >> m;
        vector<vector<int_fast32_t>> adj(n+1);
        for (int_fast32_t i = 0; i < m; ++i) {
            int_fast32_t u, v; cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<int_fast32_t> color(n+1, -1); // -1 = unvisited, 0/1 = bipartite colors
        long long ans = 0;

        for (int_fast32_t s = 1; s <= n; ++s) {
            if (color[s] != -1) continue;
            // BFS this component
            deque<int_fast32_t> dq;
            dq.push_back(s);
            color[s] = 0;
            int_fast32_t cnt0 = 0, cnt1 = 0;
            bool is_bip = true;
            while (!dq.empty()) {
                int_fast32_t u = dq.front(); dq.pop_front();
                if (color[u] == 0) ++cnt0; else ++cnt1;
                for (int_fast32_t v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        dq.push_back(v);
                    } else {
                        if (color[v] == color[u]) {
                            is_bip = false;
                            // still continue to mark reachable nodes to avoid re-visiting
                        }
                    }
                }
            }
            if (is_bip) ans += max(cnt0, cnt1);
            // else add 0
        }

        cout << ans << '\n';
    }
    return 0;
}