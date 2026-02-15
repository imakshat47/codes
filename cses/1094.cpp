// Increasing Array
#include<iostream>
#include<vector>
// #include <initializer_list>
#include <algorithm> // for std::max
using namespace std;

int main(){
    int n; cin>>n;
    vector<int> arr(n);
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    long int ans=0;
    int last_max=arr[0];
    for(int i=1;i<n;++i){
        ans+=max({arr[i-1]-arr[i],last_max-arr[i],0});
        last_max=max(last_max,arr[i]);
    }
    cout<<ans<<endl;
}

/*  Test Cases:
10
6 10 4 10 2 8 9 2 7 7


*/