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
	double x1 = crom.crom[0];
	double x2 = crom.crom[1];
	return (FitVal) 100 * pow(pow(x1,2) - x2,2) + pow(1 - x1,2);
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
	double delta = 0.0001;
	for(int i = 0; i < res.crom.size(); i++){
		numRand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		res.crom[i] = res.crom[i] + valor_x(-2.048, 2.048, crom.desviacionEstandar[i], delta, numRand);
		if(isnan(res.crom[i])){
			res.crom[i] = crom.crom[i];
			cout<<"B"<<endl;
			cout<<i<<endl;
			i--;
			int te;
			cin>>te;
			continue;
		}
		if(res.crom[i] < -2.048 or res.crom[i] > 2.048){
			res.crom[i] = crom.crom[i];
			i--;
		} 
	}
	return res;
}


Poblacion nextFun(Poblacion padres, Poblacion hijos, bool & flag){
	Poblacion res;
	if(padres.front().fit > hijos.front().fit){
		flag = false;
		res = padres;
		return res;
	}
	else{
		flag = true;
		res = hijos;
		return res;
	}
}


int main(){
	srand(time(NULL));
	int tamP = 1;
	int tamCrom = 2;
	int numIter = 1000;

	AlgoGen ag(tamP, tamCrom, numIter, fitFun, funMut, nextFun);
	ag.run();
}