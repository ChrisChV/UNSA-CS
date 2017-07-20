#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <iostream>
#include <functional>
#include "Cromosoma.h"
#include "Errors.h"

using namespace std;

typedef float Val;
typedef float Prob;
typedef function<Val*(Cromosoma*)> FunFenotipos;
typedef function<Val(Val*)> FunFitness;

class Individuo{
public:
	Individuo();
	void del(){
		for(int i = 0; i < nCrom; i++){
			cromosomas[i].del();
		}
		delete[] cromosomas;
		delete[] fenotipos;
		
	}
	Individuo(vector<int> cromSizes, int nFen, FunFenotipos funFen, FunFitness funFit);
	Individuo(Cromosoma * cromosomas, int nCrom, int nFen, FunFenotipos funFen, FunFitness funFit);
	void evaluar(FunFenotipos funFen, FunFitness funFit);
	void print();
	Cromosoma * cromosomas;
	Val * fenotipos;
	Val fitness;
	Prob probSelec;
	Prob probSelecAcum;
	int nCrom;
	int nFen;
};

Individuo::Individuo(){
	nCrom = 0;
	nFen = 0;
	cromosomas = nullptr;
	fenotipos = nullptr;
	fitness = 0;
	probSelec = 0;
	probSelecAcum = 0;
}

Individuo::Individuo(vector<int> cromSizes, int nFen, FunFenotipos funFen, FunFitness funFit){
	nCrom = cromSizes.size();
	this->nFen = nFen;
	cromosomas = new Cromosoma[nCrom];
	for(int i = 0; i < nCrom; i++){
		cromosomas[i] = Cromosoma(cromSizes[i]);
	}
	evaluar(funFen, funFit);
}

Individuo::Individuo(Cromosoma * cromosomas, int nCrom, int nFen, FunFenotipos funFen, FunFitness funFit){
	this->nCrom = nCrom;
	this->nFen = nFen;
	this->cromosomas = cromosomas;
	fitness = 0;
	probSelec = 0;
	probSelecAcum = 0;
	evaluar(funFen, funFit);	
}

void Individuo::evaluar(FunFenotipos funFen, FunFitness funFit){
	fenotipos = funFen(cromosomas);
	fitness = funFit(fenotipos);
	probSelec = 0;
	probSelecAcum = 0;
}

void Individuo::print(){
	for(int i = 0; i < nCrom; i++){
		cromosomas[i].print();
		cout<<" ";
	}
	for(int i = 0; i < nFen; i++){
		cout<<fenotipos[i]<<" ";
	}
	cout<<fitness<<" "<<probSelec<<" "<<probSelecAcum<<endl;
}

bool funSortIn(Individuo a, Individuo b){
	return a.fitness > b.fitness;
}

#endif
