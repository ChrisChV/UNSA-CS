#include <iostream>
#include <fstream>
#include <cmath>
#include "formulas.h"

using namespace std;

Number funcion(Number x){
	//return x - pow(x,x-cos(x));
	//return x - cos(sin(x));
	return pow(x,5) - 3 * pow(x,3) - 2 * x * x + 2 - x;
	//return 1 - (x*x/4);
}

Number _funcion(Number x){
	//return pow(x,x-cos(x));
	//return cos(sin(x));
	return pow(x,5) - 3 * pow(x,3) - 2 * x * x + 2;
	//return 1 + x -(x*x/4);
}

void MetodoPuntoFijo(Number(*f)(Number),Number(*g)(Number), Number r, string fi, Number n, Number presicion){
	string file = fi + ".csv";
	ofstream archivo(file);
	Number fr = 0;
	Number gr = 0;
	Number i = 0;
	Number r_anterior = 0;
	archivo<<"\"i\",\"ri\",\"f(ri)\",\"g(ri)\""<<endl;
	do{
		r_anterior = r;
		fr = f(r);
		gr = g(r);
		archivo<<i<<","<<r<<","<<fr<<","<<gr<<endl;
		r = gr;
		i++;
	}while((ErrorAbsoluto(r_anterior,r) > presicion and i != n) or i == 1);
	archivo<<"\"Resultado Final\","<<r<<endl;
	archivo.close();
	cout<<"Archivo generado correctamente"<<endl;
}

int main(){
	Number(*f)(Number) = funcion;
	Number(*g)(Number) = _funcion;
	Number r;
	Number n;
	Number presicion;
	string file;
	cout<<"Ingrese su r0->";
	cin>>r;
	cout<<"Ingrese su n->";
	cin>>n;
	cout<<"Ingrese la presicion->";
	cin>>presicion;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	MetodoPuntoFijo(f,g,r,file,n,presicion);
}