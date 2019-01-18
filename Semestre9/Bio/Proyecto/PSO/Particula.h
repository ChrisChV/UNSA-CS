#ifndef PARTICULA_H
#define PARTICULA_H

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

class Particula{
public:
	Particula(){};
	Particula(int n);
	void setFitness();
	void print();
	void setVelocidades(vector<double> & mejorGlobal, bool flag);
	void setNewPosiciones(bool flag);

	vector<double> posiciones;
	vector<double> velocidades;
	vector<double> mejorLocal;
	double fitness;
	double mejorFitnessLocal;
};

Particula::Particula(int n){
	double numRandom = 0;
	int signo = 0;
	for(int i = 0; i < n; i++){
		numRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0));
		signo = rand() % 2;
		if(signo == 1) numRandom *=-1;
		posiciones.push_back(numRandom);
		numRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0));
		signo = rand() % 2;
		if(signo == 1) numRandom *=-1;
		velocidades.push_back(numRandom);
	}
	setFitness();
}

void Particula::print(){
	for(double pos : posiciones){
		cout<<pos<<" ";
	}
	for(double vel : velocidades){
		cout<<vel<<" ";
	}
	cout<<fitness<<endl;
}

void Particula::setFitness(){
	fitness = pow(posiciones[0],2) + pow(posiciones[1],2);
}

void Particula::setVelocidades(vector<double> & mejorGlobal, bool flag){
	double w = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	double phi1 = 2.0;
	double phi2 = 2.0;
	double rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	double rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	for(int i = 0; i < velocidades.size(); i++){
		if(flag == true){
			velocidades[i] = w * velocidades[i] + phi2 * rand2 * (mejorGlobal[i] - posiciones[i]);
		}
		else{
			velocidades[i] = w * velocidades[i] + phi1 * rand1 * (mejorLocal[i] - posiciones[i]) + phi2 * rand2 * (mejorGlobal[i] - posiciones[i]);
		}
		
	}
}

void Particula::setNewPosiciones(bool flag){
	for(int i = 0; i < posiciones.size(); i++){
		posiciones[i] = posiciones[i] + velocidades[i];
	}
	setFitness();
	if(flag == true or mejorFitnessLocal > fitness){
		mejorLocal = posiciones;
		mejorFitnessLocal = fitness;
	}
}

#endif