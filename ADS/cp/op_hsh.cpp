// lp_hash.cpp — Linear Probing
#include <iostream>
#include <vector>
using namespace std;

class LinearProbeHash {
    vector<int> table;
    int M;
public:
    LinearProbeHash(int m): M(m), table(m, -1) {}

    int hash(int key) const { return key % M; }

    void insertKey(int key){
        int idx = hash(key);
        while (table[idx] != -1 && table[idx] != -2)
            idx = (idx + 1) % M;
        table[idx] = key;
    }
    bool searchKey(int key){
        int idx = hash(key), start = idx;
        while (table[idx] != -1){
            if (table[idx]==key) return true;
            idx = (idx + 1) % M;
            if (idx==start) break;
        }
        return false;
    }
    void removeKey(int key){
        int idx = hash(key), start=idx;
        while (table[idx] != -1){
            if (table[idx]==key){
                table[idx] = -2; // deleted marker
                return;
            }
            idx = (idx + 1) % M;
            if (idx==start) break;
        }
    }
};

int main(){
    LinearProbeHash h(7);
    // Easy
    h.insertKey(10); h.insertKey(17); // collisions at 3
    cout<<"LP Easy find 17: "<<h.searchKey(17)<<\"\\n\";

    // Medium
    for(int x:{3,10,17,24}) h.insertKey(x);
    cout<<"LP Med find 24: "<<h.searchKey(24)<<\"\\n\";

    // Hard
    for(int i=0;i<50;++i) h.insertKey(i*3);
    cout<<"LP Hard find 99: "<<h.searchKey(99)<<\"\\n\";
    return 0;
}
