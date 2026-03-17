#include <bits/stdc++.h>
#include <iostream>
#include <vector>

using namespace std;

// 2m + 1 will be at most 301
const int MAX_SIZE = 305;

struct Matrix {
    int mat[MAX_SIZE][MAX_SIZE];
    int size;
    Matrix(int sz) {
        size = sz;
        for (int i = 0; i < sz; ++i)
            for (int j = 0; j < sz; ++j)
                mat[i][j] = 0;
    }
};

// Highly optimized Matrix Multiplication
Matrix multiply(const Matrix &A, const Matrix &B, int mod) {
    Matrix C(A.size);
    static int B_T[MAX_SIZE][MAX_SIZE];
    int sz = A.size;
    
    // Transpose B beforehand to optimize cache access inside the inner loop
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            B_T[j][i] = B.mat[i][j];
        }
    }
    
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            unsigned long long sum = 0;
            for (int k = 0; k < sz; ++k) {
                sum += 1ULL * A.mat[i][k] * B_T[j][k];
                // Do modulo every 16 iterations to prevent unsigned long long overflow
                // Extremely fast bitwise check: (k % 16 == 15)
                if ((k & 15) == 15) {
                    sum %= mod;
                }
            }
            C.mat[i][j] = sum % mod;
        }
    }
    return C;
}

// Binary Exponentiation for Matrices
Matrix power(Matrix A, long long p, int mod) {
    Matrix res(A.size);
    for (int i = 0; i < A.size; ++i) res.mat[i][i] = 1; // Identity matrix
    while (p > 0) {
        if (p & 1) res = multiply(res, A, mod);
        A = multiply(A, A, mod);
        p >>= 1;
    }
    return res;
}

void solve() {
    long long n;
    int m, mod;
    if (!(cin >> n >> m >> mod)) return;

    int S = 2 * m + 1;
    Matrix M(S);
    
    // 1. Build Transitions for p_new[R]
    for (int R = 1; R <= m; ++R) {
        int i = R - 1;
        for (int K = 1; K <= R - 1; ++K) 
            M.mat[i][K - 1] = K % mod;
        for (int K = R; K <= m; ++K) 
            M.mat[i][K - 1] = R % mod;
        for (int K = R + 1; K <= m; ++K) 
            M.mat[i][m + K - 1] = (mod - (R % mod)) % mod;
    }
    
    // 2. Build Transitions for s_new[L]
    for (int L = 1; L <= m; ++L) {
        int i = m + L - 1;
        for (int K = L; K <= m; ++K) 
            M.mat[i][K - 1] = (m - L + 1) % mod;
        for (int K = L + 1; K <= m; ++K) 
            M.mat[i][m + K - 1] = (mod - ((K - L) % mod)) % mod;
    }
    
    // 3. Build Accumulator for the Total Paths (Ans)
    for (int j = 0; j < 2 * m; ++j) {
        long long sum = 0;
        for (int R = 1; R <= m; ++R) {
            sum += M.mat[R - 1][j];
        }
        M.mat[2 * m][j] = sum % mod;
    }
    M.mat[2 * m][2 * m] = 1; // Preserve previously accumulated Ans

    // 4. Initial State Vector setup (for k = 1)
    vector<long long> V(S, 0);
    for (int i = 0; i < m; ++i) {
        V[i] = 1 % mod;
    }
    V[m] = m % mod;          // s[1][1] = m
    V[2 * m] = m % mod;      // Initial accumulated Ans for row 1

    // If n is 1, return the base path state directly
    if (n == 1) {
        cout << V[2 * m] << "\n";
        return;
    }

    // Evaluate Mn = M^(n-1)
    Matrix Mn = power(M, n - 1, mod);
    
    // Calculate Final accumulative Answer
    long long final_ans = 0;
    for (int j = 0; j < S; ++j) {
        final_ans = (final_ans + 1LL * Mn.mat[2 * m][j] * V[j]) % mod;
    }

    cout << final_ans << "\n";
}

int main() {
    // Standard I/O Speedup
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}