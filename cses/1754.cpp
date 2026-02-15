// Coin Piles
#include<iostream>
using namespace std;

int main(){
    int t; cin>>t;
    while(t--){
        int a,b; cin>>a>>b;
        if(a==0 or b==0){
            cout<<"NO"<<endl;
            continue;
        }
        if((a%3==0 && b%3==0) || (a+b)%3==0){
            cout<<"YES"<<endl;
        }else cout<<"NO"<<endl;
    }
}