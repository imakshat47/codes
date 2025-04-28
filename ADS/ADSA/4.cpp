#include <iostream>
using namespace std;
int main()
{
    int n,pos,elem;
    cin>>n;
    int arr[n+1];
    for(int i=0;i<n;i++)
    {
        cin>>arr[i];
    }
    cin>>elem;
    cout<<n+1<<" ";
    cin>>pos;
    if(pos<1 || pos>n+1)
    {
        cout<<"Invalid"<<endl;
        return 1;
    }
    for(int i=n;i>=pos;i--)
    {
        arr[i]=arr[i-1];
    }
    arr[pos-1]=elem;
    n++;
    for(int i=0;i<n;i++)
    {
        cout<<arr[i]<<" ";
    }
    return 0;
}