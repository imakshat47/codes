// splay_tree_test.cpp
// Splay Tree: amortized O(log n)

#include <iostream>
using namespace std;

struct Node {
    int val;
    Node *l,*r;
    Node(int v): val(v), l(nullptr), r(nullptr) {}
};

class SplayTree {
    Node* root;
    void rotateRight(Node*& p) {
        Node* q = p->l; p->l = q->r; q->r = p; p = q;
    }
    void rotateLeft(Node*& p) {
        Node* q = p->r; p->r = q->l; q->l = p; p = q;
    }
    void splay(Node*& p, int v) {
        if(!p) return;
        if(v < p->val){
            if(!p->l) return;
            if(v < p->l->val) { splay(p->l->l,v); rotateRight(p); }
            else if(v > p->l->val){ splay(p->l->r,v); if(p->l->r) rotateLeft(p->l); }
            rotateRight(p);
        } else if(v > p->val){
            if(!p->r) return;
            if(v > p->r->val){ splay(p->r->r,v); rotateLeft(p); }
            else if(v < p->r->val){ splay(p->r->l,v); if(p->r->l) rotateRight(p->r); }
            rotateLeft(p);
        }
    }
public:
    SplayTree(): root(nullptr) {}
    void insert(int v){ 
        if(!root){ root=new Node(v); return; }
        splay(root,v);
        if(root->val==v) return;
        Node* n=new Node(v);
        if(v < root->val){ n->l=root->l; n->r=root; root->l=nullptr; }
        else{ n->r=root->r; n->l=root; root->r=nullptr; }
        root=n;
    }
    bool search(int v){
        splay(root,v);
        return root && root->val==v;
    }
};

int main(){
    SplayTree st;
    // Easy
    st.insert(10); st.insert(20); st.insert(30);
    cout<<"SP Easy search(20): "<<st.search(20)<<'\\n';
    // Medium
    for(int i=1;i<=10;++i) st.insert(i);
    st.search(6);
    cout<<"SP Med root should be 6: "<< (st.search(6)?6:-1) <<'\\n';
    // Hard
    for(int i=0;i<1000;++i) st.insert(rand()%1000);
    cout<<"SP Hard search(500): "<<st.search(500)<<'\\n';
    return 0;
}
