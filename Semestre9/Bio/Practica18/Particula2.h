#ifndef PARTICULA_H
#define PARTICULA_H

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

enum LETRAS{A,B,C,D,E,F,G,H,I,J};

#define A_DOMINA_B 0
#define B_DOMINA_A 1
#define MISMA_FRONTERA 2

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
		case F: return "F";
		case G: return "G";
		case H: return "H";
		case I: return "I";
		case J: return "J";
	}
}

class Particula{
public:
	Particula(){}
	Particula(Particula * p){
		this->camino = p->camino;
		this->velocidades = p->velocidades;
		this->fitness = p->fitness;
	}
	Particula(matrix & costos1, matrix & costos2, int n){
		fitness = vector<double>(2,0);
		vector<bool> flag(n,0);
		int randNum = 0;

		//Inicial
		//camino.push_back(A);
		//flag[A] = true;

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
		mejorLocal.push_back(Particula(this));
		setFitness(costos1, costos2);

	}
	void print(bool flag);
	void setFitness(matrix & costos1, matrix & costos2);
	void setVelocidades(Camino & mejorGlobal);
	void setNewPosiciones(matrix & costos1, matrix & costos2);
	void verifyFrontera(Particula newP);

	
	Camino camino;
	SecuanciaSO velocidades;
	vector<Particula> mejorLocal;
	vector<double> fitness;
};

int dominancia(Particula a, Particula b){
	if(a.fitness[0] < b.fitness[0] and a.fitness[1] < b.fitness[1]) return A_DOMINA_B;
	if(a.fitness[0] > b.fitness[0] and a.fitness[1] > b.fitness[1]) return B_DOMINA_A;
	return MISMA_FRONTERA;
}


void Particula::print(bool flag){
	cout<<"Camino:[";
	for(int i : camino){
		cout<<getLetra(i)<<" ";
	}
	cout<<"] ";
	cout<<"Velocidad:[";
	for(auto tup : velocidades){
		cout<<"("<<get<0>(tup)<<","<<get<1>(tup)<<") ";
	}
	cout<<"] ";
	cout<<" Costo Actual: [";
	for(int i : fitness){
		cout<<i<<" ";
	}
	cout<<"]"<<endl;
	if(flag == true){
		cout<<"Frontera:"<<endl;
		for(Particula p : mejorLocal){
			p.print(false);
		}
	}
}

void Particula::verifyFrontera(Particula newP){
	vector<Particula> res;
	bool flag = true;
	int dom = 0;
	for(Particula p : mejorLocal){
		dom = dominancia(newP, p);
		if(dom == B_DOMINA_A){
			flag = false;
			res.push_back(p);
		} 
		else if(dom == MISMA_FRONTERA) res.push_back(p);

	}
	if(flag == true) res.push_back(newP);
	mejorLocal = res;
}

void Particula::setFitness(matrix & costos1, matrix & costos2){
	fitness[0] = 0;
	fitness[1] = 0;
	int estadoAns = -1;
	for(int i : camino){
		if(estadoAns == -1){
			estadoAns = i;
			continue;
		}
		fitness[0] += costos1[estadoAns][i];
		fitness[1] += costos2[estadoAns][i];
		estadoAns = i;
	}
}

void Particula::setVelocidades(Camino & mejorGlobal){
	int randNumber = rand() % mejorLocal.size();
	velocidades = (mejorLocal[randNumber].camino - camino) + (mejorGlobal - camino);
}

void Particula::setNewPosiciones(matrix & costos1, matrix & costos2){
	camino = camino + velocidades;
	setFitness(costos1, costos2);
	verifyFrontera(Particula(this));
}



#endif