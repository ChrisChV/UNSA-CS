#include <iostream>
#include <cmath>
#include "vec.h"

using namespace std;

typedef long double Num

Vector3 Campo_Electrico_Anillo(Num R, Num Q, Num a, int N){
	Vector3 VR,VA,E,r,u;
	Num lambda,dl,dq,ro,dro,s,de,ke;
	lambda = Q/(M_PI*R*R);
	dl = (M_PI*R*R)/N;
	dro = (M_PI*R)/N;
	dq = lambda * dl;
	VA = Vector3(0,0,a);
	ke = 1/(4*M_PI*8.85*pow(10,-12));
	for(int i = 0; i<N; i++){
		ro = i * dro;
		VR = Vector3(R * cos(ro), R * sin(ro),0);
		r = VA - VR;
		u = r.unitario();
		s = r.modulo();
		de = u *= (ke*(dq/pow(s,2)));
		E = E + de;
	}
	return E;
}

Vector3 Campo_Electrico_Anillo(Num R, Num Q, Num a, int N){
	Vector3 VR,VA,E,r,u;
	Num lambda,dl,dq,ro,dro,s,de,ke;
	lambda = Q/(2*M_PI*R);
	dl = (2*M_PI*R)/N;
	dro = (2*M_PI)/N;
	dq = lambda * dl;
	VA = Vector3(0,0,a);
	ke = 1/(4*M_PI*8.85*pow(10,-12));
	for(int i = 0; i<N; i++){
		ro = i * dro;
		VR = Vector3(R * cos(ro), R * sin(ro),0);
		r = VA - VR;
		u = r.unitario();
		s = r.modulo();
		de = u *= (ke*(dq/pow(s,2)));
		E = E + de;
	}
	return E;
}

int main(){

}