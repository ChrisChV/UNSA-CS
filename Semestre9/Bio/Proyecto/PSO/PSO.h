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
	void run();
	int numPar;
	int numIter; 
};


PSO::PSO(int numPar, int numIter){
	this->numPar = numPar;
	this->numIter = numIter;
}

void PSO::run(){
	for(int i = 0; i < numPar; i++){
		particulas.push_back(Particula(2));
	}
	for(int i = 0; i < numIter; i++){
		sort(particulas.begin(), particulas.end(), [](Particula a, Particula b){
			return a.fitness < b.fitness;
		});
		if(i == 0) mejorGlobal = particulas.front();
		else if(mejorGlobal.fitness > particulas.front().fitness) mejorGlobal = particulas.front();

		cout<<"Mejor Global: ";

		mejorGlobal.print();

		int count = 1;
		for(Particula p : particulas){
			cout<<count++<<"\t";
			p.print();
		}
		
		for(auto iter = particulas.begin(); iter != particulas.end(); ++iter){
			if(i == 0){
				(*iter).setVelocidades(mejorGlobal.posiciones, true);
				(*iter).setNewPosiciones(true);
			} 
			else{
				(*iter).setVelocidades(mejorGlobal.posiciones, false);	
				(*iter).setNewPosiciones(false);
			} 	
		}		
	}
}

#endif