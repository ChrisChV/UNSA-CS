#ifndef ALGOGEN_H
#define ALGOGEN_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <algorithm>
#include <vector>
#include "PGTree.h"

using namespace std;

typedef PGTree Cromosoma;
typedef double FitVal;
typedef vector<Cromosoma> Poblacion;
typedef tuple<Cromosoma,Cromosoma> ParCromo;
typedef function<Cromosoma(Poblacion)> RepFunc;
typedef function<ParCromo(Poblacion)> AlgoGenSelectFunc;
typedef function<ParCromo(ParCromo)> AlgoGenCrossFunc;
typedef function<Cromosoma(Cromosoma)> AlgoGenMutFunc;
typedef function<void(Poblacion &, int)> AlgoGenNextFunc;

class AlgoGen{
	public:
		AlgoGen(int tamP, int numIter, float probRep ,float probCruz, float probMut,
				RepFunc repFunc, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextFunc next, Variables variables, Constantes constantes, Funciones funciones);
		void run(Entradas entradas, Entradas deseados);
	private:
		void initPoblacion(Variables variables, Constantes constantes, Funciones funciones);
		int tamP;
		int numIter;
		float probRep;
		float probCruz;
		int puntoCruz;
		float probMut;
		RepFunc repFunc;
		AlgoGenCrossFunc cross;
		AlgoGenMutFunc mut;
		AlgoGenSelectFunc select;
		AlgoGenNextFunc next;
		Poblacion pobl;
		Entradas entradas;
		Entradas deseados;
};

AlgoGen::AlgoGen(int tamP, int numIter, float probRep ,float probCruz, float probMut,
				RepFunc repFunc, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextFunc next, Variables variables, Constantes constantes, Funciones funciones){
	this->tamP = tamP;
	this->numIter = numIter;
	this->probRep = probRep;
	this->probCruz = probCruz;
	this->probMut = probMut;
	this->repFunc = repFunc;
	this->select = select;
	this->cross = cross;
	this->mut = mut;
	this->next = next;
	initPoblacion(variables, constantes, funciones);
}

void AlgoGen::initPoblacion(Variables variables, Constantes constantes, Funciones funciones){
	cout<<"Generando Poblacion Inicial"<<endl;
	for(int i = 0; i < tamP; i++){
		pobl.push_back(Cromosoma(variables, constantes, funciones));
	}
	int actual = 1;
	for(Cromosoma c : pobl){
		cout<<actual++<<")\t";
		c.print();
	}
	cout<<endl;
}

void AlgoGen::run(Entradas entradas, Entradas deseados){
	this->entradas = entradas;
	this->deseados = deseados;
	vector<ParCromo> padres;
	Poblacion newPobl;
	int actual = 1;
	for(int i = 0; i < numIter + 1; i++){
		cout<<"Iteracion "<<i<<endl<<endl;
		cout<<"Evaluando Individuos"<<endl;
		for(int j = 0; j < pobl.size(); j++){
			pobl[j].fitness(entradas, deseados);
		}
		actual = 1;
		for(Cromosoma c : pobl){
			cout<<actual++<<")\t";
			c.print();
		}
		cout<<endl;
		if(i == numIter) break;
		int rep = probRep * 100;
		int crossP = rep + (probCruz * 100);
		int mutP = 100;
		int numRandom = rand() % 101;
		newPobl.clear();
		while(newPobl.size() < tamP){
			if(numRandom <= rep){
				newPobl.push_back(repFunc(pobl));
			}
			else if(numRandom <= crossP){
				Cromosoma hijo1;
				Cromosoma hijo2;
				tie(hijo1, hijo2) = cross(select(pobl));
				newPobl.push_back(hijo1);
				newPobl.push_back(hijo2);
			}
			else if(numRandom <= mutP){
				newPobl.push_back(mut(repFunc(pobl)));
			}	
		}
		for(int j = 0; j < newPobl.size(); j++){
			newPobl[j].fitness(entradas, deseados);
		}
		next(newPobl, tamP);
		for(Cromosoma crom : pobl){
			crom.deleteTree();
		}
		pobl = newPobl;
	}
	cout<<"Fin del proceso"<<endl;
}

#endif