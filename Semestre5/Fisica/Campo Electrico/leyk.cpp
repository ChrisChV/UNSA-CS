#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

typedef long double Num;

void Circuito(Num W, Num Ep, Num R, Num C, Num L, Num tf){
	Num ti, dt, Wo, V;
	int N = 5000, I, DI, Vo;
	
}

void Carga(Num R, Num C, Num E, int N, Num S){
	Num T, dt, tf, Vc, t,q,I;
	T = R * C;
	tf = S * T;
	dt = tf/N;
	t = 0;
	q = 0;
	ofstream archivo("carga.csv");
	for(int i = 0; i <= N; i++){
		t = i * dt;
		I = -q/(R*C) + E/R;
		q = q + I * dt;
		Vc = q / C;
		archivo<<t<<","<<q<<","<<I<<","<<Vc<<endl;
	}
}

int main(){
	Carga(2,1.0*pow(10,-6),5,2000,5);
}