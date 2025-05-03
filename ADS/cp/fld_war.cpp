// floyd_warshall_test.cpp
// Floyd–Warshall: O(V^3)

#include <iostream>
#include <vector>
#include <limits>
using namespace std;
const int INF = numeric_limits<int>::max()/2;

void floydWarshall(vector<vector<int>>& d) {
    int V = d.size();
    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    for (auto& row : d) {
        for (int x : row)
            cout << (x>=INF? -1 : x) << ' ';
        cout << '\n';
    }
}

int main() {
    // Easy
    {
        vector<vector<int>> d = {
            {0,1,INF},
            {INF,0,1},
            {INF,INF,0}
        };
        cout << "FW Easy:\n"; floydWarshall(d);
    }
    // Medium
    {
        vector<vector<int>> d = {
            {0,5,INF,10},
            {INF,0,3,INF},
            {INF,INF,0,1},
            {INF,INF,INF,0}
        };
        cout << "FW Medium:\n"; floydWarshall(d);
    }
    // Hard
    {
        vector<vector<int>> d(6, vector<int>(6, INF));
        for (int i=0;i<6;i++) d[i][i]=0;
        d[0][1]=4; d[1][2]=-2; d[2][3]=3;
        d[3][4]=2; d[4][5]=-1; // no negative cycles
        cout << "FW Hard:\n"; floydWarshall(d);
    }
    return 0;
}
