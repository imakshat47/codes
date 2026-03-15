#include<iostream>
#include<queue>
#include<list>
using namespace std;

int main() {
    int_fast32_t T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int_fast32_t n, k, p, m;
        cin >> n >> k >> p >> m;
        vector<int> a(n+1);
        for (int_fast32_t i = 1; i <= n; ++i) cin >> a[i];

        list<int> q;
        for (int_fast32_t i = 1; i <= n; ++i) q.push_back(i);

        int_fast32_t wins = 0;
        int_fast32_t budget = m;

        while (true) {
            // iterate first k elements (or fewer if q shorter)
            auto it = q.begin();
            bool found_target = false;
            list<int>::iterator target_it = q.end();
            int_fast32_t minCost = INT_MAX;
            list<int>::iterator minIt = q.end();

            int_fast32_t cnt = 0;
            for (auto jt = q.begin(); jt != q.end() && cnt < k; ++jt, ++cnt) {
                int_fast32_t idx = *jt;
                if (idx == p) {
                    found_target = true;
                    target_it = jt;
                }
                if (a[idx] < minCost) {
                    minCost = a[idx];
                    minIt = jt;
                }
            }

            if (found_target) {
                // can we afford playing the win-card?
                if (budget < a[p]) break;
                // play it
                budget -= a[p];
                ++wins;
                // move it to back
                q.erase(target_it);
                q.push_back(p);
            } else {
                // need to play cheapest among first k
                if (minIt == q.end()) break; // no available moves (shouldn't happen)
                if (budget < minCost) break;
                budget -= minCost;
                int_fast32_t idx = *minIt;
                q.erase(minIt);
                q.push_back(idx);
            }
        }

        cout << wins << '\n';
    }
    return 0;
}