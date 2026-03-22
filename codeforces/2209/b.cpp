#include <bits/stdc++.h>
using namespace std;

int_fast64_t fdiv2(int_fast64_t x)
{
    return x >= 0 ? x / 2 : (x - 1) / 2;
}

int_fast32_t main()
{

    int_fast32_t t;
    cin >> t;
    while (t--)
    {
        int_fast32_t n;
        cin >> n;
        vector<int_fast64_t> a(n);
        for (auto &x : a)
            cin >> x;

        for (int_fast32_t i = 0; i < n; i++)
        {

            vector<pair<int_fast64_t, int_fast32_t>> events;
            int_fast32_t lc = 0;

            for (int_fast32_t j = i + 1; j < n; j++)
            {
                if (a[j] < a[i])
                {

                    int_fast64_t u = fdiv2(a[i] + a[j] - 1);
                    events.push_back({u + 1, 0});
                    lc++;
                }
                else if (a[j] > a[i])
                {

                    int_fast64_t l = fdiv2(a[i] + a[j]) + 1;
                    events.push_back({l, 1});
                }
            }

            sort(events.begin(), events.end());

            int_fast32_t rc = 0;
            int_fast32_t best = lc;

            int_fast32_t ei = 0, m = (int_fast32_t)events.size();
            while (ei < m)
            {
                int_fast64_t cur = events[ei].first;

                while (ei < m && events[ei].first == cur)
                {
                    if (events[ei].second == 0)
                        lc--;
                    else
                        rc++;
                    ei++;
                }
                best = max(best, lc + rc);
            }

            cout << best;
            if (i < n - 1)
                cout << ' ';
        }
        cout << '\n';
    }
    return 0;
}