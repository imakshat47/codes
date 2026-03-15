// #include <bits/stdc++.h>
#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

int main()
{
    int_fast32_t T;
    if (!(cin >> T))
        return 0;
    cout.setf(std::ios::fixed);
    cout <<setprecision(10);
    while (T--)
    {
        int_fast32_t n;
        cin >> n;
        vector<int_fast32_t> c(n);
        vector<int_fast32_t> p(n);
        for (int_fast32_t i = 0; i < n; ++i)
            cin >> c[i] >> p[i];
        double nextF = 0.0; // F(i+1)
        for (int_fast32_t i = n - 1; i >= 0; --i)
        {
            double q = (100 - p[i]) / 100.0;
            double take = c[i] + q * nextF;
            double skip = nextF;
            double curF = max(skip, take);
            nextF = curF;
        }
        cout << nextF << '\n';
    }
    return 0;
}