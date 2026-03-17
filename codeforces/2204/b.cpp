#include <bits/stdc++.h>
using namespace std;

int_fast32_t main() {

    int_fast32_t t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int_fast32_t n; cin >> n;
        vector<int_fast32_t> a(n+1);
        for (int_fast32_t i = 1; i <= n; ++i) cin >> a[i];

        vector<int_fast32_t> lastPos(n+1);
        int_fast32_t pmax = INT_MIN;
        for (int_fast32_t i = 1; i <= n; ++i) {
            if (a[i] >= pmax) { // >= to keep rightmost occurrence
                pmax = a[i];
                lastPos[i] = i;
            } else {
                lastPos[i] = lastPos[i-1];
            }
        }

        int_fast32_t ops = 0;
        int_fast32_t cur = n;
        while (cur > 0) {
            int_fast32_t pos = lastPos[cur];
            ++ops;
            cur = pos - 1;
        }
        cout << ops << '\n';
    }
    return 0;
}