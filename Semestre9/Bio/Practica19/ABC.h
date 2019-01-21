#ifndef ABC_H
#define ABC_H

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "Fuente.h"

using namespace std;

class ABC{
	public:
		ABC(){};
		void run(FitFun fitFun, int numVal, int numFuentes, int numObs, int numIter, int tamColmena);
		int searchAleatorio();
		int getMejorGlobal();
		vector<Fuente> fuentes;
		Fuente mejorGlobal;

};


int ABC::searchAleatorio(){
	TypeVal numRand = static_cast<double> (rand()) / (static_cast<double> (RAND_MAX));
	for(int i = 0; i < fuentes.size(); i++){
		if(numRand <= fuentes[i].probAcumulada) return i;
	}
	return fuentes.size() - 1;
}

int ABC::getMejorGlobal(){
	int iMejor = -1;
	int fitnessMejor = -1;
	for(int i = 0; i < fuentes.size(); i++){
		if(fitnessMejor == -1 or fuentes[i].fitness > fitnessMejor){
			iMejor = i;
			fitnessMejor = fuentes[i].fitness;
		}
	}
	return iMejor;
}

void ABC::run(FitFun fitFun, int numVal, int numFuentes, int numObs, int numIter, int tamColmena){
	int limite = (tamColmena * numVal) / 2;

	for(int i = 0; i < numFuentes; i++){
		fuentes.push_back(Fuente(fitFun, numVal));
		fuentes.back().setFitness();
	}

	int j = 0;
	int k = 0;
	TypeVal KVal = 0;
	TypeVal sumFit = 0;
	TypeVal probAcumulada = 0;
	int _i = 0;
	int iMejor = 0;


	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i+1<<endl;
		sumFit = 0;
		probAcumulada = 0;
		cout<<"Fuentes"<<endl;
		for(int f = 0; f < fuentes.size(); f++){
			fuentes[f].print();
		}

		_i = 0;
		for(auto iter = fuentes.begin(); iter != fuentes.end(); ++iter, _i++){
			j = rand() % numVal;
			k = rand() % numFuentes;
			if(_i == k) k = (k + 1) % numFuentes;
			KVal = fuentes[k].valores[j];
			iter->getCandidatos(KVal, j);
			sumFit += iter->fitness;
		}
		cout<<endl;
		cout<<"Luego de abejas empleadas"<<endl;
		for(int f = 0; f < fuentes.size(); f++){
			fuentes[f].print();
		}
		cout<<endl;

		for(auto iter = fuentes.begin(); iter != fuentes.end(); ++iter){
			iter->setProbabilidad(sumFit);
			probAcumulada = iter->probabilidad;
			iter->probAcumulada = probAcumulada;
		}
		fuentes.back().probAcumulada = 1.0;
		for(int l = 0; l < numObs; l++){
			cout<<"Observadora "<<l+1<<endl;
			j = rand() % numVal;
			k = rand() % numFuentes;
			if(_i == k) k = (k + 1) % numFuentes;
			KVal = fuentes[k].valores[j];
			_i = searchAleatorio();
			cout<<"Fuente escojida:"<<_i<<endl;
			fuentes[_i].getCandidatos(KVal, j);
			fuentes[_i].print();
			cout<<endl;
		}
		for(auto iter = fuentes.begin(); iter != fuentes.end(); ++iter){
			if(iter->count >= limite){
				iter->reemplazar();
			}
		}
		//sort(fuentes.begin(), fuentes.end(), [] (Fuente a, Fuente b){
		//	return a.fitness > b.fitness;
		//});
		iMejor = getMejorGlobal();
		if(i == 0 or fuentes[iMejor].fitness > mejorGlobal.fitness) mejorGlobal = fuentes[iMejor];
	}

	cout<<"Fuentes Finales"<<endl;
	for(int f = 0; f < fuentes.size(); f++){
		fuentes[f].print();
	}
	cout<<endl;
	cout<<"Mejor Global"<<endl;
	mejorGlobal.print();
}


#endif