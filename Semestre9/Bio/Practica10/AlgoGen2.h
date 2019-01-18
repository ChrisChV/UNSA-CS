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

class Cromosoma;

typedef float FitVal;
typedef vector<Cromosoma> Poblacion;
typedef vector<int> CromVal;
typedef function<FitVal(Cromosoma)> AlgoGenFitnessFunc;
typedef tuple<Cromosoma,Cromosoma> ParCromo;
typedef function<vector<ParCromo>(Poblacion, int)> AlgoGenSelectFunc;
typedef function<int(vector<ParCromo>, float, Poblacion &)> AlgoGenCrossFunc; //hacer push
typedef function<void(Poblacion &, float)> AlgoGenMutFunc;
typedef function<Poblacion(Poblacion, int)> AlgoGenNextPoblFunc;
typedef function<bool(Cromosoma)> AlgoGenRestrictionFunc;

class Cromosoma{
	public:
		Cromosoma(){}
		Cromosoma(int tamCrom);
		Cromosoma(CromVal crom){
			this->crom = crom;
			frontera = -1;
			cDistance = -1;
			fit = vector<FitVal>(2);
		}
		void printCrom(){
			for(int i : crom){
				//cout<<"i->"<<i<<endl;
				switch(i){
					case 0: cout<<"A "; break;
					case 1: cout<<"B "; break;
					case 2: cout<<"C "; break;
					case 3: cout<<"D "; break;
					case 4: cout<<"E "; break;
					case 5: cout<<"F "; break;
					case 6: cout<<"G "; break;
					case 7: cout<<"H "; break;
					case 8: cout<<"I "; break;
					case 9: cout<<"J "; break;
				}
			}
			cout<<"\t";
			for(float f : fit){
				cout<<f<<" ";
			}
			cout<<"\t"<<frontera<<" "<<cDistance;
		}
		int bin_to_int();
		int tamCrom;
		int probabilidad;
		int frontera;
		float cDistance;
		CromVal crom;
		vector<FitVal> fit;
};

Cromosoma::Cromosoma(int tamCrom){
	vector<bool> flag(tamCrom, 0);
	this->tamCrom = tamCrom;
	int numRandom = 0;
	int iter = 0;
	fit = vector<FitVal>(2);
	frontera = -1;
	cDistance = -1;
	for(int i = 0; i < tamCrom; i++){
		numRandom = rand() % tamCrom;
		if(flag[numRandom] == false){
			crom.push_back(numRandom);
			flag[numRandom] = true;
		} 
		else{
			iter = 0;
			for(int i = 0; i < flag.size(); i++){
				if(flag[i] == false){
					crom.push_back(iter);
					flag[i] = true;
					break;	
				} 
				iter++;
			}
		}
	}
}

int Cromosoma::bin_to_int(){
	int res = 0;
	int actual = 0;
	char c = 0;
	for(int i = crom.size() - 1; i >= 0; i--){
		c = crom[i];
		if(c == '1') res += pow(2, actual);
		actual++;
	}
	return res;
}

class AlgoGen{
	public:
		AlgoGen(int tamP, int tamCrom, int numIter, float probCruz, float probMut,
				vector<AlgoGenFitnessFunc>, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl);
		void run();
	private:
		void initPoblacion();
		int tamP;
		int tamCrom;
		int numIter;
		float probCruz;
		int puntoCruz;
		float probMut;
		vector<AlgoGenFitnessFunc> funciones;
		AlgoGenSelectFunc select;
		AlgoGenCrossFunc cross;
		AlgoGenMutFunc mut;
		AlgoGenNextPoblFunc next;
		//AlgoGenRestrictionFunc restriction;
		Poblacion pobl;
};

AlgoGen::AlgoGen(int tamP, int tamCrom, int numIter, float probCruz, float probMut,
				vector<AlgoGenFitnessFunc> funciones, AlgoGenSelectFunc select, AlgoGenCrossFunc cross, AlgoGenMutFunc mut,
				AlgoGenNextPoblFunc getNextPobl){
	this->tamP = tamP;
	this->tamCrom = tamCrom;
	this->numIter = numIter;
	this->probCruz = probCruz;
	this->probMut = probMut;
	this->funciones = funciones;
	this->select = select;
	this->cross = cross;
	this->mut = mut;
	this->next = getNextPobl;
	initPoblacion();
}

void AlgoGen::initPoblacion(){
	cout<<"Generando Poblacion Inicial"<<endl;
	for(int i = 0; i < tamP; i++){
		pobl.push_back(Cromosoma(tamCrom));
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
			for(int j = 0; j < pobl.size(); j++){
				for(int k = 0; k < 2; k++){
					pobl[j].fit[k] = funciones[k](pobl[j]);
				}
			}
			actual = 1;
			for(Cromosoma c : pobl){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<endl;
			}
			pobl = next(pobl, tamP);
		}
		cout<<"Iteracion "<<i<<endl<<endl;
		cout<<"Evaluando Individuos"<<endl;
		for(int j = 0; j < pobl.size(); j++){
			for(int k = 0; k < 2; k++){
				pobl[j].fit[k] = funciones[k](pobl[j]);
			}
		}
		actual = 1;
		for(Cromosoma c : pobl){
			cout<<actual++<<")\t";
			c.printCrom();
			cout<<endl;
		}
		cout<<endl;
		if(i == numIter) break;
		padres = select(pobl, tamP);
		newPobl.clear();
		int faltantes = cross(padres, probCruz, newPobl);
		while(faltantes != 0){
			cout<<"Faltan elegir "<<faltantes<<" padres"<<endl;
			padres = select(pobl, faltantes);
			faltantes = cross(padres, probCruz, newPobl);
		}
		mut(newPobl, probMut);
		pobl.insert(pobl.begin(), newPobl.begin(), newPobl.end());
	}
	cout<<"Fin del proceso"<<endl;
}

#endif