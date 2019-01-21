#ifndef AS_H
#define AS_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "Hormiga1.h"

using namespace std;

class AS{
	public:
		AS(){}
		AS(matrix & costos, double femIni, double alfa, double delta, double Q ,double evaporacion
			, int numH, int numIter, int nBL);
		void run();
		matrix costos;
		matrix distancias;
		matrix feromonas;
		int numH;
		int numIter;
		int nBL;
		double evaporacion;
		double alfa;
		double beta;
		double Q;
		Hormiga * mejorGlobal;
};

AS::AS(matrix & costos, double femIni, double alfa, double delta, double Q ,double evaporacion , 
			int numH, int numIter, int nBL){
	this->costos = costos;
	feromonas = matrix(costos.size());
	for(int i = 0; i < costos.size(); i++){
		feromonas[i] = vector<double>(costos.size(), femIni);
	}
	this->numH = numH;
	this->numIter = numIter;
	this->nBL = nBL;
	this->evaporacion = evaporacion;
	this->alfa = alfa;
	this->beta = delta;
	this->Q = Q;
}

void parallelHormiga(Hormiga * hormiga, matrix * distancias, matrix * feromonas, matrix * costos,
						 double alfa, double beta, double Q){
	hormiga->getCamino(*distancias, *feromonas, alfa, beta);
	hormiga->setCosto(*costos, Q);
}

void AS::run(){
	distancias = matrix(costos.size());
	for(int i = 0; i < costos.size(); i++){
		distancias[i] = vector<double>(costos.size(), 0);
		for(int j = 0; j < costos.size(); j++){
			if(i == j) continue;
			distancias[i][j] = 1.0 / costos[i][j];
		}
	}

	thread threads[numH];
	vector<Hormiga *> hormigas(numH);
	matrix newFeromonas;
	Hormiga * actualHormiga = nullptr;
	mejorGlobal = nullptr;
	int estadoAns = -1;

	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		for(int j = 0; j < numH; j++){
			cout<<"Hormiga "<<j<<endl;
			hormigas[j] = new Hormiga();
			hormigas[j]->getCamino(distancias, feromonas, alfa, beta);
			hormigas[j]->setCosto(costos, Q);
		}

		for(auto iter = hormigas.begin(); iter != hormigas.end(); ++iter){
			(*iter)->busquedaLocal(costos, Q, nBL);
		}

		sort(hormigas.begin(), hormigas.end(), []	(Hormiga * a, Hormiga * b){
			return a->costo < b->costo;
		});
		if(mejorGlobal == nullptr){
			mejorGlobal = new Hormiga();
			(*mejorGlobal) = (*hormigas.front());
		}
		else if(mejorGlobal->costo > hormigas.front()->costo){
			(*mejorGlobal) = (*hormigas.front());	
		}
		cout<<"Mejor global: ";
		mejorGlobal->print();
		newFeromonas = matrix(costos.size());
		for(int j = 0; j < costos.size(); j++){
			newFeromonas[j] = vector<double>(costos.size(), 0);
		}
		for(auto iter = hormigas.begin(); iter != hormigas.end(); ++iter){
			actualHormiga = (*iter);
			actualHormiga->print();
			estadoAns = -1;
			
			for(auto iter2 = actualHormiga->res.begin(); iter2 != actualHormiga->res.end(); ++iter2){
				if(estadoAns == -1) {
					estadoAns = (*iter2);
					continue;
				}
				newFeromonas[estadoAns][(*iter2)] += actualHormiga->ferCosto;
				newFeromonas[(*iter2)][estadoAns] += actualHormiga->ferCosto;
				estadoAns = (*iter2);
			}
		}
		for(int j = 0; j < feromonas.size(); j++){
			for(int k = 0; k < feromonas.size(); k++){
				if(k == j) continue;
				feromonas[j][k] = feromonas[j][k] * evaporacion + newFeromonas[j][k];
			}
		}
		cout<<"Matrix de feromonas"<<endl;
		for(int j = 0; j < feromonas.size(); j++){
			for(int k = 0; k < feromonas.size(); k++){
				cout<<feromonas[j][k]<<" ";
			}
			cout<<endl;
		}
	}
}



#endif