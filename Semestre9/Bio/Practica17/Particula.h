#ifndef PARTICULA_H
#define PARTICULA_H

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

enum LETRAS{A,B,C,D,E};

typedef vector<int> Camino;
typedef vector<tuple<int,int>> SecuanciaSO;
typedef vector<vector<double>> matrix;

SecuanciaSO operator-(Camino a, Camino b){
	SecuanciaSO res;
	bool state = 0;
	int pos1 = 0;
	int actual = 0;
	bool flag = false;
	while(true){
		flag = false;
		for(int i = 0; i < a.size(); i++){
			if(a[i] != b[i]){
				flag = true;
				pos1 = i;
				actual = a[i];
				for(int j = 0; j < b.size(); j++){
					if(b[j] == actual){
						res.push_back(make_tuple(pos1,j));
						swap(a[pos1], a[j]);
						break;
					}
				}
			}
		}
		if(flag == false) break;
	}
	return res;
}

SecuanciaSO operator+(SecuanciaSO a, SecuanciaSO b){
	SecuanciaSO res;
	res.insert(res.end(), a.begin(), a.end());
	res.insert(res.end(), b.begin(), b.end());
	return res;
}

Camino operator+(Camino a, SecuanciaSO b){
	Camino res = a;
	for(auto tup : b){
		swap(res[get<0>(tup)], res[get<1>(tup)]);
	}
	return res;
}

string getLetra(int letra){
	switch(letra){
		case A: return "A";
		case B: return "B";
		case C: return "C";
		case D: return "D";
		case E: return "E";
	}
}

class Particula{
public:
	Particula(){}
	Particula(matrix & costos, int n){
		vector<bool> flag(n,0);
		int randNum = 0;

		//Inicial
		camino.push_back(A);
		flag[A] = true;

		for(int i = 0; i < n; i++){
			randNum = rand() % n;
			if(flag[randNum] == true){
				for(int j = 0; j < n; j++){
					if(flag[j] == false){
						camino.push_back(j);
						flag[j] = true;		
					}
				}
			}
			else{
				camino.push_back(randNum);
				flag[randNum] = true;
			}
		}
		mejorLocal = camino;
		setFitness(costos);
		mejorFitnessLocal = fitness;
	}
	void print();
	void setFitness(matrix & costos);
	void setVelocidades(Camino & mejorGlobal);
	void setNewPosiciones(matrix & costos);

	
	Camino camino;
	SecuanciaSO velocidades;
	Camino mejorLocal;
	double fitness;
	double mejorFitnessLocal;
};

void Particula::print(){
	cout<<"Actual:[";
	for(int i : camino){
		cout<<getLetra(i)<<" ";
	}
	cout<<"]"<<endl;
	cout<<"Costo Actual:"<<fitness<<endl;
	cout<<"Mejor:[";
	for(int i : mejorLocal){
		cout<<getLetra(i)<<" ";
	}
	cout<<"]"<<endl;
	cout<<"Costo del Mejor:"<<mejorFitnessLocal<<endl;
	cout<<"Velocidad:[";
	for(auto tup : velocidades){
		cout<<"("<<get<0>(tup)<<","<<get<1>(tup)<<") ";
	}
	cout<<"]"<<endl;
}

void Particula::setFitness(matrix & costos){
	fitness = 0;
	int estadoAns = -1;
	for(int i : camino){
		if(estadoAns == -1){
			estadoAns = i;
			continue;
		}
		fitness += costos[estadoAns][i];
		estadoAns = i;
	}
}

void Particula::setVelocidades(Camino & mejorGlobal){
	velocidades = (mejorLocal - camino) + (mejorGlobal - camino);
}

void Particula::setNewPosiciones(matrix & costos){
	camino = camino + velocidades;
	setFitness(costos);
	if(mejorFitnessLocal < fitness){
		mejorLocal = camino;
		mejorFitnessLocal = fitness;
	}
}



#endif