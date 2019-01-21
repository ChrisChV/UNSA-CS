#ifndef HORMIGA_H
#define HORMIGA_H


#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

typedef vector<vector<double>> matrix;

enum VALS{A, B, C, D, E, F, G, H, I, J};

class Hormiga{
	public:
		Hormiga(){
			costo = 0;
		};
		void getCamino(matrix & distancias, matrix & feromonas, double alfa, double delta);
		void setCosto(matrix & costos, matrix & flujos ,double Q);
		void print();
		vector<bool> flags;
		vector<int> res;
		double costo;
		double ferCosto;
};

void Hormiga::print(){
	for(auto iter = res.begin(); iter != res.end(); ++iter){
		cout<<(*iter)<<" ";
	}
	cout<<" "<<costo<<" "<<ferCosto<<endl;
}

int getForProb(vector<double> & probs, float rand){
	double sum = 0;
	for(int i = 0; i < probs.size(); i++){
		if(probs[i] == 0) continue;
		sum += probs[i];
		if(rand <= sum) return i;
	}
	return probs.size() - 1;
}

void Hormiga::setCosto(matrix & distancias, matrix & flujos, double Q){
	costo = 0;
	int count = 0;
	int actualCount = 0;
	for(auto iter = res.begin(); iter != res.end(); ++iter, actualCount++){
		count = 0;		
		for(auto iter2 = res.begin(); iter2 != res.end(); ++iter2, count++){
			if(iter == iter2) continue;
			costo += flujos[actualCount][count] * distancias[(*iter)][(*iter2)];
		}
	}
	ferCosto = Q / costo;
}

void Hormiga::getCamino(matrix & distancias, matrix & feromonas, double alfa, double beta){
	vector<bool>(distancias.size(),0).swap(flags);
	res.clear();
	res.shrink_to_fit();
	vector<double> probs;

	int estadoActual = rand() % distancias.size();
	flags[estadoActual] = true;
	res.push_back(estadoActual);
	float randNumber = 0;
	double sum = 0;
	for(int i = 0; i < flags.size() - 1; i++){
		probs = vector<double>(flags.size(),0);
		sum = 0;
		for(int j = 0; j < distancias.size(); j++){
			if(j == estadoActual) continue;
			if(flags[j] == true) continue;
			probs[j] = pow(distancias[estadoActual][j],alfa) * pow(feromonas[estadoActual][j],beta);
			sum += probs[j];
		}
		for_each(probs.begin(), probs.end(), [sum](double & a){
			a = a / sum;
		});
		randNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		estadoActual = getForProb(probs, randNumber);
		flags[estadoActual] = true;
		res.push_back(estadoActual);
	}
}

#endif