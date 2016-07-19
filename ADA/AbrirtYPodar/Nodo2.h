#ifndef NODO2_H
#define NODO2_H

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

class Nodo{
	public:
		Nodo();
		Nodo(tuple<vector<int>,vector<int>>,int,int,int);
		Nodo(tuple<vector<int>,vector<int>>, int, int, Nodo *);
		vector<int> procesadores;
		vector<int> prod;
		int costo;
		int nivel;
		int id;
		Nodo * padre;
};

Nodo::Nodo(){
	costo = 0;
	nivel = 0;
	padre = nullptr;
}

Nodo::Nodo(tuple<vector<int>,vector<int>> tt, int costo, int nivel, int id){
	prod = get<0>(tt);
	procesadores = get<1>(tt);
	this->costo = costo;
	this->nivel = nivel;
	this->id = id;
	padre = nullptr;
}

Nodo::Nodo(tuple<vector<int>,vector<int>> tt, int id, int costo, Nodo * p){
	prod = get<0>(tt);
	procesadores = get<1>(tt);
	this->costo = costo;
	nivel = p->nivel + 1;
	this->id = id;
	padre = p;
}

#endif