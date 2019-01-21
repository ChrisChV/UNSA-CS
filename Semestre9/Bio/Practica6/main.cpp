#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <algorithm>
#include "AlgoGen2.h"

using namespace std;

typedef tuple<int,int> Transicion;


FitVal fitFun(Cromosoma & crom){
	vector<int> entrada = {N, S, S, N, S, S, N, S, S, N, S, S, N, S, S};
	vector<int> salida;
	vector<bool> activos(crom.tamEstados,false);
	vector<vector<Transicion>> mapa;
	int conEntrada = 0;
	int escribir = 0;
	int pasarA = 0;
	for(int i = 0; i < crom.tamEstados; i++){
		mapa.push_back(vector<Transicion>(crom.tamDic));
		if(crom.crom[i][0] == '1')  activos[i] == true;
		for(int j = 0; j < crom.tamDic; j++){
			conEntrada = cToI(crom.crom[i][1 + j]);
			escribir = cToI(crom.crom[i][1 + crom.tamDic + j]);
			pasarA = cToI(crom.crom[i][1 + crom.tamDic * 2 + j]);
			mapa[i][conEntrada] = make_tuple(escribir, pasarA);
		}
	}
	int estadoActual = crom.estadoInicial;
	FitVal sum = 0;
	for(int i = 0; i < entrada.size(); i++){
		if(i != 0){
			if(entrada[i] != salida[i - 1]) sum++;
		}
		Transicion tran = mapa[estadoActual][entrada[i]];
		salida.push_back(get<0>(tran));
		estadoActual = get<1>(tran);
		//if(activos[estadoActual] == false) return FITNESS_INFINITO;
	}
	crom.salida = salida;

	return sum;
}

Cromosoma funMut(Cromosoma crom, bool & flag){
	int numRandom = rand() % 101;
	if(numRandom <= 20){
		numRandom = rand() % crom.tamEstados;
		crom.crom[numRandom][0] = '0';
		crom.activos[numRandom] = false;
		flag = crom.isvalid();
		return crom;
	}
	else if(numRandom <= 40){
		numRandom = rand() % crom.tamEstados;
		if(crom.activos[numRandom] == true) crom.estadoInicial = numRandom;
		else{
			for(int i = 0; i < crom.activos.size(); i++){
				if(crom.activos[i] == true){
					crom.estadoInicial = i;	
					break;
				}
			}	
		}
		flag = true;
		return crom;
	}
	else if(numRandom <= 60){
		int randState = rand() % crom.tamEstados;
		swap(crom.crom[randState][1], crom.crom[randState][2]);
		flag = true;
		return crom;
	}
	else if(numRandom <= 80){
		int randState = rand() % crom.tamEstados;
		numRandom = rand() % crom.tamDic;
		int numRandom2 = rand() % crom.tamDic;
		crom.crom[randState][1 + crom.tamDic + numRandom] = to_string(numRandom2).front();
		flag = true;
		return crom;
	}
	else if(numRandom <= 100){
		numRandom = rand() % crom.tamEstados;
		crom.crom[numRandom][0] = '1';
		crom.activos[numRandom] = true;
		flag = true;
		return crom;
	}
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
	int tamP = 50;
	int tamDic = 2;
	int tamEstados = 4;
	int numIter = 100;

	AlgoGen ag(tamP, tamDic, tamEstados, numIter, fitFun, funMut, nextFun);
	ag.run();
}






