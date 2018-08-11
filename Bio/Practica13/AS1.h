#ifndef AS_H
#define AS_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "Hormiga.h"

using namespace std;

class AS{
	public:
		AS(){}
		AS(matrix & costos, matrix & flujos, double alfa, double delta, double Q ,double evaporacion,
			double ferMax, double ferMin, int numH, int numIter);
		void run();
		matrix costos;
		matrix flujos;
		matrix distancias;
		matrix feromonas;
		int numH;
		int numIter;
		double evaporacion;
		double alfa;
		double delta;
		double Q;
		double ferMax;
		double ferMin;
		Hormiga * mejorGlobal;
};

AS::AS(matrix & costos, matrix & flujos, double alfa, double delta, double Q ,double evaporacion, 
			double ferMax, double ferMin, int numH, int numIter){
	this->costos = costos;
	this->flujos = flujos;
	feromonas = matrix(costos.size());
	for(int i = 0; i < costos.size(); i++){
		feromonas[i] = vector<double>(costos.size(), ferMax);
	}
	this->numH = numH;
	this->numIter = numIter;
	this->evaporacion = evaporacion;
	this->alfa = alfa;
	this->delta = delta;
	this->Q = Q;
	this->ferMax = ferMax;
	this->ferMin = ferMin;
}

void parallelHormiga(Hormiga * hormiga, matrix * distancias, matrix * feromonas, matrix * costos, matrix * flujos,
						 double alfa, double beta, double Q){
	hormiga->getCamino(*distancias, *feromonas, alfa, beta);
	hormiga->setCosto(*costos, *flujos, Q);
}

void AS::run(){
	distancias = matrix(costos.size());
	for(int i = 0; i < costos.size(); i++){
		distancias[i] = vector<double>(costos.size(), 0);
		for(int j = 0; j < costos.size(); j++){
			for(int k = 0; k < costos.size(); k++){
				distancias[i][j] += costos[i][k] * flujos[k][j];
			}
			distancias[i][j] = 1.0 / distancias[i][j];
		}
	}

	thread threads[numH];
	vector<Hormiga *> hormigas(numH);
	matrix newFeromonas;
	Hormiga * actualHormiga = nullptr;
	int estadoAns = -1;
	mejorGlobal = nullptr;

	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		for(int j = 0; j < numH; j++){
			hormigas[j] = new Hormiga();
			threads[j] = thread(parallelHormiga, hormigas[j], &distancias, &feromonas, &costos, &flujos, alfa, delta, Q);
		}
		for(int j = 0; j < numH; j++){
			threads[j].join();
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
		for(auto iter = hormigas.begin(); iter != hormigas.end(); ++iter){
			(*iter)->print();
		}
		newFeromonas = matrix(costos.size());
		for(int j = 0; j < costos.size(); j++){
			newFeromonas[j] = vector<double>(costos.size(), 0);
		}
		estadoAns = -1;
		for(auto iter = hormigas.front()->res.begin(); iter != hormigas.front()->res.end(); ++iter){
			if(estadoAns == -1){
				estadoAns = (*iter);
				continue;
			}
			newFeromonas[estadoAns][(*iter)] += hormigas.front()->ferCosto;
			newFeromonas[(*iter)][estadoAns] += hormigas.front()->ferCosto;
			estadoAns = (*iter);
		}

		for(int j = 0; j < feromonas.size(); j++){
			for(int k = 0; k < feromonas.size(); k++){
				if(k == j) continue;
				feromonas[j][k] = feromonas[j][k] * evaporacion + newFeromonas[j][k];
				if(feromonas[j][k] < ferMin) feromonas[j][k] = ferMin;
				else if(feromonas[j][k] > ferMax) feromonas[j][k] = ferMax;
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