#include <iostream>
#include <cmath>

using namespace std;

typedef float Var;

Var t = 0.001;
Var terminus = 100000;
Var presicion = 0.1;

Var funcion(Var x){
	return (x*x) + x + 1;	
}

Var derivada_media(Var xo, Var xf){
	Var dx =  xf - xo;
	Var dy = funcion(xf) - funcion(xo);
	Var Dy = dy / dx;
	return Dy;
}

int main(){
	Var dx = 0.001;
	Var xo;
	cout<<"Ingrese su Xo ";
	cin>>xo;
	Var xf = xo + dx;
	Var Dya = derivada_media(xo,xf);
	Var Dy = 0;
	int c = 0;
	do{
		dx += t;
		xf = xo + dx;
		Dy = derivada_media(xo,xf);
	}while(abs(Dy-Dya) / abs(Dya) <= presicion);
	//if(c == terminus) cout<<"El programa no pudo resolver la derevada"<<endl;
	else cout<<"Dya->"<<Dya<<endl<<"Dy->"<<Dy<<endl<<"Xf->"<<xf<<endl<<"dx->"<<dx<<endl;
}