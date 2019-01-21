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
		void run(FitFun fitFun, LimitFun limitFun, ConstructFuenteFun constructFuenteFun,
					 int numFuentes, int numObs, int numIter, int tamColmena);
		int searchAleatorio();
		int getMejorGlobal();
		vector<Fuente> fuentes;
		Fuente mejorGlobal;

};


int ABC::searchAleatorio(){
	TypeDVal numRand = static_cast<double> (rand()) / (static_cast<double> (RAND_MAX));
	for(int i = 0; i < fuentes.size(); i++){
		if(numRand <= fuentes[i].probAcumulada) return i;
	}
	return fuentes.size() - 1;
}

int ABC::getMejorGlobal(){
	int iMejor = -1;
	int fitnessMejor = -1;
	for(int i = 0; i < fuentes.size(); i++){
		if(fitnessMejor == -1 or fuentes[i].fitness < fitnessMejor){
			iMejor = i;
			fitnessMejor = fuentes[i].fitness;
		}
	}
	return iMejor;
}

void ABC::run(FitFun fitFun,LimitFun limitFun, ConstructFuenteFun constructFuenteFun,
					 int numFuentes, int numObs, int numIter, int tamColmena){
	for(int i = 0; i < numFuentes; i++){
		fuentes.push_back(constructFuenteFun());
		fuentes.back().setFitness();
	}
	int numVal = fuentes.back().valores.size();
	int limite = (tamColmena * numVal) / 2;



	int j = 0;
	int k = 0;
	TypeVal KVal = 0;
	TypeDVal sumFit = 0;
	TypeDVal probAcumulada = 0;
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

		for(auto iter = fuentes.begin(); iter != fuentes.end(); ++iter){
			j = rand() % numVal;
			k = rand() % numFuentes;
			KVal = fuentes[k].valores[j];	
			iter->getCandidatos(KVal, j);
			if(limitFun(*iter) == false){
				(*iter) = constructFuenteFun();
				iter->setFitness();
			} 
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
			KVal = fuentes[k].valores[j];
			_i = searchAleatorio();
			fuentes[_i].getCandidatos(KVal, j);
			if(limitFun(fuentes[_i]) == false){
				fuentes[_i] = constructFuenteFun();
				fuentes[_i].setFitness();
			}
			cout<<"Fuente escojida:"<<_i<<endl;
			fuentes[_i].print();
			cout<<endl;
		}
		for(auto iter = fuentes.begin(); iter != fuentes.end(); ++iter){
			if(iter->count >= limite){
				iter->reemplazar();
				if(limitFun(*iter) == false){
					(*iter) = constructFuenteFun();
					iter->setFitness();
				} 
			}
		}
		//sort(fuentes.begin(), fuentes.end(), [] (Fuente a, Fuente b){
		//	return a.fitness > b.fitness;
		//});
		iMejor = getMejorGlobal();
		if(i == 0 or fuentes[iMejor].fitness < mejorGlobal.fitness) mejorGlobal = fuentes[iMejor];
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