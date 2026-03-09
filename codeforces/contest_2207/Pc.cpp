#include <bits/stdc++.h>
using namespace std;

int main(){
    uint_fast32_t t;
    cin>>t;

    while(t--){
        uint_fast32_t n;
        long long h;
        cin>>n>>h;

        vector<long long>a(n),b(n);
        for(uint_fast32_t i=0;i<n;i++){
            cin>>a[i];
            b[i]=h-a[i];
        }

        vector<int>L(n),R(n);
        stack<int>st;

        for(uint_fast32_t i=0;i<n;i++){
            while(!st.empty() && b[st.top()]>=b[i]) st.pop();
            L[i]=st.empty()? -1:st.top();
            st.push(i);
        }

        while(!st.empty()) st.pop();

        for(uint_fast32_t i=n-1;i>=0;i--){
            while(!st.empty() && b[st.top()]>=b[i]) st.pop();
            R[i]=st.empty()? n:st.top();
            st.push(i);
        }

        vector<long long>best(n);

        for(uint_fast32_t i=0;i<n;i++){
            long long width=R[i]-L[i]-1;
            best[i]=width*b[i];
        }

        vector<long long>pref(n),suff(n);

        pref[0]=best[0];
        for(uint_fast32_t i=1;i<n;i++)
            pref[i]=max(pref[i-1],best[i]);

        suff[n-1]=best[n-1];
        for(uint_fast32_t i=n-2;i>=0;i--)
            suff[i]=max(suff[i+1],best[i]);

        long long ans=0;

        for(uint_fast32_t i=0;i<n;i++){
            ans=max(ans,best[i]);
            if(i+1<n) ans=max(ans,pref[i]+suff[i+1]);
        }

        cout<<ans<<"\n";
    }
}