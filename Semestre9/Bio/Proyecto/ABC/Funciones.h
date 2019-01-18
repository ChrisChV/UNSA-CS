#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
#include "ABC.h"
#include "Terreno.h"

using namespace std;

#define CROM_COMPLETO 0
#define CROM_DEFECTUOSO 1
#define CROM_NORMAL 2

vector<Terreno> zona;
int disponibilidadAgua = 100; //default;
int tamOfLastVal = 16; //default


string getBin(vector<TypeVal> valores){
	string partBin = "";
	string totalBin = "";
	string temp = "";
	for(int i = 0; i < valores.size(); i++){
		partBin = bitset<16>(valores[i]).to_string();
		if(i == valores.size() - 1){
			temp.clear();
			for(int j = 0; j < tamOfLastVal; j++){
				temp.push_back(partBin[partBin.size() - 1 - j]);
			}
			for(int j = 0; j < temp.size(); j++){
				totalBin.push_back(temp[temp.size() - 1 -j]);
			}
		}
		else totalBin += partBin;
	}
	return totalBin;
}

TypeDVal funFit(vector<TypeVal> valores){
	string totalBin = getBin(valores);
	TypeDVal res = 0;
	for(int i = 0; i < totalBin.size(); i++){
		if(totalBin[i] == '0') continue;
		res += zona[i].getFitness();
	}
	return res;
}


void setTerrenos(string fileName, vector<Terreno> & terrenos, int & dispAgua){
	ifstream inFile(fileName);
	int numTerrenos = 0;
	int area = 0;
	double distancia = 0;
	double rendimiento = 0;
	int agua = 0;
	double costo = 0;
	double beneficio = 0;

	inFile>>numTerrenos;
	inFile>>dispAgua;

	for(int i = 0; i < numTerrenos; i++){
		inFile>>area;
		inFile>>distancia;
		inFile>>rendimiento;
		inFile>>agua;
		inFile>>costo;
		inFile>>beneficio;
		terrenos.push_back(Terreno(area,rendimiento, distancia, costo, beneficio, agua));
	}
}

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

Fuente generarFuente(){
	string newCrom = "";
	int numRandom = 0;
	int state = 0;
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
	int diviciones = newCrom.size() / 16;
	int resto = newCrom.size() % 16;
	string partBin = "";
	Fuente res;
	int actual = 0;
	for(int i = 0; i < diviciones; i++){
		partBin.clear();
		for(int j = 0; j < 16; j++){
			partBin.push_back(newCrom[actual]);
			actual++;
		}
		res.valores.push_back(bitset<16>(partBin).to_ulong());
	}
	if(resto != 0){
		partBin.clear();
		partBin = string(16 - resto, '0');
		for(int i = 0; i < resto; i++){
			partBin.push_back(newCrom[actual]);
			actual++;
		}
		TypeVal test = bitset<16>(partBin).to_ulong();
		res.valores.push_back(test);	
		

	}
	res.fitFun = funFit;
	return res;
}

bool funcLimit(Fuente fuente){
	string totalBin = getBin(fuente.valores);
	int sum = 0;
	for(int i = 0; i < totalBin.size(); i++){
		if(totalBin[i] == '0') continue;
		sum += zona[i].cultivo.agua * zona[i].area;
	}
	if(sum <= disponibilidadAgua) return true;
	return false;
}

#endif