#ifndef PSO_H
#define PSO_H

#include <iostream>
#include <algorithm>
#include "Particula.h"


using namespace std;

class PSO{
public:
	PSO(int numPar, int numIter);
	void verifyFronteraGlobal(Particula newP);
	void run();
	vector<Particula> particulas;
	vector<Particula> mejorGlobal;
	int numPar;
	int numIter;
};


PSO::PSO(int numPar, int numIter){
	this->numPar = numPar;
	this->numIter = numIter;
}

void PSO::verifyFronteraGlobal(Particula newP){
	vector<Particula> res;
	bool flag = true;
	int dom = 0;
	for(Particula p : mejorGlobal){
		dom = dominancia(newP, p);
		if(dom == B_DOMINA_A){
			flag = false;
			res.push_back(p);
		} 
		else if(dom == MISMA_FRONTERA) res.push_back(p);
	}
	if(flag == true) res.push_back(newP);
	mejorGlobal = res;
}

void PSO::run(){
	Particula randMejorGlobal;
	int numRandom = 0;
	for(int i = 0; i < numPar; i++){
		particulas.push_back(Particula(2));
	}
	for(int i = 0; i < numIter; i++){
		cout<<"Iteración "<<i<<endl;
		for(Particula p : particulas){
			verifyFronteraGlobal(p);
		}
		sort(mejorGlobal.begin(), mejorGlobal.end(), [] (Particula a, Particula b){
			return a.fitness[0] < b.fitness[0];
		});

		cout<<endl;

		int count = 1;
		for(Particula p : particulas){
			cout<<count++<<endl;
			p.print(true);
			cout<<endl;
		}

		cout<<"Frontera Mejor Global: "<<endl;

		for(Particula p : mejorGlobal){
			p.print(false);
		}
		
		numRandom = rand() % mejorGlobal.size();
		randMejorGlobal = mejorGlobal[numRandom];

		for(auto iter = particulas.begin(); iter != particulas.end(); ++iter){
			if(i == 0){
				(*iter).setVelocidades(randMejorGlobal.posiciones, true);
				(*iter).setNewPosiciones(true);
			} 
			else{
				(*iter).setVelocidades(randMejorGlobal.posiciones, false);	
				(*iter).setNewPosiciones(false);
			}
		}
	}
}

#endif