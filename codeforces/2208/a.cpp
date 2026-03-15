#include<iostream>
#include<unordered_map>
using namespace std;

int main() {
    uint_fast16_t t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int_fast32_t n;
        cin >> n;
        unordered_map<int_fast32_t,int_fast32_t> freq;
        freq.reserve(n * n * 2);
        for (int_fast32_t i = 0; i < n; ++i) {
            for (int_fast32_t j = 0; j < n; ++j) {
                int_fast32_t x; cin >> x;
                ++freq[x];
            }
        }
        int_fast64_t limit = 1LL * n * (n - 1);
        bool ok = true;
        for (auto &p : freq) {
            if (p.second > limit) { ok = false; break; }
        }
        cout << (ok ? "YES\n" : "NO\n");
    }
    return 0;
}