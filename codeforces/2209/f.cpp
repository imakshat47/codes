#include <bits/stdc++.h>

using namespace std;

struct State
{
    int_fast32_t dist;
    int_fast32_t leaf;
};

State combine(State a, State b)
{
    if (a.dist > b.dist)
        return a;
    if (a.dist < b.dist)
        return b;
    return a.leaf < b.leaf ? a : b;
}

State add_edge(State a)
{
    return {a.dist + 1, a.leaf};
}

struct Edge
{
    int_fast32_t to;
    int_fast32_t id;
    bool is_forward;
};

struct TopKSum
{
    int_fast32_t k;
    int_fast64_t sum;
    multiset<int_fast64_t> top, rest;

    TopKSum(int_fast32_t k) : k(k), sum(0) {}

    void insert(int_fast64_t x)
    {
        if (k == 0)
        {
            rest.insert(x);
            return;
        }
        if (top.size() < k)
        {
            top.insert(x);
            sum += x;
        }
        else
        {
            int_fast64_t min_top = *top.begin();
            if (x > min_top)
            {
                top.erase(top.begin());
                sum -= min_top;
                rest.insert(min_top);
                top.insert(x);
                sum += x;
            }
            else
            {
                rest.insert(x);
            }
        }
    }

    void erase(int_fast64_t x)
    {
        if (k == 0)
        {
            rest.erase(rest.find(x));
            return;
        }
        auto it = rest.find(x);
        if (it != rest.end())
        {
            rest.erase(it);
        }
        else
        {
            top.erase(top.find(x));
            sum -= x;
            if (!rest.empty())
            {
                auto it_rest = prev(rest.end());
                int_fast64_t max_rest = *it_rest;
                rest.erase(it_rest);
                top.insert(max_rest);
                sum += max_rest;
            }
        }
    }
};

int_fast32_t n, k_op;
vector<int_fast64_t> a;
vector<vector<Edge>> adj;
vector<State> down_state;
vector<int_fast32_t> dest_dir;
vector<int_fast32_t> parent_edge_idx;
vector<int_fast32_t> deg;
vector<int_fast64_t> leaf_sum;
TopKSum *tks;
int_fast64_t max_ans;

void dfs1(int_fast32_t u, int_fast32_t p, int_fast32_t p_edge_idx = -1)
{
    parent_edge_idx[u] = p_edge_idx;
    down_state[u] = {-1, 1000000000};
    for (auto &e : adj[u])
    {
        if (e.to == p)
            continue;
        int_fast32_t dir_idx = e.is_forward ? (2 * e.id) : (2 * e.id + 1);
        dfs1(e.to, u, dir_idx);
        down_state[u] = combine(down_state[u], add_edge(down_state[e.to]));
    }
    if (down_state[u].dist == -1)
        down_state[u] = {0, u};
}

void dfs2(int_fast32_t u, int_fast32_t p, State up_state)
{
    for (auto &e : adj[u])
    {
        int_fast32_t v = e.to;
        int_fast32_t dir_idx = e.is_forward ? (2 * e.id) : (2 * e.id + 1);
        if (v == p)
            dest_dir[dir_idx] = up_state.leaf;
        else
            dest_dir[dir_idx] = down_state[v].leaf;
    }

    State m1 = {-1, 1000000000}, m2 = {-1, 1000000000};
    auto add_to_pool = [&](State s)
    {
        if (s.dist > m1.dist || (s.dist == m1.dist && s.leaf < m1.leaf))
        {
            m2 = m1;
            m1 = s;
        }
        else if (s.dist > m2.dist || (s.dist == m2.dist && s.leaf < m2.leaf))
        {
            m2 = s;
        }
    };

    if (p != 0)
        add_to_pool(up_state);
    if (deg[u] == 1)
        add_to_pool({0, u});
    for (auto &e : adj[u])
        if (e.to != p)
            add_to_pool(add_edge(down_state[e.to]));

    for (auto &e : adj[u])
    {
        int_fast32_t v = e.to;
        if (v != p)
        {
            State val_v = add_edge(down_state[v]);
            State best_for_v = (val_v.leaf == m1.leaf) ? m2 : m1;
            dfs2(v, u, add_edge(best_for_v));
        }
    }
}

void dfs3(int_fast32_t u, int_fast32_t p)
{
    max_ans = max(max_ans, a[u] + tks->sum);
    for (auto &e : adj[u])
    {
        int_fast32_t v = e.to;
        if (v == p)
            continue;

        int_fast32_t u_to_v = e.is_forward ? (2 * e.id) : (2 * e.id + 1);
        int_fast32_t v_to_u = e.is_forward ? (2 * e.id + 1) : (2 * e.id);

        int_fast32_t dest_v = dest_dir[u_to_v], dest_u = dest_dir[v_to_u];

        tks->erase(leaf_sum[dest_v]);
        leaf_sum[dest_v] -= a[v];
        tks->insert(leaf_sum[dest_v]);
        tks->erase(leaf_sum[dest_u]);
        leaf_sum[dest_u] += a[u];
        tks->insert(leaf_sum[dest_u]);

        dfs3(v, u);

        tks->erase(leaf_sum[dest_u]);
        leaf_sum[dest_u] -= a[u];
        tks->insert(leaf_sum[dest_u]);
        tks->erase(leaf_sum[dest_v]);
        leaf_sum[dest_v] += a[v];
        tks->insert(leaf_sum[dest_v]);
    }
}

void solve()
{
    cin >> n >> k_op;
    a.assign(n + 1, 0);
    for (int_fast32_t i = 1; i <= n; ++i)
        cin >> a[i];

    adj.assign(n + 1, vector<Edge>());
    deg.assign(n + 1, 0);

    for (int_fast32_t i = 0; i < n - 1; ++i)
    {
        int_fast32_t u, v;
        cin >> u >> v;
        adj[u].push_back({v, i, true});
        adj[v].push_back({u, i, false});
        deg[u]++;
        deg[v]++;
    }

    if (n == 1)
    {
        cout << a[1] << "\n";
        return;
    }

    down_state.assign(n + 1, State());
    dest_dir.assign(2 * n, 0);
    parent_edge_idx.assign(n + 1, -1);

    dfs1(1, 0);
    dfs2(1, 0, {-1, 1000000000});

    leaf_sum.assign(n + 1, 0);
    for (int_fast32_t i = 2; i <= n; ++i)
    {
        int_fast32_t d = dest_dir[parent_edge_idx[i]];
        leaf_sum[d] += a[i];
    }

    int_fast32_t L = 0;
    for (int_fast32_t i = 1; i <= n; ++i)
    {
        if (deg[i] == 1)
            L++;
    }

    int_fast32_t M = min(k_op - 1, L);
    tks = new TopKSum(M);

    for (int_fast32_t i = 1; i <= n; ++i)
    {
        if (deg[i] == 1)
            tks->insert(leaf_sum[i]);
    }

    max_ans = 0;
    dfs3(1, 0);

    cout << max_ans << "\n";
    delete tks;
}

int_fast32_t main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int_fast32_t t;
    cin >> t;
    while (t--)
        solve();
    return 0;
}