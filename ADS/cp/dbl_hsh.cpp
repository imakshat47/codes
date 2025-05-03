// dh_hash.cpp — Double Hashing
#include <iostream>
#include <vector>
using namespace std;

class DoubleHash {
    vector<int> table; int M;
public:
    DoubleHash(int m): M(m), table(m, -1) {}
    int h1(int key) const { return key % M; }
    int h2(int key) const { return 1 + (key % (M-1)); }

    void insertKey(int key){
        int i=0, idx;
        while (i < M) {
            idx = (h1(key) + i * h2(key)) % M;
            if (table[idx] < 0) { table[idx]=key; return; }
            ++i;
        }
    }
    bool searchKey(int key){
        int i=0, idx;
        while (i < M) {
            idx = (h1(key) + i * h2(key)) % M;
            if (table[idx]==key) return true;
            if (table[idx]<0) break;
            ++i;
        }
        return false;
    }
    void removeKey(int key){
        int i=0, idx;
        while (i < M) {
            idx = (h1(key) + i * h2(key)) % M;
            if (table[idx]==key) { table[idx]=-2; return; }
            if (table[idx]<0) break;
            ++i;
        }
    }
};

int main(){
    DoubleHash h(13);
    // Easy
    h.insertKey(5); h.insertKey(18);
    cout<<"DH Easy find 18: "<<h.searchKey(18)<<\"\\n\";

    // Medium
    for(int x:{10,23,36,49}) h.insertKey(x);
    cout<<"DH Med find 36: "<<h.searchKey(36)<<\"\\n\";

    // Hard
    for(int i=0;i<200;++i) h.insertKey(i*7);
    cout<<"DH Hard find 133: "<<h.searchKey(133)<<\"\\n\";
    return 0;
}
