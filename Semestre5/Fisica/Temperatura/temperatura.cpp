#include <iostream>
#include <fstream>

using namespace std;

typedef long double Num;

// 5 gramos de hielo 5 grados 
// 100 gramos a 80 grados

Num cu = 0.5;
Num ca = 1;
Num La = 80;
Num mf = 30;
Num mc = 20;
Num tf = -15;
Num tc = 20;

Num Q(Num t){
	//Num Q = c * mf * (t-tf) + c * mc * (t-tc);
	Num Q = mf * cu * (t-tf) + La * mf + mc * ca * (t-tc);
	return Q;
}

int main(){
	Num Qr;
	ofstream archivo("gra.dat");
	for(Num t = tf; t < tc; t = t+0.5){
		Qr = Q(t);
		archivo<<t<<" "<<Qr<<endl;
	}
}