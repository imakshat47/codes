#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n; cin >> n;
        vector<string> s(n);
        for (int i = 0; i < n; ++i) cin >> s[i];

        // quick basic checks (diagonal and mutual)
        bool ok = true;
        for (int i = 0; i < n && ok; ++i) {
            if (s[i][i] != '1') ok = false;
            for (int j = i+1; j < n && ok; ++j) {
                if (s[i][j] == '1' && s[j][i] == '1') ok = false;
            }
        }
        if (!ok) { cout << "No\n"; continue; }

        const int W = (n + 63) >> 6;            // words per row
        // flat storage: rows_flat[u*W + w] gives word w of row u
        vector<ull> rows_flat((size_t)n * W, 0);
        vector<int> cnt(n, 0);

        for (int i = 0; i < n; ++i) {
            int base = i * W;
            for (int j = 0; j < n; ++j) if (s[i][j] == '1') {
                int w = j >> 6;
                rows_flat[base + w] |= (1ULL << (j & 63));
                ++cnt[i];
            }
        }

        vector<pair<int,int>> edges;
        edges.reserve(n-1);

        // per-parent processing
        vector<int> candidates;
        candidates.reserve(n);
        vector<ull> union_sel; union_sel.resize(W);

        for (int u = 0; u < n && ok; ++u) {
            candidates.clear();
            int base_u = u * W;
            // build list L: v != u and s[u][v]=='1'
            for (int v = 0; v < n; ++v) if (v != u && s[u][v] == '1') candidates.push_back(v);
            // sort by cnt desc (bigger sets first)
            sort(candidates.begin(), candidates.end(), [&](int a, int b){
                if (cnt[a] != cnt[b]) return cnt[a] > cnt[b];
                return a < b;
            });

            // target = rows[u] with u bit cleared
            int wu = u >> 6, bu = u & 63;
            // compute target implicitly by checking against rows_flat[base_u] and ignoring u bit when comparing

            // reset union_sel
            for (int w = 0; w < W; ++w) union_sel[w] = 0ULL;

            // remaining count estimate: track number of 1-bits remaining in target (optional optimization)
            // compute initial remaining bits = cnt[u] - 1 (since target excludes u)
            int remaining = cnt[u] - 1;

            for (int v : candidates) {
                if (remaining <= 0) break;
                // test if rows[v] adds any bit not yet in union_sel AND in target
                int base_v = v * W;
                bool contributes = false;
                // loop words
                for (int w = 0; w < W; ++w) {
                    // target word = rows_flat[base_u + w], with u bit zeroed if w==wu
                    ull targ = rows_flat[base_u + w];
                    if (w == wu) targ &= ~(1ULL << bu);
                    // missing = targ & ~union_sel
                    ull missing = targ & ~union_sel[w];
                    if (!missing) continue;      // nothing remaining here
                    // now check if rows[v][w] intersects missing
                    ull inter = rows_flat[base_v + w] & missing;
                    if (inter) { contributes = true; break; }
                }
                if (contributes) {
                    // accept v as child: update union_sel and remaining (use popcount on intersection)
                    edges.emplace_back(u, v);
                    for (int w = 0; w < W; ++w) {
                        ull targ = rows_flat[base_u + w];
                        if (w == wu) targ &= ~(1ULL << bu);
                        ull add = rows_flat[base_v + w] & targ & ~union_sel[w];
                        if (add) {
                            remaining -= __builtin_popcountll(add);
                            union_sel[w] |= add;
                        }
                    }
                }
            }

            if (remaining != 0) { ok = false; break; }  // couldn't cover target
        }

        if (!ok || (int)edges.size() != n - 1) { cout << "No\n"; continue; }

        // undirected connectivity check (fast BFS)
        vector<vector<int>> und(n);
        for (auto &e : edges) {
            und[e.first].push_back(e.second);
            und[e.second].push_back(e.first);
        }
        vector<char> seen(n, 0);
        int qh = 0, qt = 0;
        vector<int> q(n);
        q[qt++] = 0; seen[0] = 1;
        while (qh < qt) {
            int x = q[qh++]; 
            for (int y : und[x]) if (!seen[y]) { seen[y] = 1; q[qt++] = y; }
        }
        if (qt != n) { cout << "No\n"; continue; }

        // final verification: directed closure matches s
        vector<vector<int>> adj(n);
        for (auto &e : edges) adj[e.first].push_back(e.second);

        bool match = true;
        vector<char> vis(n);
        vector<int> q2; q2.reserve(n);
        for (int st = 0; st < n && match; ++st) {
            q2.clear();
            fill(vis.begin(), vis.end(), 0);
            q2.push_back(st); vis[st] = 1;
            for (size_t qi = 0; qi < q2.size(); ++qi) {
                int x = q2[qi];
                for (int y : adj[x]) if (!vis[y]) { vis[y] = 1; q2.push_back(y); }
            }
            for (int j = 0; j < n; ++j) {
                char want = s[st][j];
                char got = vis[j] ? '1' : '0';
                if (want != got) { match = false; break; }
            }
        }
        if (!match) { cout << "No\n"; continue; }

        cout << "Yes\n";
        for (auto &e : edges) cout << e.first + 1 << ' ' << e.second + 1 << '\n';
    }
    return 0;
}