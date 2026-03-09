#include <bits/stdc++.h>
using namespace std;

bool can(long long x, long long n, long long m, long long l, vector<long long>& a){
    priority_queue<long long, vector<long long>, greater<long long>> pq;

    for(int i=0;i<m;i++) pq.push(x);

    for(int i=n-1;i>=0;i--){
        long long t = a[i];

        if(pq.top() < l - t) return false;

        long long cur = pq.top();
        pq.pop();

        pq.push(cur + x);
    }

    return pq.top() >= l;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while(t--){
        long long n,m,l;
        cin >> n >> m >> l;

        vector<long long>a(n);
        for(auto &x:a) cin>>x;

        long long lo=0,hi=l,ans=l;

        while(lo<=hi){
            long long mid=(lo+hi)/2;

            if(can(mid,n,m,l,a)){
                ans=mid;
                hi=mid-1;
            }
            else lo=mid+1;
        }

        cout<<ans<<"\n";
    }
}