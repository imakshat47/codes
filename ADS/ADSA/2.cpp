#include <iostream>
using namespace std;
int main()
{
    int n;
    cin>>n;
    int arr[n];
    for(int i=0;i<n;i++)
    {
        cin>>arr[i];
        cout<<endl;
    }
    int* ptr=arr;
    cout<<"Print values"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<*(ptr+i)<<" ";
    }
    return 0;
}