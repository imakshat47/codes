#include<iostream>
using namespace std;

int main(){
    uint_fast32_t t; 
    cin >> t;

    while(t--){
        uint_fast32_t n; 
        cin >> n;

        string s; 
        cin >> s;

        string smax = s;
        bool changed = true;

        // maximize
        while(changed){
            changed = false;
            for(uint_fast32_t i = 1; i < n-1; i++){
                if(smax[i-1]=='1' && smax[i+1]=='1' && smax[i]=='0'){
                    smax[i] = '1';
                    changed = true;
                }
            }
        }

        uint_fast32_t maxc = 0;
        for(char c : smax) if(c=='1') maxc++;

        string smin = smax;
        changed = true;

        // minimize
        while(changed){
            changed = false;
            for(uint_fast32_t i = 1; i < n-1; i++){
                if(smin[i-1]=='1' && smin[i+1]=='1' && smin[i]=='1'){
                    smin[i] = '0';
                    changed = true;
                }
            }
        }

        uint_fast32_t minc = 0;
        for(char c : smin) if(c=='1') minc++;

        cout << minc << " " << maxc << "\n";
    }
}