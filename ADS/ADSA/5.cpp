#include <bits/stdc++.h>
using namespace std;
int Search(int n,int arr[], int key)
{
    for(int i=0;i<n;i++)
    {
        if(arr[i]==key)
        {
            return i;
        }
    }
    return -1;
}
int main()
{
    int n,key;
    cin>>n>>key;
    int arr[n];
    for(int i=0;i<n;i++)
    {
        cin>>arr[i];
    }
    cout<<Search(n,key);
    return 0;

}