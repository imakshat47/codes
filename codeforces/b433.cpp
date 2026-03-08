#include<iostream>
#include<algorithm>
using namespace std;

int main(){
    uint32_t n; cin>>n;
    uint32_t arr[n+1] {};

    for(uint32_t i=1;i<=n;++i)
        cin>>arr[i];
    
    // prefix sum and sorted prefix sum
    uint64_t pfx[n+2] {}, srtPfx[n+2] {};
    
    for(uint32_t i=1;i<=n;++i)
        pfx[i] = pfx[i-1] + arr[i];
    
    sort(arr+1,arr+n+1);

    for(uint32_t i=1;i<=n;++i)
        srtPfx[i] = srtPfx[i-1]+arr[i];

    uint32_t m; cin>>m;
    while(m--){
        uint32_t q, l,r; cin>>q>>l>>r;

        if(q==1){
            cout<<pfx[r]-pfx[l-1]<<endl;
        }else 
            cout<<srtPfx[r]-srtPfx[l-1]<<endl;
    } 
}