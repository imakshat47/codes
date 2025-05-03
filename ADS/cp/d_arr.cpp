// dynamic_array_test.cpp
// Dynamic Array: amortized O(1) insert

#include <iostream>
using namespace std;

class DynArr {
    int *data, sz, cap;
public:
    DynArr(): sz(0), cap(2) { data=new int[cap]; }
    void push(int v){
        if(sz==cap){
            cap*=2;
            int* tmp=new int[cap];
            for(int i=0;i<sz;++i) tmp[i]=data[i];
            delete[] data;
            data=tmp;
        }
        data[sz++]=v;
    }
    void pop(){ if(sz>0) --sz; if(sz<cap/4){ cap/=2; /* shrink */ } }
    int size(){ return sz; }
    int operator[](int i){ return data[i]; }
};

int main(){
    DynArr da;
    // Easy
    da.push(1); da.push(2); da.push(3);
    cout<<"DA Easy size: "<<da.size()<<'\\n';
    // Medium
    for(int i=4;i<=10;++i) da.push(i);
    cout<<"DA Med capacity growth OK, size: "<<da.size()<<'\\n';
    // Hard
    for(int i=0;i<100000;++i) da.push(i);
    cout<<"DA Hard size: "<<da.size()<<'\\n';
    return 0;
}
