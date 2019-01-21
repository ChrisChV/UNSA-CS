#ifndef HOPFIELD_H
#define HOPFIELD_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef float PesoType;
typedef float ValType;
typedef vector<ValType> Patron;

class Hopfield{
	public:
		Hopfield(string file, int tamPatron, int numPatrones);
		Hopfield(vector<Patron> patrones);
		void run();
		void printPatron(int index);
		void printPatron(Patron p);
		Patron evaluate(Patron);
		vector<Patron> evaluate(string file, int tamPatron, int numPatrones);
		vector<vector<PesoType>> pesos;
		vector<Patron> patrones;

};

void Hopfield::printPatron(int index){
	printPatron(patrones[index]);
}

void Hopfield::printPatron(Patron p){
	int colums = 28;
	int rows = 28;
	int count = 0;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < colums; j++){
			if(p[count] == 1) cout<<"@ ";
			else if(p[count] == -1) cout<<". ";
			count++;
		}
		cout<<endl;
	}
	cout<<endl;
}

Patron producto(Patron patron, vector<vector<PesoType>> & pesos){
	Patron res;
	ValType sum = 0;
	for(int i = 0; i < patron.size(); i++){
		sum = 0;
		for(int j = 0; j < patron.size(); j++){
			sum += patron[j] * pesos[j][i];
		}
		res.push_back(sum);
	}
	return res;
}

bool operator==(Patron a, Patron b){
	for(int i = 0; i < a.size(); i++){
		if(a[i] != b[i]) return false;
	}
	return true;
}

Hopfield::Hopfield(string file, int tamPatron, int numPatrones){
	ifstream inFile(file.c_str());
	string word = "";
	int count = 0;
	int count2 = 0;
	while(inFile>>word){
		if(count == 0) patrones.push_back(Patron());
		if(word == "1") patrones.back().push_back(1);
		else if(word == "-1") patrones.back().push_back(-1);
		else cout<<"ERROR"<<endl;
		count++;
		if(count == tamPatron){
			count = 0;
			count2++;
			if(count2 == numPatrones) break;
		}
	}
	pesos = vector<vector<PesoType>>(patrones[0].size());
	for(int i = 0; i < pesos.size(); i++){
		pesos[i] = vector<PesoType>(patrones[0].size(), 0);
	}
}

Hopfield::Hopfield(vector<Patron> patrones){
	this->patrones = patrones;
	pesos = vector<vector<PesoType>>(patrones[0].size());
	for(int i = 0; i < pesos.size(); i++){
		pesos[i] = vector<PesoType>(patrones[0].size(), 0);
	}
}

Patron Hopfield::evaluate(Patron p){
	Patron res;
	Patron salida = producto(p, pesos);
	for(int i = 0; i < salida.size(); i++){
		if(salida[i] > 0) res.push_back(1);
		else if(salida[i] < 0) res.push_back(-1);
		else res.push_back(p[i]);
	}
	return res;
}

vector<Patron> Hopfield::evaluate(string file, int tamPatron, int numPatrones){
	ifstream inFile(file.c_str());
	string word = "";
	int count = 0;
	int count2 = 0;
	vector<Patron> res;
	Patron p;
	while(inFile>>word){
		if(count == 0){
			p.clear();
			p.shrink_to_fit();
		}
		if(word == "1") p.push_back(1);
		else if(word == "-1") p.push_back(-1);
		else cout<<"ERROR"<<endl;
		count++;
		if(count == tamPatron){
			count = 0;
			count2++;
			res.push_back(evaluate(p));
			if(count2 == numPatrones) break;
		}
	}
	return res;
}

void Hopfield::run(){
	int count = 0;
	int limit = 100;
	PesoType  sum = 0;
	bool flag = false;
	Patron res;
	while(true){
		cout<<"Iteracion "<<count++<<endl;
		for(int i = 0; i < pesos.size(); i++){
			for(int j = 0;  j < pesos[i].size(); j++){
				if(i == j) continue;
				sum = 0;
				for(int k = 0; k < patrones.size(); k++){
					sum += patrones[k][i] * patrones[k][j];
				}
				pesos[i][j] = sum;
			}

		}
		flag = true;
		for(int i = 0; i < patrones.size(); i++){
			res = evaluate(patrones[i]);
			if(!(patrones[i] == res)){
				flag = false;
				break;
			}
		}
		if(flag == true) break;
		if(count == limit) break;
	}
}

#endif