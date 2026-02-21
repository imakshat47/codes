#include<iostream>
using namespace std;
int main() {
	int n; cin>>n;
	bool even_flag = true;
	int sum_even = 0, sum_odd = 0;
	while(n){
		if(even_flag){
			sum_even+=n%10;
		}else{
			sum_odd+=n%10;
		}
		n/=10;
		even_flag!=even_flag;
	}
	return 0;
}