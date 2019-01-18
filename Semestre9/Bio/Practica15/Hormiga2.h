#ifndef HORMIGA_H
#define HORMIGA_H


#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

typedef vector<vector<double>> matrix;

enum VALS{A, B, C, D, E, F, G, H, I, J, K, L, M};

class Hormiga{
	public:
		Hormiga(){
			costo = 0;
		};
		void getCamino(matrix & costos, matrix & distancias, matrix & feromonas, double alfa, double delta, double q0, double ferInit, double phi, double Q);
		void setCosto(matrix & distancias, double Q);
		void busquedaLocal(matrix & costos, double Q, int n);
		void print();
		vector<bool> flags;
		vector<int> res;
		double costo;
		double ferCosto;
};

void Hormiga::busquedaLocal(matrix & costos, double Q, int n){
	cout<<"ANTERIOR"<<endl;
	print();
	Hormiga * mejor = this;
	Hormiga * actual = new Hormiga();
	bool flag = false;
	int randNumber1 = 0;
	int randNumber2 = 0;
	while(true){
		flag = false;
		for(int i = 0; i < n; i++){
			*actual = *mejor;
			randNumber1 = rand() % costos.size();
			randNumber2 = rand() % costos.size();
			swap(actual->res[randNumber1], actual->res[randNumber2]);
			actual->setCosto(costos, Q);
			if(actual->costo < mejor->costo){
				*mejor = *actual;
				flag = true;
				break;
			}
		}
		if(flag == false) break;
	}
	*this = *mejor;
	cout<<"ACTUAL"<<endl;
	print();
}

void Hormiga::print(){
	for(auto iter = res.begin(); iter != res.end(); ++iter){
		switch(*iter){
			case A: cout<<"A "; break;
			case B: cout<<"B "; break;
			case C: cout<<"C "; break;
			case D: cout<<"D "; break;
			case E: cout<<"E "; break;
			case F: cout<<"F "; break;
			case G: cout<<"G "; break;
			case H: cout<<"H "; break;
			case I: cout<<"I "; break;
			case J: cout<<"J "; break;
		}
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

void Hormiga::setCosto(matrix & costos, double Q){
	int estadoAns = -1;
	costo = 0;
	for(auto iter = res.begin(); iter != res.end(); ++iter){
		if(estadoAns == -1){
			estadoAns = (*iter);
			continue;
		}
		costo += costos[estadoAns][(*iter)];
		estadoAns = (*iter);
	}
	ferCosto = Q / costo;
}

int getMayor(vector<double> & probs){
	double max = -1;
	double indexMax = -1;
	for(int i = 0; i < probs.size(); i++){
		if(max == -1 or probs[i] > max){
			max = probs[i];
			indexMax = i;	
		} 
	}
	return indexMax;
}

void Hormiga::getCamino(matrix & costos, matrix & distancias, matrix & feromonas, double alfa, double beta, double q0, 
					double ferInit, double phi, double Q){
	vector<bool>(distancias.size(),0).swap(flags);
	res.clear();
	res.shrink_to_fit();
	vector<double> probs;
	int estadoAns= -1;
	int estadoActual = rand() % distancias.size();
	flags[estadoActual] = true;
	res.push_back(estadoActual);
	float randNumber = 0;
	double sum = 0;
	for(int i = 0; i< distancias.size(); i++){
		for(int j = 0; j < distancias.size(); j++){
			cout<<distancias[i][j]<<" ";
		}
		cout<<endl;
	}

	for(int i = 0; i < flags.size() - 1; i++){
		probs = vector<double>(flags.size(),0);
		sum = 0;
		for(int j = 0; j < distancias.size(); j++){
			if(j == estadoActual) continue;
			if(flags[j] == true) continue;
			cout<<estadoActual<<" - "<<j;
			probs[j] = pow(distancias[estadoActual][j],alfa) * pow(feromonas[estadoActual][j],beta);
			sum += probs[j];
			cout<<" t="<<feromonas[estadoActual][j]<<" n="<<distancias[estadoActual][j]<<" t*n="<<probs[j]<<endl;
		}
		randNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if(randNumber > q0){
			cout<<"PROBABILIDAD"<<endl;
			cout<<"SUM="<<sum<<endl;
			for_each(probs.begin(), probs.end(), [sum](double & a){
				a = a / sum;
				if(a != 0) cout<<a<<endl;
			});
			randNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			cout<<"randNumber="<<randNumber<<endl;
			estadoAns = estadoActual;
			estadoActual = getForProb(probs, randNumber);
			cout<<"Estado elejido "<<estadoActual<<endl;
			flags[estadoActual] = true;
			res.push_back(estadoActual);
		}
		else{
			cout<<"MAYOR"<<endl;
			estadoAns = estadoActual;
			estadoActual = getMayor(probs);
			cout<<"Estado elejido "<<estadoActual<<endl;
			flags[estadoActual] = true;
			res.push_back(estadoActual);
		}
	}
	setCosto(costos,Q);
	busquedaLocal(costos, Q, 10);
	estadoAns = -1;
	for(auto iter = res.begin(); iter != res.end(); ++iter){
		if(estadoAns == -1){
			estadoAns = (*iter);
			continue;
		}
		feromonas[estadoAns][(*iter)] = (1 - phi) * feromonas[estadoAns][(*iter)] + phi * ferInit;
		feromonas[(*iter)][estadoAns] = (1 - phi) * feromonas[(*iter)][estadoAns] + phi * ferInit;
		estadoAns = (*iter);
	}
}

#endif