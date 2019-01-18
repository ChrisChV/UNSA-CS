#include <iostream>
#include <cmath>

using namespace std;

typedef long double Num;

Num presion(Num D1, Num D2, Num V1){
	Num rho, DP, V2, k;
	rho = 950;
	k = pow(D2/D1,2);
	V2 = V1/k;
	DP = 0.5 * rho * (1-k*k) * V2*V2;
	return DP;
}

int main(){
	Num Dp;
	Dp = presion(5,0.1,0.05);
	cout<<Dp<<endl;
}