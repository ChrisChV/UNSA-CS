#include <iostream>
#include <map>
#include <fstream>
#include "OperacionesMatriz.h"
#include "Punto.h"

using namespace std;

vector<Num> hallarAs(vector<Punto> &puntos){
	Matriz tabla = zeros(puntos.size(), puntos.size() + 1);
	for(int i = 0; i < puntos.size(); i++){
		tabla[i][0] = puntos[i].x;
		tabla[i][1] = puntos[i].y;
	}
	for(int i = 2; i < puntos.size() + 1; i++){
		for(int j = i - 1; j < puntos.size(); j++){
			tabla[j][i] = (tabla[j][i-1] - tabla[j-1][i-1]) / (tabla[j][0] - tabla[j-(i-1)][0]);
		}
	}
	mostrarMatriz(tabla);
	vector<Num> res; 
	for(int i = 0; i < puntos.size(); i++){
		res.push_back(tabla[i][i+1]);
	}
	return res;
}

void Newton(vector<Punto> &puntos){
	auto As = hallarAs(puntos);
	ofstream archivo("funcion.h");
	archivo<<"#ifndef FUNCION_H"<<endl;
	archivo<<"#define FUNCION_H"<<endl;
	archivo<<"#include <iostream>"<<endl;
	archivo<<"#include <cmath>"<<endl;
	archivo<<"#include \"Punto.h\""<<endl<<endl;
	archivo<<"#include \"OperacionesMatriz.h\""<<endl;
	archivo<<"using namespace std;"<<endl<<endl;
	archivo<<"Num fun(Num x){"<<endl;
	archivo<<"return ";
	cout<<As.size()<<endl;
	for(int i = 0; i < As.size(); i++){
		if(i != 0) archivo<<"+";
		archivo<<to_string(As[i]);
		if(i != 0) archivo<<"*";
		string temp = " ";
		for(int j = 0; j < i; j++){
			temp += "(x-" + to_string(puntos[j].x) + ")*";
		}
		temp.pop_back();
		archivo<<temp;
	}
	cout<<"BB"<<endl;
	archivo<<";"<<endl;
	archivo<<"}"<<endl<<endl;
	archivo<<"vector<Punto> getPuntos(){"<<endl;
	archivo<<"vector<Punto> res;"<<endl;
	for(auto iter = puntos.begin(); iter != puntos.end(); ++iter){
		archivo<<"res.push_back(Punto("<<(*iter).x<<","<<(*iter).y<<"));"<<endl;
	}
	archivo<<"return res;"<<endl;
	archivo<<"}"<<endl;
	archivo<<"#endif";
	archivo.close();
	system("g++ -std=c++11 secondNewton.cpp -o runnew2");
	system("./runnew2");
}

int main(){
	vector<Punto> puntos;
	puntos.push_back(Punto(1,2));
	puntos.push_back(Punto(3,4));
	puntos.push_back(Punto(5,2));
	Newton(puntos);

}