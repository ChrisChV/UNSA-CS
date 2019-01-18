#include <iostream>
#include <cmath>

using namespace std;

long lll(long a, long b, long m){
	if(b == 1) return a % m;
	if(a == 1) return 1;
	if(b == 0) return 1;
	long temp = lll(a,b/2,m);
	return (temp * temp) % m;
}

int main(){
	cout<<lll(7,16,13)<<endl;
}