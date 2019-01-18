#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "OperacionesMatriz.h"


// Mini buscador de Google
// Corpus de Google: palbra 1 - palabra 2 - frecuencia de ocurrecia entre palabra 1 y palabra 2
// Hacer un buscador de similitud semantica. ejm Leon -> palabras similares semanticamente a leon
// La metafora geometrica del significado
// Word space model -> Magnuss Sahlgren
// Capitulo 2, 3, y 4 -> 17 a la 42 -> Resumen
// Exponer los capitulos -> opcional

using namespace std;


Num fun1(Lista r){
	//return (pow(r[0],2) - r[1] + 0.5) / 2;
	return (8 * r[0] - 4 * pow(r[0],2) + pow(r[1],2) + 1) / 8;
}

Num fun2(Lista r){
	//return (-1 *pow(r[0],2) - 4 * pow(r[1],2) + 8 * r[1] + 4) / 8;
	return (2 * r[0] - pow(r[0],2) + 4 * r[1] - pow(r[1],2) + 3) / 4;
}

Lista _PuntoFijo(ListaFunciones g, Lista r){
	Lista res;
	for(int i = 0; i < g.size(); i++){
		res.push_back(g[i](r));
	}
	return res;
}

void MetodoPuntoFijo(ListaFunciones g, Lista r, string fi, Num n, Num presicion){
	string file = fi + ".csv";
	ofstream archivo(file.c_str());
	int i = 0;
	archivo<<"R"<<i<<endl;
	escribirLista(r,archivo);
	Lista r_anterior;
	do{
		r_anterior = r;
		r = _PuntoFijo(g,r);
		i++;
		archivo<<"R"<<i<<endl;
		escribirLista(r,archivo);
	}while(!ErrorAbsoluto(r_anterior,r,presicion) and i < n);
	archivo<<"Resultado actual"<<endl;
	escribirLista(r,archivo);
	archivo.close();
}

int main(){
	ListaFunciones g;
	Num(*f1)(Lista)  = fun1;
	Num(*f2)(Lista)  = fun2;
	g.push_back(f1);
	g.push_back(f2);
	Lista r = {1.1,2.0};
	Num n;
	Num presicion;
	string file;
	cout<<"Ingrese su n->";
	cin>>n;
	cout<<"Ingrese la presicion->";
	cin>>presicion;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	MetodoPuntoFijo(g,r,file,n,presicion);
}