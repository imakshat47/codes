#include <bits/stdc++.h>
using namespace std;

string computeS(int d) {
    string res = "";
    while (d > 9) {
        res += to_string(d);
        int s = 0, tmp = d;
        while (tmp > 0) { s += tmp % 10; tmp /= 10; }
        d = s;
    }
    res += (char)('0' + d);
    return res;
}

int computeG(int d) {
    // Sum of all digit values in S(d)
    int total = 0;
    while (d > 9) {
        int s = 0, tmp = d;
        while (tmp > 0) { s += tmp % 10; tmp /= 10; }
        total += s;
        d = s;
    }
    return total + d;
}

void solve() {
    string s;
    cin >> s;
    if (s.size() == 1) { cout << s << "\n"; return; }

    int freq[10] = {}, sum_s = 0;
    for (char c : s) { freq[c-'0']++; sum_s += c-'0'; }

    auto tryD = [&](int d) -> string {
        if (d <= 0) return "";
        string T = computeS(d);

        int freq2[10] = {};
        for (char c : T) freq2[c-'0']++;
        for (int i = 0; i < 10; i++) if (freq2[i] > freq[i]) return "";

        int rem[10], rc = 0;
        for (int i = 0; i < 10; i++) { rem[i] = freq[i]-freq2[i]; rc += rem[i]; }
        if (rc < 2) return "";  // x must be > 9

        int first = -1;
        for (int i = 9; i >= 1; i--) if (rem[i] > 0) { first = i; break; }
        if (first == -1) return "";

        string x(1, '0'+first); rem[first]--;
        for (int i = 9; i >= 0; i--) x.append(rem[i], '0'+i);
        return x + T;
    };

    // Case d <= 9: f(d) = 2d => d = sum_s/2
    if (sum_s % 2 == 0) {
        int d = sum_s / 2;
        if (d >= 1 && d <= 9) {
            string res = tryD(d);
            if (!res.empty()) { cout << res << "\n"; return; }
        }
    }

    // Case d >= 10: g(d) <= ~72 so d is near sum_s
    for (int d = max(10, sum_s - 100); d < sum_s; d++) {
        if (d + computeG(d) == sum_s) {
            string res = tryD(d);
            if (!res.empty()) { cout << res << "\n"; return; }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t; cin >> t;
    while (t--) solve();
    return 0;
}