// skip_list_test.cpp
// Simplified Skip List: average O(log n) search/insert

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Node {
    int val;
    vector<Node*> next;
    Node(int v, int lvl): val(v), next(lvl, nullptr) {}
};

class SkipList {
    Node *head;
    int maxLevel;
public:
    SkipList(int ml=6): maxLevel(ml) {
        head = new Node(-1, maxLevel);
        srand(time(nullptr));
    }
    int randomLevel() { 
        int lvl=1; while (rand()%2 && lvl<maxLevel) ++lvl; 
        return lvl;
    }
    void insert(int v) {
        vector<Node*> update(maxLevel);
        Node* cur=head;
        for(int i=maxLevel-1;i>=0;--i){
            while(cur->next[i] && cur->next[i]->val < v) cur=cur->next[i];
            update[i]=cur;
        }
        int lvl=randomLevel();
        Node* nn=new Node(v,lvl);
        for(int i=0;i<lvl;++i){
            nn->next[i]=update[i]->next[i];
            update[i]->next[i]=nn;
        }
    }
    bool search(int v) {
        Node* cur=head;
        for(int i=maxLevel-1;i>=0;--i){
            while(cur->next[i] && cur->next[i]->val < v) cur=cur->next[i];
        }
        cur=cur->next[0];
        return cur && cur->val==v;
    }
};

int main(){
    SkipList sl;
    // Easy
    sl.insert(1); sl.insert(2); sl.insert(3);
    cout<<"SL Easy search(2): "<<sl.search(2)<<'\\n';
    // Medium
    sl.insert(5); sl.insert(10); sl.insert(7); sl.insert(3);
    cout<<"SL Med search(10): "<<sl.search(10)<<'\\n';
    // Hard
    for(int i=0;i<1000;++i) sl.insert(rand()%10000);
    cout<<"SL Hard average search(500): "<<sl.search(500)<<'\\n';
    return 0;
}
