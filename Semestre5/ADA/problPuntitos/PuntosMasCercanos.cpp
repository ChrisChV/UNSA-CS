#include <iostream>
#include "Punto.h"
#include <tuple>
#include <cmath>
#include <algorithm>

using namespace std;

enum Lados{IZQUIERDA,DERECHA};

float Distancia(tuple<Punto,Punto> tt){
	Punto p1 = get<0>(tt);
	Punto p2 = get<1>(tt);
	return sqrt(pow(p2.x - p1.x,2) + pow(p2.y - p1.y,2));
}

float _Distancia(int x1, int y1, int x2, int y2){
	return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}

tuple<Punto,Punto> _puntosMenorDistancia(vector<Punto> vec, vector<Punto>::iterator ini,
				vector<Punto>::iterator medio, vector<Punto>::iterator fin, tuple<Punto,Punto> puntosMenores){
	int med =  (((*fin).x - (*medio).x) / 2) + (*medio).x; 
	sort(ini, medio + 1,sortByY);
	sort(medio + 1, fin + 1,sortByY);
	bool lado = IZQUIERDA;
	auto iterIzq = ini;
	auto iterDer = medio + 1;
	float tempIzq = 0;
	float tempDer = 0;
	float menorDis = Distancia(puntosMenores);
	while(iterIzq != medio + 1 and iterDer != fin + 1){
		if(lado == IZQUIERDA){
			tempIzq = _Distancia((*iterIzq).x,(*iterDer).y,med,(*iterIzq).y);
			tempDer = _Distancia((*iterDer).x,(*iterDer).y,med,(*iterIzq).y);
			lado = DERECHA;
			++iterIzq;
		}
		else{
			tempIzq = _Distancia((*iterIzq).x,(*iterIzq).y,med,(*iterDer).y);
			tempDer = _Distancia((*iterDer).x,(*iterDer).y,med,(*iterDer).y);
			lado = IZQUIERDA;
			++iterDer;
		}
		float radio = menorDis / 2;
		if(tempIzq <= radio and tempDer <= radio){
			puntosMenores = make_tuple(*iterIzq,*iterDer);
			menorDis = Distancia(puntosMenores);
		}
	}
	return puntosMenores;
}

tuple<Punto,Punto> puntosMenorDistancia(vector<Punto> vec, int ini, int fin){
	if(fin - ini == 1) return make_tuple(vec[ini],vec[fin]);
	if(fin - ini == 2){
		auto a = make_tuple(vec[ini],vec[ini + 1]);
		auto b = make_tuple(vec[ini],vec[fin]);
		auto c = make_tuple(vec[ini + 1],vec[fin]);
		float a1 = Distancia(a);
		float b1 = Distancia(b);
		float c1 = Distancia(c);
		if(a1 < b1){
			if(a1 < c1) return a;
			else if(c1 < b1) return c;
			return b;
		}
		else if(b1 < c1) return b;
		else if(c1 < a1) return c;
		else return a;
	}
	int M = (ini + fin)/2;
	auto a = puntosMenorDistancia(vec,ini,M);
	auto b = puntosMenorDistancia(vec,M+1,fin);
	auto menorDis = a;
	if(Distancia(b) < Distancia(a)) menorDis = b;
	return _puntosMenorDistancia(vec, vec.begin() + ini, vec.begin() + M, vec.begin() + fin,menorDis);
}

int main(){
	srand (time(NULL));
	int xMax;
	int yMax;
	int numPuntos;
	string file;
	cout<<"Ingresa su xMax->";
	cin>>xMax;
	cout<<"Ingrese su yMax->";
	cin>>yMax;
	cout<<"Ingrese su numero maximo de puntos->";
	cin>>numPuntos;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	vector<Punto> vec = generarPuntosAleatorios(xMax,yMax,numPuntos);
	sort(vec.begin(),vec.end(),sortByX);
	auto res = puntosMenorDistancia(vec,0,vec.size()-1);
	vector<Punto> rr;
	rr.push_back(get<0>(res));
	rr.push_back(get<1>(res));
	generarGrafica(file,vec,rr);
}