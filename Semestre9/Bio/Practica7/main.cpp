#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <cmath>
#include <algorithm>
#include "AlgoGen1.h"

using namespace std;

typedef tuple<int,int> Transicion;

FitVal fitFun(Cromosoma & crom){
	float x1 = crom.crom[0];
	float x2 = crom.crom[1];
	return (FitVal) -cos(x1) * cos(x2) * exp(-pow(x1 - M_PI,2) - pow(x2 - M_PI, 2));
}

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

Cromosoma funMut(Cromosoma crom){
	Cromosoma res = crom;
	double alfa = 2.0;
	float numRand = 0;
	double desvio = 0.3;
	double delta = 0.1;
	for(int i = 0; i < res.deltas.size(); i++){
		numRand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		res.deltas[i] = res.deltas[i] * (1 + alfa * valor_x(-10, 10, desvio, delta, numRand));
		if(isnan(res.deltas[i])){
			res.deltas[i] = crom.deltas[i];
			cout<<"A"<<endl;
			cout<<i<<endl;
			i--;
			int te;
			cin>>te;
			continue;
		}

		numRand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		res.crom[i] = res.crom[i] + res.deltas[i] * valor_x(-10, 10, desvio, delta, numRand);
		//cout<<valor_x(-5, 5, desvio, delta, numRand)<<endl;
		if(isnan(res.crom[i])){
			res.crom[i] = crom.crom[i];
			cout<<"B"<<endl;
			cout<<i<<endl;
			i--;
			int te;
			cin>>te;
			continue;
		}
		if(res.crom[i] < -10 or res.crom[i] > 10){
			res.crom[i] = crom.crom[i];
			i--;
		} 

	}
	return res;
}


Poblacion nextFun(Poblacion padres, Poblacion hijos){
	Poblacion res;
	sort(padres.begin(), padres.end(), sortFun);
	sort(hijos.begin(), hijos.end(), sortFun);
	res.insert(res.begin(), padres.begin(), padres.begin() + padres.size() / 2);
	res.insert(res.begin(), hijos.begin(), hijos.begin() + hijos.size() / 2);
	sort(res.begin(), res.end(), sortFun);
	return res;
}


int main(){
	srand(time(NULL));
	int tamP = 100;
	int tamCrom = 2;
	int numIter = 100;

	AlgoGen ag(tamP, tamCrom, numIter, fitFun, funMut, nextFun);
	ag.run();
}