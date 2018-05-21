#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <cmath>
#include <algorithm>
#include "AlgoGen1.h"

using namespace std;

typedef tuple<int,int> Transicion;

double desviacionDeltas;

FitVal fitFun(Cromosoma  crom){
	double x1 = crom.crom[0];
	double x2 = crom.crom[1];
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

Cromosoma getMenor(vector<Cromosoma> contrincantes){
	Cromosoma res;
	bool flag = false;
	for(Cromosoma c : contrincantes){
		if(flag == false){
			res = c;
			flag = true;	
		} 
		else if(res.fit > c.fit) res = c;
	}
	return res;
}

vector<ParCromo> funcTournamentSelect(Poblacion pobl, int n){
	cout<<"Seleccion de padres metodo torneo"<<endl;
	int tamTorneo = 3;
	cout<<"Tam de torneo: "<<tamTorneo<<endl;
	vector<ParCromo> res;
	vector<Cromosoma> contrincantes;
	Cromosoma padres[2];
	int numRandom = 0;
	for(int i = 0; i < n; i++){
		cout<<"Torneo "<<i<<endl;
		for(int j = 0; j < 2; j++){
			contrincantes.clear();
			for(int k = 0; k < tamTorneo; k++){
				numRandom = rand() % pobl.size();
				contrincantes.push_back(pobl[numRandom]);
				cout<<k + 1<<")\t";
				contrincantes.back().printCrom();
				cout<<endl;
			}
			padres[j] = getMenor(contrincantes);
			cout<<"Ganador:\t";
			padres[j].printCrom();
			cout<<endl<<endl;
			cout<<endl;
		}
		res.push_back(make_tuple(padres[0], padres[1]));
	}
	return res;
}

void funcCross(ParCromo padres, Poblacion & resPobl){
	int tam = get<0>(padres).crom.size();
	Cromosoma res(tam);
	for(int i = 0; i < tam; i++){
		res.crom[i] = (get<0>(padres).crom[i] + get<1>(padres).crom[i]) / 2;
		res.deltas[i] = sqrt(get<0>(padres).crom[i] + get<1>(padres).crom[i]);
	}
	cout<<endl;
	cout<<"Cruzamiento"<<endl;
	resPobl.push_back(res);
	resPobl.back().printCrom();
	cout<<endl<<endl;
}

void mutFunc(Cromosoma & crom){
	float numRand = 0;
	double delta = 0.1;

	for(int i = 0; i < crom.crom.size(); i++){
		numRand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		crom.deltas[i] = crom.deltas[i] * exp(valor_x(-10,10,desviacionDeltas, delta,numRand));
		numRand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		crom.crom[i] = crom.crom[i] + valor_x(-10,10, crom.deltas[i], delta, numRand);
		if(crom.crom[i] < -10 or crom.crom[i] > 10){
			crom.crom[i] = crom.crom[i];
			//i--;
		} 
	}
}


Poblacion nextGen(Poblacion pobl){
	sort(pobl.begin(), pobl.end(), [] (Cromosoma a, Cromosoma b){
		return a.fit < b.fit;
	});
	pobl.pop_back();
	return pobl;
}

int main(){
	srand(time(NULL));
	int tamP = 20;
	int tamCrom = 2;
	int numIter = 5000;

	desviacionDeltas = 1 / sqrt(2 * sqrt(tamP));

	AlgoGen ag(tamP, tamCrom, numIter, fitFun, funcTournamentSelect, funcCross , mutFunc, nextGen);
	ag.run();
}