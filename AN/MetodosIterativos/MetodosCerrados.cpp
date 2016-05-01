#include <iostream>
#include <fstream>
#include <cmath>
#include "formulas.h"

/// rjhancco@impa.br

using namespace std;

enum Tipos{BISECCION,FALSA_POSICION};

Number Biseccion(Number a, Number b){
	return (a + b) / 2;
}

Number FalsaPosicion(Number a, Number b, Number fa, Number fb){
	return a - ((b-a)/(fb-fa)) * fa;
}

Number Funcion(Number x){
	return x - pow(x,x-cos(x));
	//return x - cos(sin(x));
	//return pow(x,5) - 3 * pow(x,3) - 2 * x * x + 2 - x;
}

bool MetodoCerrado(Number a, Number b, string file, Number(*f)(Number), int tipo, Number presicion, int n){
	cout<<f(a)<<endl;
	cout<<f(b)<<endl;
	if(f(a) * f(b) > 0 or tipo < BISECCION or tipo > FALSA_POSICION) return false;
	string fi = file + ".csv";
	ofstream archivo(fi);
	if(archivo.fail()) return false;
	archivo.precision(15);
	archivo<<"\"i\",\"ai\",\"bi\",\"ri\",\"f(ri)\",\"f(ai)\",\"f(b1)\""<<endl;
	Number r = 0;
	Number fr = 0;
	Number fa = 0;
	Number fb = 0;
	Number r_anterior= 0;
	int i = 0;
	do{
		r_anterior = r;
		fa = f(a);
		fb = f(b);
		if(tipo == BISECCION) r = Biseccion(a,b);
		else r = FalsaPosicion(a,b,fa,fb);
		fr = f(r);
		archivo<<i<<","<<a<<","<<b<<","<<r<<","<<fr<<","<<fa<<","<<fb<<endl;
		if(fa * fr < 0) b = r;
		else a = r;
		i++;
	}while((ErrorAbsoluto(r_anterior, r) > presicion and i != n) or i == 1);
	archivo<<"\"Resultado Final\","<<r<<endl;
	archivo.close();
	return true;
}

int main(){
	Number (*f)(Number) = Funcion;
	Number a;
	Number b;
	Number presicion;
	int n;
	int tt;
	string file;
	cout<<"Ingrese a->";
	cin>>a;
	cout<<"Ingrese b->";
	cin>>b;
	cout<<"BISECCION (1) o FALSA POSICION (2)";
	cin>>tt;
	cout<<"Ingrese la precision->";
	cin>>presicion;
	cout<<"Ingrese la n->";
	cin>>n;
	cout<<"Nombre del archivo donde quiere que salga la tabla->";
	cin>>file;
	if(!MetodoCerrado(a,b,file,f,tt-1,presicion,n)) cout<<"Hubo un Problema"<<endl;
	else cout<<"Archivo generado correctamente"<<endl;
}


