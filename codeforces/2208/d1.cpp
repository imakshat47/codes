#include <iostream>
#include <vector>
#include<deque>
using namespace std;

int_fast32_t main()
{

    int_fast32_t T;
    if (!(cin >> T))
        return 0;
    while (T--)
    {
        int_fast32_t n;
        cin >> n;
        vector<string> s(n);
        for (int_fast32_t i = 0; i < n; ++i)
            cin >> s[i];

        bool ok = true;

        for (int_fast32_t i = 0; i < n && ok; ++i)
        {
            if (s[i][i] != '1')
                ok = false;
            for (int_fast32_t j = 0; j < n && ok; ++j)
                if (i != j)
                {
                    if (s[i][j] == '1' && s[j][i] == '1')
                        ok = false;
                }
        }
        if (!ok)
        {
            cout << "No\n";
            continue;
        }

        for (int_fast32_t i = 0; i < n && ok; ++i)
        {
            for (int_fast32_t j = 0; j < n && ok; ++j)
                if (s[i][j] == '1')
                {
                    for (int_fast32_t k = 0; k < n; ++k)
                        if (s[j][k] == '1' && s[i][k] == '0')
                        {
                            ok = false;
                            break;
                        }
                }
        }
        if (!ok)
        {
            cout << "No\n";
            continue;
        }

        vector<vector<int>> adj(n);
        vector<pair<int, int>> edges;
        for (int_fast32_t u = 0; u < n; ++u)
        {
            for (int_fast32_t v = 0; v < n; ++v)
                if (u != v && s[u][v] == '1')
                {
                    bool has_mid = false;
                    for (int_fast32_t w = 0; w < n; ++w)
                        if (w != u && w != v)
                        {
                            if (s[u][w] == '1' && s[w][v] == '1')
                            {
                                has_mid = true;
                                break;
                            }
                        }
                    if (!has_mid)
                    {

                        adj[u].push_back(v);
                        edges.emplace_back(u, v);
                    }
                }
        }

        if ((int)edges.size() != n - 1)
        {
            cout << "No\n";
            continue;
        }

        vector<vector<int>> und(n);
        for (auto &e : edges)
        {
            int_fast32_t u = e.first, v = e.second;
            und[u].push_back(v);
            und[v].push_back(u);
        }
        vector<char> vis(n, 0);
        int_fast32_t cnt = 0;
        auto dfsu = [&](auto &&self, int_fast32_t u) -> void
        {
            vis[u] = 1;
            cnt++;
            for (int_fast32_t w : und[u])
            {
                if (!vis[w])
                    self(self, w);
            }
        };

        dfsu(dfsu, 0);
        if (cnt != n)
        {
            cout << "No\n";
            continue;
        }

        vector<string> reach(n, string(n, '0'));
        for (int_fast32_t i = 0; i < n; ++i)
        {

            vector<char> seen(n, 0);
            deque<int> dq;
            dq.push_back(i);
            seen[i] = 1;
            while (!dq.empty())
            {
                int_fast32_t x = dq.front();
                dq.pop_front();
                reach[i][x] = '1';
                for (int_fast32_t y : adj[x])
                    if (!seen[y])
                    {
                        seen[y] = 1;
                        dq.push_back(y);
                    }
            }
        }

        if (reach != s)
        {
            cout << "No\n";
            continue;
        }

        cout << "Yes\n";
        for (auto &e : edges)
        {
            cout << e.first + 1 << " " << e.second + 1 << "\n";
        }
    }
    return 0;
}