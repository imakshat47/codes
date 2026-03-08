#include<iostream>
#include<algorithm>
using namespace std;
const int32_t N=1e2+1;
int32_t Freq[N] {};

void Approcah1(int32_t *arr, uint32_t n){
    int32_t maxi = -1;
    for(int i=0;i<n;++i){
        maxi = max(maxi,arr[i]);
        Freq[arr[i]]++;
    }

    for(uint32_t x:Freq)
        cout<<x<<" ";
    cout<<endl;
}

// O (nlog n)
void Approach2(int32_t *arr, uint32_t n){
    sort(arr,arr+n);
    int32_t lst_ele = arr[0], count = 0;

    for(int i=0;i<n;++i){
        if(arr[i]==lst_ele)
            ++count;
        else{
            cout<<lst_ele<<" => "<<count<<endl;
            lst_ele = arr[i];
            count=1;
        }
    }
    cout<<lst_ele<<" => "<<count<<endl;

}

int main(){
    uint32_t n;cin>>n;
    // uint32_t key; cin>>key;
    int32_t arr[n] {};
    uint32_t count {0}, x;
    for(uint32_t i=0;i<n;++i){
        cin>>arr[i];
        // cin>>x;
        // if(x==key)
        // ++count;
    }

    // Approcah1(arr,n);

    Approach2(arr,n);
    // cout<<count<<endl;
}