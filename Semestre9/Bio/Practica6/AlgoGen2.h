#ifndef ALGOGEN_H
#define ALGOGEN_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;

#define N 0
#define S 1

#define FITNESS_INFINITO 1000;

class Cromosoma;

typedef float FitVal;
typedef vector<Cromosoma> Poblacion;
typedef vector<string> CromVal;
typedef function<FitVal(Cromosoma&)> AlgoGenFitnessFunc;
typedef tuple<Cromosoma,Cromosoma> ParCromo;
typedef function<Cromosoma(Cromosoma,bool & )> AlgoGenMutFunc;
typedef function<Poblacion(Poblacion, Poblacion)> AlgoGenNextPoblFunc;

int cToI(char c){
	string s = "";
	s.push_back(c);
	return stoi(s);
}

void printEntrada(vector<int> entrada){
	for(int i : entrada){
		switch(i){
			case N: cout<<"N"; break;
			case S: cout<<"S"; break;
		}
	}
}

class Cromosoma{
	public:
		Cromosoma(){}
		Cromosoma(int tamDic, int tamEstados, bool & res);
		Cromosoma(CromVal crom){this->crom = crom;}
		void printCrom(){
			int count = 0;
			cout<<"Inicial: ";
			switch(estadoInicial){
				case 0: cout<<"A "; break;
				case 1: cout<<"B "; break;
				case 2: cout<<"C "; break;
				case 3: cout<<"D "; break;
			}

			for(string s : crom){
				switch(count){
					case 0: cout<<"A: "; break;
					case 1: cout<<"B: "; break;
					case 2: cout<<"C: "; break;
					case 3: cout<<"D: "; break;
				}
				count++;
				for(int i = 0; i < s.size(); i++){
					if(i == 0) cout<<s[i];
					else if(i == s.size() - 1 or i == s.size() - 2){
						switch(cToI(s[i])){
							case 0: cout<<"A"; break;
							case 1: cout<<"B"; break;
							case 2: cout<<"C"; break;
							case 3: cout<<"D"; break;
						}
					}
					else{
						switch(cToI(s[i])){
							case N: cout<<"N"; break;
							case S: cout<<"S"; break;
						}
					}
				}
				cout<<" ";
			}
		}
		int bin_to_int();
		bool isvalid();
		int tamCrom;
		int tamDic;
		int tamEstados;
		int probabilidad;
		FitVal fit;
		CromVal crom;
		vector<int> salida;
		vector<bool> activos;
		int estadoInicial;
};

bool sortFun(Cromosoma a, Cromosoma b){
	return a.fit < b.fit;
}

bool Cromosoma::isvalid(){
	int count = 0;
	int minActiveStates = 2;
	for(string s : crom){
		if(count > 2) return false;
		if(s.front() == '0') count++;
	}
	return true;
}

Cromosoma::Cromosoma(int tamDic, int tamEstados, bool & res){
	this->tamDic = tamDic;
	this->tamEstados = tamEstados;
	crom = CromVal(tamEstados);
	int numRandom = rand() % tamEstados;
	vector<bool> flags(tamDic, false);
	activos = vector<bool>(tamEstados, false);
	int count = 0;
	for(int i = 0; i < tamEstados; i++){
		numRandom = rand() % 2;
		crom[i].push_back(to_string(numRandom).front());
		if(numRandom == 1) activos[i] = true;
		else count++;
	}
	if(count > 2){
		res = false;
		return;
	}
	if(activos[numRandom] == true) estadoInicial = numRandom;
	for(int i = 0; i < activos.size(); i++){
		if(activos[i] == true){
			 estadoInicial = i;
			 break;
		}
		
	}

	for(int i = 0; i < tamEstados; i++){
		for(int j = 0; j < tamDic; j++){
			/*
			numRandom = rand() % tamDic;
			crom[i].push_back(to_string(numRandom).front());
			if(numRandom == 0) crom[i].push_back('1');
			else crom[i].push_back('0');
			break;
			*/

			if(flags[numRandom] == false){
				flags[numRandom] = true;
				crom[i].push_back(to_string(numRandom).front());
			}
			else{
				for(int k = 0; k < flags.size(); k++){
					if(flags[k] == false){
						flags[k] = true;
						crom[i].push_back(to_string(k).front());
						break;
					}
				}
			}
			
		}
		flags = vector<bool>(tamDic, false);
		for(int j = 0; j < tamDic; j++){
			numRandom = rand() % tamDic;
			if(flags[numRandom] == false){
				flags[numRandom] = true;
				crom[i].push_back(to_string(numRandom).front());
			}
			else{
				for(int k = 0; k < flags.size(); k++){
					if(flags[k] == false){
						flags[k] = true;
						crom[i].push_back(to_string(k).front());			
						break;
					}
				}
			}
		}
		flags = vector<bool>(tamEstados, false);
		for(int j = 0; j < tamDic; j++){
			numRandom = rand() % tamEstados;
			if(flags[numRandom] == false and activos[numRandom] == true){
				flags[numRandom] = true;
				crom[i].push_back(to_string(numRandom).front());
			}
			else{
				for(int k = 0; k < flags.size(); k++){
					if(flags[k] == false and activos[k] == true){
						flags[k] = true;
						crom[i].push_back(to_string(k).front());			
						break;
					}
				}	
			}
		}
		cout<<crom[i]<<endl;
	}
	res = true;
	//res = isvalid();
}


class AlgoGen{
	public:
		AlgoGen(int tamP, int tamDic, int tamEstados, int numIter, 
				AlgoGenFitnessFunc fitness, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl);
		void run();
	private:
		void initPoblacion();
		int tamP;
		int tamDic;
		int tamEstados;
		int numIter;
		AlgoGenFitnessFunc fitness;
		AlgoGenMutFunc mut;
		AlgoGenNextPoblFunc next;
		//AlgoGenRestrictionFunc restriction;
		Poblacion pobl;
};

AlgoGen::AlgoGen(int tamP, int tamDic, int tamEstados, int numIter, 
				AlgoGenFitnessFunc fitness, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl){
	this->tamP = tamP;
	this->numIter = numIter;
	this->tamDic = tamDic;
	this->tamEstados = tamEstados;
	this->fitness = fitness;
	this->mut = mut;
	this->next = getNextPobl;
	initPoblacion();
}

void AlgoGen::initPoblacion(){
	cout<<"Generando Poblacion Inicial"<<endl;
	bool flag = false;
	for(int i = 0; i < tamP; i++){
		pobl.push_back(Cromosoma(tamDic, tamEstados, flag));
		if(flag == false){
			pobl.pop_back();
			i--;
		}
	}
	int actual = 1;
	for(Cromosoma c : pobl){
		cout<<actual++<<")\t";
		c.printCrom();
		cout<<endl;
	}
	cout<<endl;
}

void AlgoGen::run(){
	vector<ParCromo> padres;
	Poblacion newPobl;
	int actual = 1;
	for(int i = 0; i < numIter + 1; i++){
		if(i != 0){
			cout<<"Seleccion de la siguiente poblacion"<<endl;
			cout<<"Poblacion de Padres"<<endl;
			for(int j = 0; j < pobl.size(); j++){
				pobl[j].fit = fitness(pobl[j]);
			}
			actual = 1;
			for(Cromosoma c : pobl){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<"\t"<<c.fit<<endl;
			}
			cout<<endl;
			cout<<"Poblacion de Hijos"<<endl;
			for(int j = 0; j < newPobl.size(); j++){
				newPobl[j].fit = fitness(newPobl[j]);
			}
			actual = 1;
			for(Cromosoma c : newPobl){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<"\t"<<c.fit<<endl;
			}
			cout<<endl;
			pobl = next(pobl, newPobl);
		}
		cout<<"Iteracion "<<i<<endl<<endl;
		cout<<"Evaluando Individuos"<<endl;
		for(int j = 0; j < pobl.size(); j++){
			pobl[j].fit = fitness(pobl[j]);
		}
		sort(pobl.begin(), pobl.end(), sortFun);
		actual = 1;
		for(Cromosoma c : pobl){
			cout<<actual++<<")\t";
			c.printCrom();
			cout<<"\t"<<c.fit<<"\t";
			printEntrada(c.salida);
			cout<<endl;
		}
		cout<<endl;
		if(i == numIter) break;
		newPobl.clear();
		bool flag = false;
		for(int i = 0; i < pobl.size(); i++){
			newPobl.push_back(mut(pobl[i], flag));
			if(flag == false){
				newPobl.pop_back();
				i--;
			}
		}
	}
	cout<<"Fin del proceso"<<endl;
}

#endif