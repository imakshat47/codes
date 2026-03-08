#include<iostream>
using namespace std;
const uint32_t N = 2e5+2;
int32_t temp[N] {};

// void printArr(int32_t* arr, int n){
//     for(int i=90;i<n;++i)
//         cout<<arr[i]<<" ";
//     cout<<endl;
// }

int main(){
    uint32_t n,k,q; cin>>n>>k>>q;
    while(n--){
        uint32_t l,r;cin>>l>>r;
        temp[l]++;
        temp[r+1]--;
    }

    // printArr(temp,100);

    for(uint32_t i=1;i<N;++i){
        temp[i]+=temp[i-1];
    }

    // printArr(temp,100);

    for(uint32_t i=1;i<N;++i){
        if(temp[i]>=k)
            temp[i] = 1;
        else temp[i] = 0;
    }

    // printArr(temp,100);

    for(uint32_t i=1;i<N;++i){
        temp[i]+=temp[i-1];
    }

    // printArr(temp,100);

    while(q--){
        uint32_t a,b; cin>>a>>b;
        cout<<temp[b]-temp[a-1]<<endl;
    }

}