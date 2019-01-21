#ifndef PARTICULA_H
#define PARTICULA_H

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

#define A_DOMINA_B 0
#define B_DOMINA_A 1
#define MISMA_FRONTERA 2

class Particula{
public:
	Particula(){};
	Particula(int n);
	Particula(Particula * p){
		this->posiciones = p->posiciones;
		this->velocidades = p->velocidades;
		this->fitness = p->fitness;
	}
	void setFitness();
	void print(bool flag);
	void setVelocidades(vector<double> & mejorGlobal, bool flag);
	void setNewPosiciones(bool flag);
	void verifyFrontera(Particula newP);

	vector<double> posiciones;
	vector<double> velocidades;
	vector<Particula> mejorLocal;
	vector<double> fitness;
};

Particula::Particula(int n){
	double numRandom = 0;
	fitness = vector<double>(2,0);
	numRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0));
	posiciones.push_back(numRandom);
	numRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.0));
	posiciones.push_back(numRandom);
	numRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0));
	velocidades.push_back(numRandom);
	numRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.0));
	velocidades.push_back(numRandom);
	setFitness();
}

int dominancia(Particula a, Particula b){
	if(a.fitness[0] < b.fitness[0] and a.fitness[1] < b.fitness[1]) return A_DOMINA_B;
	if(a.fitness[0] > b.fitness[0] and a.fitness[1] > b.fitness[1]) return B_DOMINA_A;
	return MISMA_FRONTERA;
}


void Particula::print(bool flag){
	for(double pos : posiciones){
		cout<<pos<<" ";
	}
	cout<<" | ";
	for(double vel : velocidades){
		cout<<vel<<" ";
	}
	cout<<" | ";
	for(double fit : fitness){
		cout<<fit<<" ";
	}

	cout<<endl;
	if(flag == true){
		cout<<"Frontera:"<<endl;
		for(Particula p : mejorLocal){
			p.print(false);
		}	
	}
}

void Particula::setFitness(){
	fitness[0] = 4 * pow(posiciones[0],2) + 4 * pow(posiciones[1],2);
	fitness[1] = pow(posiciones[0] - 5, 2) + pow(posiciones[1] - 5, 2);
}

void Particula::verifyFrontera(Particula newP){
	vector<Particula> res;
	bool flag = true;
	int dom = 0;
	for(Particula p : mejorLocal){
		dom = dominancia(newP, p);
		if(dom == B_DOMINA_A){
			flag = false;
			res.push_back(p);
		} 
		else if(dom == MISMA_FRONTERA) res.push_back(p);
	}
	if(flag == true) res.push_back(newP);
	mejorLocal = res;
}

void Particula::setVelocidades(vector<double> & mejorGlobal, bool flag){
	double w = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	double phi1 = 2.0;
	double phi2 = 2.0;
	double rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	double rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	Particula randMejorLocal;
	int randNumber = 0;
	for(int i = 0; i < velocidades.size(); i++){
		if(flag == true){
			velocidades[i] = w * velocidades[i] + phi2 * rand2 * (mejorGlobal[i] - posiciones[i]);
		}
		else{
			randNumber = rand() % mejorLocal.size();
			randMejorLocal = mejorLocal[randNumber];
			velocidades[i] = w * velocidades[i] + phi1 * rand1 * (randMejorLocal.posiciones[i] - posiciones[i]) + phi2 * rand2 * (mejorGlobal[i] - posiciones[i]);
		}
	}
}

void Particula::setNewPosiciones(bool flag){
	for(int i = 0; i < posiciones.size(); i++){
		posiciones[i] = posiciones[i] + velocidades[i];
	}
	if(posiciones[0] < 0 or posiciones[0] > 5) posiciones[0] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0));
	if(posiciones[1] < 0 or posiciones[1] > 3) posiciones[1] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.0));
	setFitness();
	verifyFrontera(Particula(this));
	sort(mejorLocal.begin(), mejorLocal.end(), [](Particula a, Particula b){
		return a.fitness[0] < b.fitness[0];
	});
}

#endif