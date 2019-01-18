#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

void EDUL(float tf,float s0, float v0, int n){
	float t,dt,s,v,a,m;
	float u,b,g;
	float theta;
	int cN;
	ofstream archivo("eje.csv");
	m = 20;
	t = 0;
	s = s0;
	v = v0;
	dt = (tf -0)/n;
	cN = 0;
	u = 0.59;
	b=1.2;
	g=9.78;
	theta = 30.7 * M_PI /180;
	archivo<<"\"t\",\"s\",\"v\",\"a\""<<endl;
	for(int i = 0; i <= n; i++){
		t = i*dt;
		a = g*(sin(theta)-u*cos(theta)) - (b/m) * v;
		if(i == cN or i == n){
			archivo<<t<<","<<s<<","<<v<<","<<a<<endl;
			cN += n/200;
		}
		v = v + a *dt;
		s = s + v * dt;
	}
}

int main(){
	float tf,s0,v0;
	tf =  80;
	s0 = 0;
	v0 = 0.6;
	EDUL(tf,s0,v0,9000);
}