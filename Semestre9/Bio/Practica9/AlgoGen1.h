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

typedef float FitVal;
typedef vector<Cromosoma> Poblacion;
typedef vector<float> CromVal;
typedef function<FitVal(Cromosoma)> AlgoGenFitnessFunc;
typedef tuple<Cromosoma,Cromosoma> ParCromo;
typedef function<vector<ParCromo>(Poblacion, int)> AlgoGenSelectFunc;
typedef function<void(ParCromo, Poblacion &)> AlgoGenCrossFunc; //hacer push
typedef function<void(Cromosoma &)> AlgoGenMutFunc;
typedef function<Poblacion(Poblacion)> AlgoGenNextPoblFunc;
typedef function<bool(Cromosoma)> AlgoGenRestrictionFunc;

class Cromosoma{
	public:
		Cromosoma(){}
		Cromosoma(int tamCrom);
		Cromosoma(CromVal crom){this->crom = crom;}
		void printCrom(){
			for(float f : crom){
				cout<<f<<" ";
			}
			for(float f : deltas){
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
		valRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
		if(signo == 1) valRandom *= -1;
		crom.push_back(valRandom);
	}
	for(int i = 0; i < tamCrom; i++){
		valRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
		deltas.push_back(valRandom);
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

class AlgoGen{
	public:
		AlgoGen(int tamP, int tamCrom, int numIter, 
				AlgoGenFitnessFunc fitness, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl);
		void run();
	private:
		void initPoblacion();
		int tamP;
		int tamCrom;
		int numIter;
		int puntoCruz;
		AlgoGenFitnessFunc fitness;
		AlgoGenSelectFunc select;
		AlgoGenCrossFunc cross;
		AlgoGenMutFunc mut;
		AlgoGenNextPoblFunc next;
		//AlgoGenRestrictionFunc restriction;
		Poblacion pobl;
};

AlgoGen::AlgoGen(int tamP, int tamCrom, int numIter, 
				AlgoGenFitnessFunc fitness, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl){
	this->tamP = tamP;
	this->tamCrom = tamCrom;
	this->numIter = numIter;
	this->fitness = fitness;
	this->select = select;
	this->cross = cross;
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
			for(int j = 0; j < pobl.size(); j++){
				pobl[j].fit = fitness(pobl[j]);
			}
			actual = 1;
			for(Cromosoma c : pobl){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<"\t"<<c.fit<<endl;
			}
			pobl = next(pobl);
		}
		cout<<"Iteracion "<<i<<endl<<endl;
		cout<<"Evaluando Individuos"<<endl;
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
		if(i == numIter) break;
		padres = select(pobl, 1);
		newPobl = pobl;
		cross(padres.front(), newPobl);
		mut(newPobl.back());
		pobl = newPobl;
	}
	cout<<"Fin del proceso"<<endl;
}

#endif