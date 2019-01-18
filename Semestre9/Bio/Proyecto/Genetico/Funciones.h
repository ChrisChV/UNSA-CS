#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <vector>
#include "AlgoGen.h"
#include "Terreno.h"

using namespace std;

#define CROM_COMPLETO 0
#define CROM_DEFECTUOSO 1
#define CROM_NORMAL 2

vector<Terreno> zona;
int disponibilidadAgua = 100; //default;

int verifyCrom(string newCrom, int newPos){
	newCrom[newPos] = '1';
	int sum = 0;
	int count = 0;
	for(int i = 0; i < newCrom.size(); i++){
		if(newCrom[i] == '0') continue;
		count++;
		sum += zona[i].cultivo.agua * zona[i].area;
	}
	if(sum > disponibilidadAgua)  return CROM_DEFECTUOSO;
	if(count == zona.size()) return CROM_COMPLETO;
	return CROM_NORMAL;
}

Poblacion initPoblacion(int tamP){
	string newCrom = "";
	int numRandom = 0;
	int state = 0;
	Poblacion res;
	for(int i = 0; i < tamP; i++){
		newCrom = string(zona.size(), '0');
		while(true){
			numRandom = rand() % zona.size();
			if(newCrom[numRandom] == '1'){
				for(int j = 0; j < zona.size(); j++){
					if(newCrom[j] == '0'){
						numRandom = j;
						break;
					}
				}
			}
			state = verifyCrom(newCrom, numRandom);
			if(state == CROM_COMPLETO){
				newCrom[numRandom] = '1';
				break;
			}
			if(state == CROM_DEFECTUOSO){
				break;
			}
			newCrom[numRandom] = '1';
		}

		res.push_back(Cromosoma(newCrom));
	}
	return res;
}

FitVal funcFitness(Cromosoma crom){
	FitVal res = 0;
	for(int i = 0; i < crom.crom.size(); i++){
		if(crom.crom[i] == '0') continue;
		res += zona[i].getFitness();
	}
	return res;
}

vector<ParCromo> funcRouleteSelect(Poblacion pobl, int n){
	vector<ParCromo> res;
	vector<int> probabilidades;
	int suma = 0;
	for(int i = 0; i < pobl.size(); i++){
		suma += pobl[i].fit;
	}
	for(int i = 0; i < pobl.size(); i++){
		pobl[i].probabilidad = (pobl[i].fit * 100) / suma;
		if(probabilidades.size() == 0){
			probabilidades.push_back(pobl[i].probabilidad);
		}
		else if(i != pobl.size() - 1){
			probabilidades.push_back(probabilidades.back() + pobl[i].probabilidad);
		}
		else probabilidades.push_back(100);
	}
	int actual = 1;
	cout<<endl;
	Cromosoma padres[2];
	int numRandom = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < 2; j++){
			numRandom = rand() % 101;
			for(int k = 0; k < probabilidades.size(); k++){
				if(probabilidades[k] >= numRandom){
					padres[j] = pobl[k];
					break;
				}
			}
		}
		res.push_back(make_tuple(padres[0], padres[1]));
	}
	return res;
}

bool funcLimit(Cromosoma crom){
	int sum = 0;
	for(int i = 0; i < crom.crom.size(); i++){
		if(crom.crom[i] == '0') continue;
		sum += zona[i].cultivo.agua * zona[i].area;
	}
	if(sum <= disponibilidadAgua) return true;
	return false;
}

int funcCrossTwoPoints(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
	int faltantes = 0;
	int realProbCruz = probCruz * 100;
	Cromosoma padre;
	Cromosoma madre;
	int point1 = 0;
	int point2 = 0;
	int numRandom = 0;
	string newCromA = "";
	string newCromB = "";
	for(ParCromo par : padres){
		tie(padre, madre) = par;
		numRandom = rand() % 101;
		if(numRandom > realProbCruz){
			faltantes++;
			continue;	
		} 
		point1 = rand() % padre.crom.size();
		if(point1 == 0) point2 = rand() % (padre.crom.size() - 1) + 1;
		else point2 = rand() % (padre.crom.size() - point1) + (point1 + 1);
		
		newCromA = padre.crom;
		newCromB = madre.crom;
		for(int i = point1; i < point2; i++){
			swap(newCromA[i], newCromB[i]);
		}
		resPobl.push_back(Cromosoma(newCromA));
		resPobl.push_back(Cromosoma(newCromB));
	}
	return faltantes;
}


void mutFunc(Poblacion & pobl, float probMut){
	int realProbMut = probMut * 100;
	int numRandom = 0;
	int posRandom = 0;
	for(int i = 0; i < pobl.size(); i++){
		numRandom = rand() % 101;
		if(numRandom > realProbMut) continue;
		posRandom = rand() % pobl.front().crom.size();
		if(pobl[i].crom[posRandom] == '0') pobl[i].crom[posRandom] = '1';
		else pobl[i].crom[posRandom] = '0';
	}
}

Poblacion nextGen(Poblacion pobl){
	Poblacion res;
	sort(pobl.begin(), pobl.end(), [](Cromosoma a, Cromosoma b){
		return a.fit > b.fit;
	});
	res.insert(res.begin(), pobl.begin(), pobl.begin() + (pobl.size() / 2));
	return res;
}

#endif