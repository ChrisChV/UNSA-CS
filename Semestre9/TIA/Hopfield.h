#ifndef HOPFIELD_H
#define HOPFIELD_H

#include <iostream>
#include <vector>

using namespace std;

typedef PesoType float;
typedef ValType float;
typedef vector<ValType> Patron;


class Hopfield{
	public:
		Hopfield(vector<Patron> patrones);
		void run();
		Patron evaluate(Patron);
		vector<vector<PesoType>> pesos;
		vector<Patron> patrones;

};

Patron producto(Patron patron, vector<vector<PesoType>> pesos){
	Patron res;
	ValType sum = 0;
	for(int i = 0; i < patron.size(); i++){
		sum = 0;
		for(int j = 0; j < patron.size(); j++){
			sum += patron[j] * peso[j][i];
		}
		res.push_back(sum);
	}
	return res;
}

bool operator==(Patron a, Patron b){
	for(int i = 0; i < a.size(); i++){
		if(a[i] != b[i]) return false;
	}
	return true;
}

Hopfield::Hopfield(vector<Patron> patrones){
	this->patrones = patrones;
	pesos = vector<vector<PesoType>>(patrones.size());
	for(int i = 0; i < pesos.size(); i++){
		pesos[i] = vector<PesoType>(patrones.size(), 0);
	}
}

Patron Hopfield::evaluate(Patron p){
	Patron res;
	Patron salida = producto(p, pesos);
	for(int i = 0; i < salida.size(); i++){
		if(salida[i] > 0) res.push_back(1);
		else if(salida[i] < 0) res.push_back(-1);
		else res.push_back(p[i]);
	}
	return res;
}

void Hopfield::run(){
	int count = 0;
	PesoType  sum = 0;
	bool flag = false;
	Patron res;
	while(true){
		cout<<"Iteracion "<<count++<<endl;
		for(int i = 0; i < pesos.size(); i++){
			for(int j = 0;  j < pesos[i].size(); j++){
				if(i == j) continue;
				sum = 0;
				for(int k = 0; k < patrones.size(); k++){
					sum += patrones[k][i] * patrones[k][j];
				}
				pesos[i][j] = sum;
			}
		}
		flag = true;
		for(int i = 0; i < patrones.size(); i++){
			res = evaluate(patrones[i]);
			if(!(patrones[i] == res)){
				flag = false;
				break;
			}
		}
		if(flag == true) break;
	}
}

#endif