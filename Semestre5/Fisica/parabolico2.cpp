#include <iostream>
#include "formulas.h"
#include <fstream>

using namespace std;

typedef long double Number;

void MovParabolico(Number Xo, Number Yo, Number Vxo, Number Vyo, Number tfin, Number N){
	Number dt,t ,x ,y ,Vx, Vy, Ay;
	int cN = 0;
	int dN = N/1000;
	dt = tfin/N;
	Ay = -GRAVEDAD_TIERRA;
	Vy = Vyo;
	Vx = Vxo;
	y = Yo;
	x = Xo;
	ofstream archivo("rr.csv");
	archivo<<"\"i\",\"t\",\"x\",\"y\",\"Vx\",\"Vy\""<<endl;
	for(int i = 0; i <= N; i++){
		t = i * dt;
		if(i == cN){
			cout<<"i = "<<i<<"| t = "<<t<<"| x = "<<x<<"| y = "<<y<<"| Vx = "<<Vx<<"| Vy = "<<Vy<<endl;
			archivo<<i<<","<<t<<","<<x<<","<<y<<","<<Vx<<","<<Vy<<endl;
			cN = cN + dN;	
		} 
		Vy = Vy + Ay*dt;
		y = y + Vy*dt;
		x = x + Vx*dt;
	}
	archivo.close();
}

int main(){
	Number N;
	Number Vo;
	Number tfin;
	Number angulo;
	cout<<"Ingrese N->"<<endl;
	cin>>N;
	cout<<"Ingrese Vo->"<<endl;
	cin>>Vo;
	cout<<"Ingrese Tfin->"<<endl;
	cin>>tfin;
	cout<<"Ingrese angulo->"<<endl;
	cin>>angulo;
	MovParabolico(0,0,VelocidadInicialEnX(Vo,angulo),VelocidadInicialEnY(Vo,angulo),tfin,N);
}