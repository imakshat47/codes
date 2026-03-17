#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

typedef int_fast64_t ll;
const int_fast32_t MOD = 998244353;

ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

ll modInverse(ll n) {
    return power(n, MOD - 2);
}

struct Element {
    int_fast32_t a;
    ll count;
};

bool compareElements(const Element& a, const Element& b) {
    return a.a < b.a;
}

int_fast32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int_fast32_t n, m;
    cin >> n >> m;

    vector<int_fast32_t> a(n + 1);
    for (int_fast32_t i = 1; i <= n; ++i) cin >> a[i];

    vector<int_fast32_t> L(n + 1), R(n + 1);
    stack<int_fast32_t> s;

    // Monotonic stack to find range [L_i, R_i] where a_i is the unique minimum
    for (int_fast32_t i = 1; i <= n; ++i) {
        while (!s.empty() && a[s.top()] > a[i]) s.pop();
        L[i] = s.empty() ? 0 : s.top();
        s.push(i);
    }
    while (!s.empty()) s.pop();
    for (int_fast32_t i = n; i >= 1; --i) {
        while (!s.empty() && a[s.top()] >= a[i]) s.pop();
        R[i] = s.empty() ? n + 1 : s.top();
        s.push(i);
    }

    ll C = 0;
    ll P1 = 0;
    vector<Element> elements;
    for (int_fast32_t i = 1; i <= n; ++i) {
        ll cnt = (1LL * (i - L[i]) * (R[i] - i)) % MOD;
        ll inv_a = modInverse(a[i]);
        
        // Constant part: sum of initial fractions over all subarrays
        ll sub_cnt = (1LL * i * (n - i + 1)) % MOD;
        C = (C + sub_cnt * inv_a) % MOD;
        
        P1 = (P1 + cnt * inv_a) % MOD;
        elements.push_back({a[i], cnt});
    }

    sort(elements.begin(), elements.end(), compareElements);

    vector<int_fast32_t> k_vals(m);
    for (int_fast32_t i = 0; i < m; ++i) cin >> k_vals[i];

    ll P2 = 0, P3 = 0, P4 = 0;
    int_fast32_t ptr = 0;
    for (int_fast32_t k : k_vals) {
        while (ptr < n && elements[ptr].a <= k + 1) {
            ll a_val = elements[ptr].a % MOD;
            ll cnt = elements[ptr].count;
            ll inv_a = modInverse(elements[ptr].a);
            
            P2 = (P2 + cnt * inv_a) % MOD;
            P3 = (P3 + cnt) % MOD;
            P4 = (P4 + cnt * (2 - a_val + MOD)) % MOD;
            ptr++;
        }

        // Apply derived formula for S(k)
        ll term_k = (1LL * k * (P1 - P2 + P3 + MOD + MOD)) % MOD;
        ll ans = (C + term_k + P4 - P2 + MOD) % MOD;
        cout << ans << "\n";
    }

    return 0;
}