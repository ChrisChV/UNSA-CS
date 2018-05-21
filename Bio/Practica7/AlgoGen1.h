#ifndef ALGOGEN_H
#define ALGOGEN_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;

class Cromosoma;

typedef double FitVal;
typedef vector<Cromosoma> Poblacion;
typedef vector<double> CromVal;
typedef function<FitVal(Cromosoma&)> AlgoGenFitnessFunc;
typedef tuple<Cromosoma,Cromosoma> ParCromo;
typedef function<Cromosoma(Cromosoma)> AlgoGenMutFunc;
typedef function<Poblacion(Poblacion, Poblacion)> AlgoGenNextPoblFunc;

class Cromosoma{
	public:
		Cromosoma(){}
		Cromosoma(int tamCrom);
		Cromosoma(CromVal crom){this->crom = crom;}
		void printCrom(){
			for(double f : crom){
				cout<<f<<" ";
			}
			for(double f : deltas){
				cout<<f<<" ";
			}
		}
		int bin_to_int();
		int tamCrom;
		int probabilidad;
		FitVal fit;
		CromVal crom;
		CromVal deltas;
};

Cromosoma::Cromosoma(int tamCrom){
	this->tamCrom = tamCrom;
	float valRandom = 0;
	int signo = 0;
	for(int i = 0; i < tamCrom; i++){
		signo = rand() % 2;
		valRandom = rand() % 10;
		if(signo == 0) crom.push_back(valRandom);
		else crom.push_back(-valRandom);
		signo = rand() % 2;
		valRandom = rand() % 10;
		if(signo == 0) deltas.push_back(valRandom);
		else deltas.push_back(-valRandom);
	}
}

int Cromosoma::bin_to_int(){
	int res = 0;
	int actual = 0;
	char c = 0;
	for(int i = crom.size() - 1; i >= 0; i--){
		c = crom[i];
		if(c == '1') res += pow(2, actual);
		actual++;
	}
	return res;
}

bool sortFun(Cromosoma a, Cromosoma b){
	if(isnan(a.fit)) return false;
	return a.fit < b.fit;
}

class AlgoGen{
	public:
		AlgoGen(int tamP, int tamCrom, int numIter, 
				AlgoGenFitnessFunc fitness, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl);
		void run();
	private:
		void initPoblacion();
		int tamP;
		int tamCrom;
		int numIter;
		AlgoGenFitnessFunc fitness;
		AlgoGenMutFunc mut;
		AlgoGenNextPoblFunc next;
		//AlgoGenRestrictionFunc restriction;
		Poblacion pobl;
};

AlgoGen::AlgoGen(int tamP, int tamCrom, int numIter, 
				AlgoGenFitnessFunc fitness, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl){
	this->tamP = tamP;
	this->numIter = numIter;
	this->tamCrom = tamCrom;
	this->fitness = fitness;
	this->mut = mut;
	this->next = getNextPobl;
	initPoblacion();
}

void AlgoGen::initPoblacion(){
	cout<<"Generando Poblacion Inicial"<<endl;
	for(int i = 0; i < tamP; i++){
		pobl.push_back(Cromosoma(tamCrom));
	}
	int actual = 1;
	for(Cromosoma c : pobl){
		cout<<actual++<<")\t";
		c.printCrom();
		cout<<endl;
	}
	cout<<endl;
}

void AlgoGen::run(){
	vector<ParCromo> padres;
	Poblacion newPobl;
	int actual = 1;
	for(int i = 0; i < numIter + 1; i++){
		if(i != 0){
			cout<<"Seleccion de la siguiente poblacion"<<endl;
			cout<<"Poblacion de Padres"<<endl;
			for(int j = 0; j < pobl.size(); j++){
				pobl[j].fit = fitness(pobl[j]);
			}
			actual = 1;
			for(Cromosoma c : pobl){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<"\t"<<c.fit<<endl;
			}
			cout<<endl;
			cout<<"Poblacion de Hijos"<<endl;
			for(int j = 0; j < newPobl.size(); j++){
				newPobl[j].fit = fitness(newPobl[j]);
			}
			actual = 1;
			for(Cromosoma c : newPobl){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<"\t"<<c.fit<<endl;
			}
			cout<<endl;
			pobl = next(pobl, newPobl);
		}
		cout<<"Iteracion "<<i<<endl<<endl;
		cout<<"Evaluando Individuos"<<endl;
		for(int j = 0; j < pobl.size(); j++){
			pobl[j].fit = fitness(pobl[j]);
		}
		sort(pobl.begin(), pobl.end(), sortFun);
		actual = 1;
		for(Cromosoma c : pobl){
			cout<<actual++<<")\t";
			c.printCrom();
			cout<<"\t"<<c.fit<<"\t";
			cout<<endl;
		}
		cout<<endl;
		if(i == numIter) break;
		newPobl.clear();
		for(int i = 0; i < pobl.size(); i++){
			newPobl.push_back(mut(pobl[i]));
		}
	}
	cout<<"Fin del proceso"<<endl;
}

#endif