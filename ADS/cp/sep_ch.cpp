// chaining_hash.cpp — Separate Chaining
#include <iostream>
#include <vector>
#include <list>
using namespace std;

// C++11 separate chaining hash table
class ChainHash {
    vector<list<int>> table;
    int M;
public:
    ChainHash(int m): M(m), table(m) {}

    int hash(int key) const { return key % M; }  // simple mod-based hash

    void insertKey(int key) {
        table[hash(key)].push_back(key);
    }
    bool searchKey(int key) {
        auto &bucket = table[hash(key)];
        for (int k : bucket) if (k == key) return true;
        return false;
    }
    void removeKey(int key) {
        auto &bucket = table[hash(key)];
        bucket.remove(key);
    }
};

int main(){
    ChainHash h(5);
    // Easy
    h.insertKey(12); h.insertKey(22); // both in bucket 2
    cout<<"Chain Easy find 22: "<<h.searchKey(22)<<\"\\n\"; 

    // Medium
    for(int x:{15,25,35}) h.insertKey(x);
    cout<<"Chain Med find 35: "<<h.searchKey(35)<<\"\\n\";

    // Hard
    for(int i=0;i<100;++i) h.insertKey(i*5);
    cout<<"Chain Hard find 250: "<<h.searchKey(250)<<\"\\n\";
    return 0;
}
