#ifndef PSO_H
#define PSO_H

#include <iostream>
#include <algorithm>
#include "Particula.h"


using namespace std;

class PSO{
public:
	PSO(int numPar, int numIter);
	vector<Particula> particulas;
	Particula mejorGlobal;
	void run(matrix & costos);
	int numPar;
	int numIter; 
};


PSO::PSO(int numPar, int numIter){
	this->numPar = numPar;
	this->numIter = numIter;
}

void PSO::run(matrix & costos){
	for(int i = 0; i < numPar; i++){
		particulas.push_back(Particula(costos,5));
	}
	for(int i = 0; i < numIter; i++){
		cout<<"-----------Iteracion "<<i+1<<"-------------------"<<endl;
		sort(particulas.begin(), particulas.end(), [](Particula a, Particula b){
			return a.fitness < b.fitness;
		});
		if(i == 0) mejorGlobal = particulas.front();
		else if(mejorGlobal.fitness > particulas.front().fitness) mejorGlobal = particulas.front();

		cout<<"Mejor Global: ";

		mejorGlobal.print();
		cout<<endl;

		int count = 1;
		for(Particula p : particulas){
			cout<<count++<<endl;
			p.print();
			cout<<endl;
		}
		
		for(auto iter = particulas.begin(); iter != particulas.end(); ++iter){
			(*iter).setVelocidades(mejorGlobal.camino);
			(*iter).setNewPosiciones(costos);
		}		
	}
}

#endif