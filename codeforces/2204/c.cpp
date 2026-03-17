#include <bits/stdc++.h>
#include <iostream>
#include <numeric>

using namespace std;

int_fast64_t gcd(int_fast64_t a, int_fast64_t b){
    while (b != 0) {
        int_fast32_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Safe LCM function to prevent overflow. 
// If the LCM exceeds m, we return m + 1 so that m / LCM safely becomes 0.
int_fast64_t safe_lcm(int_fast64_t a, int_fast64_t b, int_fast64_t m) {
    if (a == 0 || b == 0) return m + 1; 
    
    int_fast64_t g = gcd(a, b);
    int_fast64_t a_div_g = a / g;
    
    // Check if (a / g) * b > m without causing overflow
    if (m / b < a_div_g) {
        return m + 1; 
    }
    return a_div_g * b;
}

void solve() {
    int_fast64_t a, b, c, m;
    cin >> a >> b >> c >> m;

    // 1. Calculate the LCMs for pairs and all three
    int_fast64_t l_ab = safe_lcm(a, b, m);
    int_fast64_t l_bc = safe_lcm(b, c, m);
    int_fast64_t l_ac = safe_lcm(a, c, m);
    int_fast64_t l_abc = safe_lcm(l_ab, c, m);

    // 2. Total days each individual/group combo could potentially meet
    int_fast64_t n_a = m / a;
    int_fast64_t n_b = m / b;
    int_fast64_t n_c = m / c;

    int_fast64_t n_ab = m / l_ab;
    int_fast64_t n_bc = m / l_bc;
    int_fast64_t n_ac = m / l_ac;
    int_fast64_t n_abc = m / l_abc;

    // 3. Apply Inclusion-Exclusion to find EXACT group compositions
    int_fast64_t exact_abc = n_abc;
    int_fast64_t exact_ab = n_ab - exact_abc;
    int_fast64_t exact_bc = n_bc - exact_abc;
    int_fast64_t exact_ac = n_ac - exact_abc;

    int_fast64_t exact_a = n_a - exact_ab - exact_ac - exact_abc;
    int_fast64_t exact_b = n_b - exact_ab - exact_bc - exact_abc;
    int_fast64_t exact_c = n_c - exact_ac - exact_bc - exact_abc;

    // 4. Multiply by the water allocations
    // Alone = 6, Pair = 3, Trio = 2
    int_fast64_t water_a = exact_a * 6 + exact_ab * 3 + exact_ac * 3 + exact_abc * 2;
    int_fast64_t water_b = exact_b * 6 + exact_ab * 3 + exact_bc * 3 + exact_abc * 2;
    int_fast64_t water_c = exact_c * 6 + exact_ac * 3 + exact_bc * 3 + exact_abc * 2;

    cout << water_a << " " << water_b << " " << water_c << "\n";
}

int_fast32_t main() {
    // Fast I/O is necessary since there are up to 10^4 test cases
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int_fast32_t t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}