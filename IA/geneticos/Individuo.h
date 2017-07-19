#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <iostream>
#include <functional>
#include "Cromosoma.h"
#include "sad.h"

#define NUMBER_CROM 2
#define NUMBER_DIV 2

using namespace std;


class Individuo{
	public:
		Individuo();
		Individuo(int size, FunFitness funFit);
		Individuo(int size, char * a1, char * a2, char * b1, char * b2, FunFitness funFit);
		void print();
		void reset(FunFitness funFit);
		Val operator +(Individuo i){return this->fitness + i.fitness;}

		Cromosoma cromosoma[2];
		Val fenotipo[2];
		Val fitness;
		Val probSelec;
		Val probSelecAcum;
};


Individuo::Individuo(){
	cromosoma[0] = Cromosoma();
	cromosoma[1] = Cromosoma();
	fenotipo[0] = 0;
	fenotipo[1] = 0;
	fitness = 0;
	probSelec = 0;
	probSelecAcum = 0;
}

Individuo::Individuo(int size, FunFitness funFit){
	cromosoma[0] = Cromosoma(size);
	cromosoma[1] = Cromosoma(size);
	fenotipo[0] = cromosoma[0].getFenotipo();
	fenotipo[1] = cromosoma[1].getFenotipo();
	fitness = funFit(fenotipo[0],fenotipo[1]);
	probSelec = 0;
	probSelecAcum = 0;
}

Individuo::Individuo(int size, char * a1, char * a2, char * b1, char * b2, FunFitness funFit){
	cromosoma[0] = Cromosoma(size,a1,a2);
	cromosoma[1] = Cromosoma(size,b1,b2);
	fenotipo[0] = cromosoma[0].getFenotipo();
	fenotipo[1] = cromosoma[1].getFenotipo();
	fitness = funFit(fenotipo[0],fenotipo[1]);
	probSelec = 0;
	probSelecAcum = 0;	
}

void Individuo::reset(FunFitness funFit){
	fenotipo[0] = cromosoma[0].getFenotipo();
	fenotipo[1] = cromosoma[1].getFenotipo();
	fitness = funFit(fenotipo[0],fenotipo[1]);
	probSelec = 0;
	probSelecAcum = 0;	
}

void Individuo::print(){
	cromosoma[0].print();
	cout<<" ";
	cromosoma[1].print();
	cout<<" "<<fenotipo[0]<<" "<<fenotipo[1]<<" "<<fitness<<" "<<probSelec<<" "<<probSelecAcum<<endl;
}


bool funSortIn(Individuo a, Individuo b){
	return a.fitness > b.fitness;
}

#endif