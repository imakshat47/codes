#include<iostream>
#include <vector>
using namespace std;




int main(){
    int n; cin>>n;
    vector<int> arr(n);
    for(int &x:arr)
        cin>>x;

    for(int x:arr)
        cout<<x<<" ";
}