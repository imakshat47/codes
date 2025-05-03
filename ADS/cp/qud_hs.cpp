// qp_hash.cpp — Quadratic Probing
#include <iostream>
#include <vector>
using namespace std;

class QuadraticProbeHash {
    vector<int> table; int M;
public:
    QuadraticProbeHash(int m): M(m), table(m, -1) {}
    int hash(int key) const { return key % M; }

    void insertKey(int key){
        int i = 0, idx;
        while (i < M) {
            idx = (hash(key) + i*i) % M;
            if (table[idx] < 0) { table[idx] = key; return; }
            ++i;
        }
    }
    bool searchKey(int key){
        int i = 0, idx;
        while (i < M) {
            idx = (hash(key) + i*i) % M;
            if (table[idx]==key) return true;
            if (table[idx]==-1) break;
            ++i;
        }
        return false;
    }
    void removeKey(int key){
        int i = 0, idx;
        while (i < M) {
            idx = (hash(key) + i*i) % M;
            if (table[idx]==key) { table[idx] = -2; return; }
            if (table[idx]==-1) break;
            ++i;
        }
    }
};

int main(){
    QuadraticProbeHash h(11);
    // Easy
    h.insertKey(7); h.insertKey(18);
    cout<<"QP Easy find 18: "<<h.searchKey(18)<<\"\\n\";

    // Medium
    for(int x:{5,16,27,38}) h.insertKey(x);
    cout<<"QP Med find 27: "<<h.searchKey(27)<<\"\\n\";

    // Hard
    for(int i=0;i<100;++i) h.insertKey(i*5);
    cout<<"QP Hard find 185: "<<h.searchKey(185)<<\"\\n\";
    return 0;
}
