#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<int> z_function(const string &s) {
    int n = s.size();
    vector<int> z(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r)
            z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    z[0] = n;
    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, q;
        cin >> n >> q;

        string s;
        cin >> s;

        while (q--) {
            int l, r;
            cin >> l >> r;
            l--; r--;

            string t = s.substr(l, r - l + 1);

            auto z = z_function(t);

            int m = t.size();

            vector<int> dp(m);

            dp[0] = 1;

            for (int i = 1; i < m; i++) {
                int jump = z[i];
                if (jump == 0) dp[i] = 1;
                else dp[i] = dp[i - jump] + 1;
            }

            ll ans = 0;
            for (int i = 0; i < m; i++) ans += dp[i];

            cout << ans << '\n';
        }
    }

    return 0;
}