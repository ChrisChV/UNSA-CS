#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

typedef int IdEstado;
typedef tuple<IdEstado,char,IdEstado> Transicion;

class Automata{
public:
	Automata(){};
	Automata(char c, IdEstado &estadoActual);

	void printAutomata();

	string expresionRegular;
	vector<IdEstado> estados;
	IdEstado inicial;
	vector<IdEstado> aceptacion;
	vector<char> entradas;
	vector<Transicion> transiciones;

};


Automata::Automata(char c, IdEstado &estadoActual){
	expresionRegular.push_back(c);
	estados.push_back(estadoActual);
	estadoActual++;
	estados.push_back(estadoActual);
	estadoActual++;
	inicial = estados.front();
	aceptacion.push_back(estados.back());
	entradas.push_back(c);
	transiciones.push_back(make_tuple(inicial,c,aceptacion.front()));
}

void Automata::printAutomata(){
	cout<<"Automata de "<<expresionRegular<<endl;
	cout<<"Estados"<<endl;
	cout<<estados.size()<<endl;
	for(IdEstado id : estados){
		cout<<id<<" ";
	}
	cout<<endl;
	cout<<"Inicial"<<endl;
	cout<<inicial<<endl;
	cout<<"Aceptacion"<<endl;
	cout<<aceptacion.size()<<endl;
	for(IdEstado id : aceptacion){
		cout<<id<<" ";
	}
	cout<<endl;
	cout<<"Entradas"<<endl;
	cout<<entradas.size()<<endl;
	for(char c : entradas){
		cout<<c<<' ';
	}
	cout<<endl;
	cout<<"Transiciones"<<endl;
	cout<<transiciones.size()<<endl;
	int id1 = -1;
	int id2 = -1;
	char c;
	for(Transicion tran : transiciones){
		tie(id1,c,id2) = tran;
		cout<<id1<<" "<<c<<" "<<id2<<endl;
	}
}




#endif