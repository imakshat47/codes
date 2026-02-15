// Permutations
#include<iostream>
#include<vector>

using namespace std;

int main(){
    int n; cin>>n;

    vector<int> arr(n);
    int idx=0;
    for(int i=0;i<n/2;++i){
        arr[idx++] = 2*(i+1);
    }
    for(int i=0;i<n/2;++i){
        arr[idx++] = 2*i+1;
    }
    if(n%2){
        // odd
        arr[n-1]=n;
    }

    for(int i=1;i<n;++i){
        if(abs(arr[i-1]-arr[i])==1){
             cout<<"NO SOLUTION"<<endl;
             return 0;
        }
    }

    for(int x:arr)
        cout<<x<<" ";
    cout<<endl;
}