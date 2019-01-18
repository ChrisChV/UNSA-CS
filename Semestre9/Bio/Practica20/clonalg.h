#ifndef CLONALG_H
#define CLONALG_H

#include <iostream>
#include <algorithm>
#include "leucocito.h"

using namespace std;

class Clonalg{
	public:
		Clonalg(){}
		void run(int iteraciones, int tamPobl, int numRandoms, TypeVal mutationFactor,
					 TypeVal cloneRate, FunFit funFit);
		vector<Leucocito> poblacion;
};


void Clonalg::run(int iteraciones, int tamPobl, int numRandoms, TypeVal mutationFactor,
					 TypeVal cloneRate, FunFit funFit){
	int cantidadClones = tamPobl * cloneRate;
	for(int i = 0; i < tamPobl; i++){
		poblacion.push_back(Leucocito(mutationFactor, cantidadClones, funFit));
	}

	TypeVal maxFit = -1;
	TypeVal minFit = -1;
	vector<Leucocito> nuevaPobl;

	for(int i = 0; i < iteraciones; i++){
		cout<<"Iteracion "<<i + 1<<endl;
		maxFit = -1;
		minFit = -1;
		for(auto iter = poblacion.begin(); iter != poblacion.end(); ++iter){
			iter->setFitness();
			if(maxFit == -1 or maxFit < iter->fitness) maxFit = iter->fitness;
			if(minFit == -1 or minFit > iter->fitness) minFit = iter->fitness;
		}

		for(auto iter = poblacion.begin(); iter != poblacion.end(); ++iter){
			iter->setAfinidad(maxFit, minFit);
			iter->setClones();
			nuevaPobl.push_back((*iter));
			nuevaPobl.insert(nuevaPobl.end(), iter->clones.begin(), iter->clones.end());
			iter->print();
			cout<<"MutationRate = "<<iter->mutationRate<<endl;
			iter->printClones();
		}

		cout<<endl;
		cout<<"Aleatorios"<<endl;

		for(int j = 0; j < numRandoms; j++){
			nuevaPobl.push_back(Leucocito(mutationFactor, cantidadClones, funFit));
			nuevaPobl.back().setFitness();
			nuevaPobl.back().print();
		}
		sort(nuevaPobl.begin(), nuevaPobl.end(), [] (Leucocito a, Leucocito b){
			return a.fitness < b.fitness;
		});
		nuevaPobl.erase(nuevaPobl.begin() + tamPobl, nuevaPobl.end());
		poblacion = nuevaPobl;

		cout<<"Nueva Poblacion"<<endl;

		for(auto iter = poblacion.begin(); iter != poblacion.end(); ++iter){
			iter->print();
		}
	}
}

#endif