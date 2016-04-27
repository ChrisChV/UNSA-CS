#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#define PRESICION 0.0000001

enum Tipos{BISECCION,FALSA_POSICION};

typedef long double Number;

Number ErrorRelativo(Number x, Number xs){
	return abs(xs - x);
}

Number Biseccion(Number a, Number b){
	return (a + b) / 2;
}

Number FalsaPosicion(Number a, Number b, Number fa, Number fb){
	return a - ((b-a)/(fb-fa)) * fa;
}

Number Funcion(Number x){
	return x*x*x -2;
}

bool MetodoCerrado(Number a, Number b, string file, Number(*f)(Number), int tipo){
	if(f(a) * f(b) >= 0 or tipo < BISECCION or tipo > FALSA_POSICION) return false;
	string fi = file + ".csv";
	ofstream archivo(fi);
	if(archivo.fail()) return false;
	archivo.precision(15);
	archivo<<"\"i\",\"ai\",\"bi\",\"ri\",\"f(ri)\",\"f(ai)\",\"f(b1)\""<<endl;
	Number r = 0;
	Number fr = 0;
	Number fa = 0;
	Number fb = 0;
	int i = 0;
	do{
		fa = f(a);
		fb = f(b);
		if(tipo == BISECCION) r = Biseccion(a,b);
		else r = FalsaPosicion(a,b,fa,fb);
		fr = f(r);
		archivo<<i<<","<<a<<","<<b<<","<<r<<","<<fr<<","<<fa<<","<<fb<<endl;
		if(fa * fr < 0) b = r;
		else a = r;
		i++;
	}while(abs(fr) > PRESICION);
	archivo<<"\"Resultado Final\","<<r<<endl;
	archivo.close();
	return true;
}

int main(){
	Number (*f)(Number) = Funcion;
	Number a;
	Number b;
	int tt;
	string file;
	cout<<"Ingrese a->";
	cin>>a;
	cout<<"Ingrese b->";
	cin>>b;
	cout<<"BISECCION (1) o FALSA POSICION (2)";
	cin>>tt;
	cout<<"Nombre del archivo->";
	cin>>file;
	if(!MetodoCerrado(a,b,file,f,tt-1)) cout<<"Hubo un Problema"<<endl;
	else cout<<"Archivo generado correctamente"<<endl;
}


