#include <bits/stdc++.h>
using namespace std;

int_fast32_t main() {

    int_fast32_t t;
    cin >> t;

    while (t--) {
        int_fast32_t n;
        string s;
        cin >> n >> s;

        vector<int_fast32_t> visited(n + 1, 0);

        int_fast32_t cur = 1;
        visited[cur] = 1;

        for (int_fast32_t step = 0; step < n; step++) {
            if (s[cur - 1] == 'L')
                cur--;
            else
                cur++;

            visited[cur] = 1;
        }

        int_fast32_t ans = 0;
        for (int_fast32_t i = 1; i <= n; i++)
            ans += visited[i];

        cout << ans << "\n";
    }

    return 0;
}