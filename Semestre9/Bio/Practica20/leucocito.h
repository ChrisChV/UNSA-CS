#ifndef LEUCOCITO_H
#define LEUCOCITO_H

#include <iostream>
#include <bitset>
#include <vector>
#include <functional>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

#define NUM_OF_BITS 16

typedef double TypeVal;
typedef function<TypeVal(vector<TypeVal>)> FunFit;

class Leucocito{
	public:
		Leucocito(){}
		Leucocito(TypeVal mutationFactor, int cantidadClones, FunFit funFit);
		Leucocito(Leucocito *);
		vector<bitset<NUM_OF_BITS>> valores;
		vector<Leucocito> clones;
		void setFitness();
		void setAfinidad(TypeVal maxFit, TypeVal minFit);
		void setClones();
		void mutacion();
		void print();
		void printClones();
		TypeVal fitness;
		TypeVal afinidad;
		TypeVal mutationFactor;
		int cantidadClones;
		TypeVal mutationRate;
		FunFit funFit;
};

Leucocito::Leucocito(Leucocito * nuevo){
	this->valores = nuevo->valores;
	this->fitness = nuevo->fitness;
	this->afinidad = nuevo->afinidad;
	this->mutationFactor = nuevo->mutationFactor;
	this->cantidadClones = nuevo->cantidadClones;
	this->mutationRate = nuevo->mutationRate;
	this->funFit = nuevo->funFit;
}

Leucocito::Leucocito(TypeVal mutationFactor, int cantidadClones, FunFit funFit){
	this->mutationFactor = mutationFactor;
	this->cantidadClones = cantidadClones;
	this->funFit = funFit;
	string binary = "";
	int numRandom = 0;
	for(int i = 0; i < 2; i++){
		binary.clear();

		for(int j = 0; j < NUM_OF_BITS; j++){
			numRandom = rand() % 2;
			if(numRandom == 1) binary.push_back('1');
			else binary.push_back('0');
		}
		valores.push_back(bitset<NUM_OF_BITS>(binary));
	}

}

void Leucocito::print(){
	cout<<"x=[";
	TypeVal temp = 0;
	for(int i = 0; i < valores.size(); i++){
		temp = -5 + (TypeVal) ((10) / (TypeVal) (pow(2,NUM_OF_BITS)) * valores[i].to_ulong());
		cout<<" "<<temp<<",";
	}
	cout<<"], f = "<<fitness<<", a = "<<afinidad<<endl;
}

void Leucocito::printClones(){
	for(auto clon : clones){
		cout<<clon.valores[0].to_string()<<clon.valores[1].to_string()<<endl;
	}
}

void Leucocito::setFitness(){
	vector<TypeVal> vals;
	for(auto bs : valores){
		vals.push_back(bs.to_ulong());
	}
	for(int i = 0; i < vals.size(); i++){
		vals[i] = -5 + (TypeVal) ((10) / (TypeVal) (pow(2,NUM_OF_BITS)) * vals[i]);
	}
	fitness = funFit(vals);
}

void Leucocito::setAfinidad(TypeVal maxFit, TypeVal minFit){
	if(maxFit == minFit) afinidad  = 0;
	else afinidad = 1.0 - (fitness / (maxFit - minFit));
}

void Leucocito::setClones(){
	mutationRate = exp(mutationFactor * afinidad);
	clones.clear();
	for(int i = 0; i < cantidadClones; i++){
		clones.push_back(Leucocito(this));
		clones.back().mutacion();
		clones.back().setFitness();
	}
}

void Leucocito::mutacion(){
	float random = 0;
	string binary = "";
	for(int i = 0; i < valores.size(); i++){
		binary = valores[i].to_string();
		for(int j = 0; j < binary.size(); j++){
			random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if(random >= mutationRate){
				if(binary[j] == '1') binary[j] = '0';
				else binary[j] = '1';
			}
		}
		valores[i] = bitset<NUM_OF_BITS>(binary);
	}
}

#endif