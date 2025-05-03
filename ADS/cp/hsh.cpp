// hashing_test.cpp
// Simple open-addressing hash table: avg O(1)

#include <iostream>
#include <vector>
using namespace std;

class HashTable {
    vector<int> table;
    int cap;
public:
    HashTable(int c=101): cap(c) { table.assign(cap, -1); }
    void insert(int v){
        int h = v % cap;
        while (table[h] != -1 && table[h] != -2) h = (h+1)%cap;
        table[h] = v;
    }
    bool search(int v){
        int h = v % cap;
        int start = h;
        while (table[h] != -1){
            if(table[h]==v) return true;
            h=(h+1)%cap;
            if(h==start) break;
        }
        return false;
    }
};

int main(){
    HashTable ht;
    // Easy
    ht.insert(10); ht.insert(20); ht.insert(30);
    cout<<"HT Easy search(20): "<<ht.search(20)<<'\\n';
    // Medium
    for(int x:{15,26,37,48,59}) ht.insert(x);
    cout<<"HT Med search(37): "<<ht.search(37)<<'\\n';
    // Hard
    for(int i=0;i<100000;++i) ht.insert(i);
    cout<<"HT Hard search(99999): "<<ht.search(99999)<<'\\n';
    return 0;
}
