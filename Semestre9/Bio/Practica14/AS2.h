#ifndef AS_H
#define AS_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "Hormiga2.h"

using namespace std;

class AS{
	public:
		AS(){}
		AS(matrix & costos, matrix & flujos ,double femIni, double alfa, double delta, double Q ,double evaporacion
			,int numH, int numIter, int numEstIter);
		void mutacion();
		double getUmbral();
		void run();
		matrix costos;
		matrix flujos;
		matrix distancias;
		matrix feromonas;
		int numH;
		int numIter;
		int numEstIter;
		double evaporacion;
		double alfa;
		double beta;
		double Q;
		double ferInit;
		Hormiga * mejorGlobal;
		bool mejorflag;

};

AS::AS(matrix & costos, matrix & flujos , double femIni, double alfa, double delta, double Q ,double evaporacion , 
		  int numH, int numIter, int numEstIter){
	this->costos = costos;
	this->flujos = flujos;
	feromonas = matrix(costos.size());
	for(int i = 0; i < costos.size(); i++){
		feromonas[i] = vector<double>(costos.size(), femIni);
	}
	this->numH = numH;
	this->numIter = numIter;
	this->numEstIter = numEstIter;
	this->evaporacion = evaporacion;
	this->alfa = alfa;
	this->beta = delta;
	this->Q = Q;
	this->ferInit = femIni;
	mejorflag = false;
}

/*void parallelHormiga(Hormiga * hormiga, matrix * distancias, matrix * feromonas, matrix * costos,
						 double alfa, double beta, double Q){
	hormiga->getCamino(*distancias, *feromonas, alfa, beta);
	hormiga->setCosto(*costos, Q);
}*/

double normal(double x, double desvio){
	double retorno = -0.5 * (pow(x / desvio, 2));
	retorno = exp(retorno);
	retorno = retorno / (desvio * sqrt(6.283284));
	return retorno;
}

double valor_x(double lim_inf, double lim_sup, double desvio, double delta, double aleatorio){
	double area = 0;
	double aux_suma, aux = normal(lim_inf, desvio);
	for(double i = lim_inf + delta; i < lim_sup; i+= delta){
		aux_suma = normal(i, desvio);
		area += (aux + aux_suma);
		if((area * (delta / 2)) > aleatorio) return i;
		aux = aux_suma;
	}
	return -1;
}

bool findCamino(Hormiga * ant, int val1, int val2){
	int estadoAns = -1;
	for(auto iter = ant->res.begin(); iter != ant->res.end(); ++iter){
		if(estadoAns == -1){
			estadoAns = (*iter);
			continue;
		}
		if(estadoAns == val1 and (*iter) == val2) return true;
		if(estadoAns == val2 and (*iter) == val1) return true;
		estadoAns = (*iter);
	}
	return false;
}

double AS::getUmbral(){
	double res = 0;
	int estadoAns = -1;
	float count = 0;
	for(auto iter = mejorGlobal->res.begin(); iter != mejorGlobal->res.end(); ++iter){
		if(estadoAns == -1){
			estadoAns = (*iter);
			continue;
		}
		res += feromonas[estadoAns][(*iter)];
		count++;
		estadoAns = (*iter);
	}
	return res / count;
}

void AS::mutacion(){
	matrix probs(feromonas.size());
	for(int i = 0; i < feromonas.size(); i++){
		probs[i] = vector<double>(feromonas.size(), 0);
	}
	for(int i = 0; i < feromonas.size(); i++){
		for(int j = i + 1; j < feromonas.size(); j++){
			probs[i][j] = pow(distancias[i][j],alfa) * pow(feromonas[i][j],beta);
			probs[j][i] = probs[i][j];
		}
	}
	double sum = 0;
	for(int i = 0; i < feromonas.size(); i++){
		sum = 0;
		for(int j = 0; j < feromonas.size(); j++){
			if(i == j) continue;
			sum += probs[i][j];
		}
		for(int j = 0; j < feromonas.size(); j++){
			probs[i][j] = probs[i][j] / sum;
		}
	}
	double umbral = getUmbral();
	cout<<"UMBRAL->"<<umbral<<endl;
	float randNumber = 0;
	for(int i = 0; i < feromonas.size(); i++){
		for(int j = i + 1; j < feromonas.size(); j++){
			if(probs[i][j] < 0.20){
				randNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				probs[i][j] += valor_x(-umbral,umbral,umbral,0.1,randNumber);
			}
		}
	}
}

void AS::run(){
	distancias = matrix(costos.size());
	for(int i = 0; i < costos.size(); i++){
		distancias[i] = vector<double>(costos.size(), 0);
		for(int j = 0; j < costos.size(); j++){
			for(int k = 0; k < costos.size(); k++){
				distancias[i][j] += costos[i][k] * flujos[k][j];
			}
			if(distancias[i][j] == 0) distancias[i][j] = 0.000001;
			else distancias[i][j] = 1.0 / distancias[i][j];
		}
	}

	thread threads[numH];
	vector<Hormiga *> hormigas(numH);
	matrix newFeromonas;
	Hormiga * actualHormiga = nullptr;
	mejorGlobal = nullptr;
	int estadoAns = -1;
	int interEstCount = 0;

	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		for(int j = 0; j < numH; j++){
			cout<<"Hormiga "<<j<<endl;
			hormigas[j] = new Hormiga();
			//threads[j] = thread(parallelHormiga, hormigas[j], &distancias, &feromonas, &costos, alfa, delta, Q);
			hormigas[j]->getCamino(distancias, feromonas, alfa, beta);
			hormigas[j]->setCosto(costos, flujos , Q);
		}
		//for(int j = 0; j < numH; j++){
		//	threads[j].join();
		//}
		sort(hormigas.begin(), hormigas.end(), []	(Hormiga * a, Hormiga * b){
			return a->costo < b->costo;
		});
		if(mejorGlobal == nullptr){
			mejorGlobal = new Hormiga();
			(*mejorGlobal) = (*hormigas.front());
			mejorflag = true;			
		}
		else if(mejorGlobal->costo > hormigas.front()->costo){
			(*mejorGlobal) = (*hormigas.front());	
			mejorflag = true;
		}
		else interEstCount++;

		if(mejorflag == true){
			mejorflag = false;
			interEstCount = 0;
		}
		else if(interEstCount == numEstIter){
			cout<<"REINICIANDO FEROMONAS"<<endl;
			interEstCount = 0;
			feromonas = matrix(costos.size());
			for(int i = 0; i < costos.size(); i++){
				feromonas[i] = vector<double>(costos.size(), ferInit);
			}
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
		for(auto iter = mejorGlobal->res.begin(); iter != mejorGlobal->res.end(); ++iter){
			if(estadoAns == -1){
				estadoAns = (*iter);
				continue;
			}			
			newFeromonas[estadoAns][(*iter)] += mejorGlobal->ferCosto;
			newFeromonas[(*iter)][estadoAns] += mejorGlobal->ferCosto;
			estadoAns = (*iter);
		}

		for(int j = 0; j < feromonas.size(); j++){
			for(int k = 0; k < feromonas.size(); k++){
				if(k == j) continue;
				feromonas[j][k] = feromonas[j][k] * evaporacion +  newFeromonas[j][k];
			}
		}
		estadoAns = -1;
		for(auto iter = hormigas.back()->res.begin(); iter != hormigas.back()->res.end(); ++iter){
			if(estadoAns == -1){
				estadoAns = (*iter);
				continue;
			}
			if(findCamino(mejorGlobal, estadoAns, (*iter)) == true) continue;
			feromonas[estadoAns][(*iter)] = feromonas[estadoAns][(*iter)] * evaporacion;
			feromonas[(*iter)][estadoAns] = feromonas[(*iter)][estadoAns] * evaporacion;
			estadoAns = (*iter);
		}
		mutacion();
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