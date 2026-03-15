#include <iostream>
#include <vector>
using namespace std;

static const int_fast32_t MOD = 998244353;

struct Solver
{
    int_fast32_t n;
    vector<int_fast32_t> x;

    vector<vector<int_fast32_t>> memo_any;

    vector<vector<vector<int_fast32_t>>> memo_val;

    Solver(int_fast32_t n_, const vector<int_fast32_t> &x_) : n(n_), x(x_)
    {
        memo_any.assign(n + 2, vector<int_fast32_t>(n + 2, -2));
        memo_val.assign(n + 1, vector<vector<int_fast32_t>>(n + 2, vector<int_fast32_t>(n + 2, -2)));
    }

    inline int_fast32_t addmod(int_fast32_t a, int_fast32_t b)
    {
        a += b;
        if (a >= MOD)
            a -= MOD;
        return a;
    }

    inline int_fast32_t mulmod(long long a, long long b)
    {
        return int((a * b) % MOD);
    }

    int_fast32_t forest_with_parent(int_fast32_t p, int_fast32_t l, int_fast32_t r)
    {
        if (l > r)
            return 1;
        int_fast32_t &res = memo_val[p][l][r];
        if (res != -2)
            return res;
        long long ans = 0;

        if (x[l] != -1 && x[l] != p)
        {
            res = 0;
            return 0;
        }

        for (int_fast32_t m = l; m <= r; ++m)
        {
            int_fast32_t left = subtree(l, m);
            int_fast32_t right = forest_with_parent(p, m + 1, r);
            ans += 1LL * left * right % MOD;
            if (ans >= (1LL << 62))
                ans %= MOD;
        }
        res = int(ans % MOD);
        return res;
    }

    int_fast32_t forest_any(int_fast32_t l, int_fast32_t r)
    {
        if (l > r)
            return 1;
        int_fast32_t &res = memo_any[l][r];
        if (res != -2)
            return res;
        long long ans = 0;

        if (x[l] != -1)
        {
            res = 0;
            return 0;
        }

        for (int_fast32_t m = l; m <= r; ++m)
        {
            int_fast32_t left = subtree(l, m);
            int_fast32_t right = forest_any(m + 1, r);
            ans += 1LL * left * right % MOD;
            if (ans >= (1LL << 62))
                ans %= MOD;
        }
        res = int(ans % MOD);
        return res;
    }

    int_fast32_t subtree(int_fast32_t l, int_fast32_t r)
    {
        return forest_with_parent(l, l + 1, r);
    }

    int_fast32_t solve()
    {

        if (x[1] != -1 && x[1] != 0)
            return 0;

        return forest_with_parent(0, 1, n);
    }
};

int main()
{

    int_fast32_t t;
    cin >> t;
    while (t--)
    {
        int_fast32_t n;
        cin >> n;
        vector<int_fast32_t> x(n + 1);
        for (int_fast32_t i = 1; i <= n; ++i)
            cin >> x[i];

        bool bad = false;
        for (int_fast32_t i = 1; i <= n; ++i)
        {
            if (x[i] > n || x[i] < -1)
                bad = true;
            if (x[i] >= i)
                bad = true;
        }
        if (bad)
        {
            cout << 0 << '\n';
            continue;
        }

        Solver solver(n, x);
        cout << solver.solve() << '\n';
    }
    return 0;
}