#ifndef GENETICO_H
#define GENETICO_H

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <tuple>
#include "Individuo.h"


//TODO INicializar el srand(time(NULL)) en el main

using namespace std;

typedef tuple<int,int> Pareja;
typedef function<void(Individuo &)> FunMutar;
typedef function<vector<Individuo>(Individuo,Individuo)> FunCrossover;

class Genetico{
	public:
		Genetico(){};
		Genetico(int sizePoblacion, vector<int> sizeCromo, int nFen, Prob probMutacion, FunFenotipos funFen, 
			FunFitness funFit, FunMutar funMutar, FunCrossover funCross);
		Individuo run(int iteraciones);
		int binary(Val e, int ini, int end);
		void selection();
		Pareja emparejar();
		void crossover(int madre, int padre, vector<Individuo> &res);
		void mutarPobl(vector<Individuo> &nextGeneration);
		void masacre(vector<Individuo> &nextGeneration);
		void print();
		void print(vector<Individuo> &nextGeneration);
		FunFenotipos funFen;
		FunFitness funFit;
		FunMutar funMutar;
		FunCrossover funCross;
		vector<Individuo> poblacion;
		Prob probMutacion;
		vector<int> sizeCromos;
		int nFen;
		int sizePoblacion;
};

Genetico::Genetico(int sizePoblacion, vector<int> sizeCromos, int nFen, Prob probMutacion, FunFenotipos funFen,
		FunFitness funFit, FunMutar funMutar, FunCrossover funCross){
	this->sizePoblacion = sizePoblacion;
	this->sizeCromos = sizeCromos;
	this->nFen = nFen;
	this->probMutacion = probMutacion;
	this->funFit = funFit;
	this->funFen = funFen;
	this->funMutar = funMutar;
	this->funCross = funCross;
	for(int i = 0; i < sizePoblacion; i++){
		poblacion.push_back(Individuo(sizeCromos, nFen, funFen, funFit));
	}
}

int Genetico::binary(Prob e, int ini, int end){
	if(ini == end){
		if(poblacion[ini].probSelecAcum >= e) return ini;
		return ini + 1; 
	}
	if(end - ini == 1){
		if(poblacion[end].probSelecAcum <= e) return end + 1;
		if(poblacion[ini].probSelecAcum < e) return end;
		return ini;
	}
	int med = (end + ini) / 2;
	if(poblacion[med].probSelecAcum == e) return med;
	if(poblacion[med].probSelecAcum > e) return binary(e,ini,med - 1);
	return binary(e,med,end);
}

Pareja Genetico::emparejar(){
	Prob randMadre = ((Prob) rand() / RAND_MAX);
	Prob randPadre = ((Prob) rand() / RAND_MAX);
	int madre = binary(randMadre, 0, sizePoblacion - 1);
	int padre = binary(randPadre, 0, sizePoblacion - 1);
	return make_tuple(madre,padre);
}


void Genetico::mutarPobl(vector<Individuo> &nextGeneration){
	float random = 0;
	for(int i = 0 ; i < nextGeneration.size(); i++){
		random = ((Val) rand() / RAND_MAX);
		if(random <= probMutacion){
			funMutar(nextGeneration[i]);	
			nextGeneration[i].evaluar(funFen, funFit);
		} 
	}
}

void Genetico::masacre(vector<Individuo> &nextGeneration){
	sort(nextGeneration.begin(), nextGeneration.end(), funSortIn);
	auto ini = nextGeneration.begin();
	auto end = nextGeneration.begin() + sizePoblacion;
	poblacion = vector<Individuo>(ini,end);
}

void Genetico::selection(){
	Val fitnessTotal = 0;
	Val probSelecTotal = 0;
	vector<Individuo> nextGeneration;
	for(Individuo i : poblacion){
		fitnessTotal += i.fitness;
	}
	for(int i = 0; i < sizePoblacion; i++){
		poblacion[i].probSelec = poblacion[i].fitness / fitnessTotal;
		probSelecTotal += poblacion[i].probSelec;
		poblacion[i].probSelecAcum = probSelecTotal;
	}
	vector<Individuo> hijos;
	for(int i = 0; i < sizePoblacion; i++){
		int padre = 0;
		int madre = 0;
		tie(madre,padre) = emparejar();
		hijos = funCross(poblacion[madre],poblacion[padre]);
		for(Individuo in : hijos){
			nextGeneration.push_back(in);
		}
	}
	mutarPobl(nextGeneration);
	masacre(nextGeneration);
}


Individuo Genetico::run(int iteraciones){
	for(int i = 0; i < iteraciones; i++){
		selection();
	}
	return poblacion[0];
}


void Genetico::print(){
	for(Individuo i : poblacion){
		i.print();
	}
}

void Genetico::print(vector<Individuo> &nextGeneration){
	for(Individuo i : nextGeneration){
		i.print();
	}	
}


#endif