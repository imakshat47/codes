#include <bits/stdc++.h>
using namespace std;
using ll = int_fast64_t;

int_fast32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int_fast32_t t;
    if (!(cin >> t)) return 0;
    while (t--) {
        ll r,g,b;
        cin >> r >> g >> b;
        // store as pairs (count, char)
        vector<pair<ll,char>> cnt = {{r,'R'}, {g,'G'}, {b,'B'}};
        string s;
        s.reserve(r+g+b);
        while (true) {
            // sort descending by remaining count
            sort(cnt.begin(), cnt.end(), [](const pair<ll,char>& a, const pair<ll,char>& b){
                return a.first > b.first;
            });
            bool placed = false;
            for (int_fast32_t i = 0; i < 3; ++i) {
                if (cnt[i].first == 0) continue;
                char c = cnt[i].second;
                // check last and third-last constraint_fast32_ts
                if (!s.empty() && s.back() == c) continue;
                if ((int_fast32_t)s.size() >= 3 && s[(int_fast32_t)s.size()-3] == c) continue;
                // place this color
                s.push_back(c);
                cnt[i].first--;
                placed = true;
                break;
            }
            if (!placed) break;
        }
        cout << s << '\n';
    }
    return 0;
}
