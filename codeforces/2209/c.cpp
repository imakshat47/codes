#include <bits/stdc++.h>

using namespace std;

int query(int i, int j)
{
    cout << "? " << i << " " << j << "\n";
    cout.flush();
    int r;
    cin >> r;
    if (r == -1)
    {

        exit(0);
    }
    return r;
}

void answer(int k)
{
    cout << "! " << k << "\n";
    cout.flush();
}

void solve()
{
    int n;
    cin >> n;

    if (query(1, 2) == 1)
    {
        answer(1);
        return;
    }
    if (query(2, 3) == 1)
    {
        answer(2);
        return;
    }
    if (query(1, 3) == 1)
    {
        answer(1);
        return;
    }

    vector<int> candidates;
    for (int i = 4; i <= 2 * n; ++i)
    {
        candidates.push_back(i);
    }

    while (candidates.size() > 1)
    {
        int u = candidates.back();
        candidates.pop_back();
        int v = candidates.back();
        candidates.pop_back();

        if (query(u, v) == 1)
        {
            answer(u);
            return;
        }
    }

    answer(candidates[0]);
}

int main()
{

    int t;
    if (cin >> t)
    {
        while (t--)
        {
            solve();
        }
    }
    return 0;
}