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
typedef function<FitVal(Cromosoma)> AlgoGenFitnessFunc;
typedef tuple<Cromosoma,Cromosoma> ParCromo;
typedef function<vector<ParCromo>(Poblacion, int)> AlgoGenSelectFunc;
typedef function<int(vector<ParCromo>, float, Poblacion &)> AlgoGenCrossFunc; //hacer push
typedef function<void(Poblacion &, float)> AlgoGenMutFunc;
typedef function<Poblacion(Poblacion)> AlgoGenNextPoblFunc;
typedef function<bool(Cromosoma)> AlgoGenLimit;
typedef function<Poblacion(int)> AlgoGenInitPobl;

class Cromosoma{
	public:
		Cromosoma(){}
		Cromosoma(int tamCrom);
		Cromosoma(string crom){this->crom = crom;}

		int bin_to_int();
		int tamCrom;
		int probabilidad;
		FitVal fit;
		string crom;
};

Cromosoma::Cromosoma(int tamCrom){
	this->tamCrom = tamCrom;
	int numRandom = 0;
	for(int i = 0; i < tamCrom; i++){
		numRandom = rand() % 2;
		if(numRandom == 0) crom.push_back('0');
		else if(numRandom == 1) crom.push_back('1');
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
		AlgoGen(int tamP, int tamCrom, int numIter, float probCruz, float probMut,
				AlgoGenFitnessFunc fitness, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl, AlgoGenLimit limitFun, AlgoGenInitPobl initPbl);
		void run();
	private:
		void initPoblacion();
		int tamP;
		int tamCrom;
		int numIter;
		float probCruz;
		int puntoCruz;
		float probMut;
		AlgoGenFitnessFunc fitness;
		AlgoGenSelectFunc select;
		AlgoGenCrossFunc cross;
		AlgoGenMutFunc mut;
		AlgoGenNextPoblFunc next;
		AlgoGenLimit limitFun;
		AlgoGenInitPobl initPbl;
		Poblacion pobl;
};

AlgoGen::AlgoGen(int tamP, int tamCrom, int numIter, float probCruz, float probMut,
				AlgoGenFitnessFunc fitness, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl, AlgoGenLimit limitFun, AlgoGenInitPobl initPbl){
	this->tamP = tamP;
	this->tamCrom = tamCrom;
	this->numIter = numIter;
	this->probCruz = probCruz;
	this->probMut = probMut;
	this->fitness = fitness;
	this->select = select;
	this->cross = cross;
	this->mut = mut;
	this->next = getNextPobl;
	this->limitFun = limitFun;
	this->initPbl = initPbl;
	pobl = initPbl(tamP);
	//initPoblacion();
}

void AlgoGen::initPoblacion(){
	cout<<"Generando Poblacion Inicial"<<endl;
	Cromosoma newCrom;
	for(int i = 0; i < tamP; i++){
		newCrom = Cromosoma(tamCrom);
		if(limitFun(newCrom) == false){
			i--;
			continue;
		}
		pobl.push_back(newCrom);
	}
	int actual = 1;
	for(Cromosoma c : pobl){
		cout<<actual++<<")\t"<<c.crom<<endl;
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
				if(limitFun(pobl[j]) == false) pobl[j].fit = -1;
				else pobl[j].fit = fitness(pobl[j]);
			}
			actual = 1;
			for(Cromosoma c : pobl){
				cout<<actual++<<")\t"<<c.crom<<"\t"<<c.fit<<endl;
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
			cout<<actual++<<")\t"<<c.crom<<"\t"<<c.fit<<endl;
		}
		cout<<endl;
		if(i == numIter) break;
		padres = select(pobl, tamP);
		newPobl.clear();
		int faltantes = cross(padres, probCruz, newPobl);
		while(faltantes != 0){
			cout<<"Faltan elegir "<<faltantes<<" padres"<<endl;
			padres = select(pobl, faltantes);
			faltantes = cross(padres, probCruz, newPobl);
		}
		mut(newPobl, probMut);
		pobl = newPobl;
	}
	cout<<"Fin del proceso"<<endl;
}

#endif