#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include "AlgoGen2.h"

using namespace std;

				       //       A   B   C   D   E   F   G   H   I   J
vector<vector<int>> matrixA = {{ 0, 12,  3, 23,  1,  5, 23, 56, 12, 11}, //A
					   		  {12,  0,  9, 18,  3, 41, 45,  5, 41, 27}, //B
					   		  { 3,  9,  0, 89, 56, 21, 12, 48, 14, 29}, //C
					   		  {23, 18, 89,  0, 87, 46, 75, 17, 50, 42}, //D
					   		  { 1,  3, 56, 87,  0, 55, 22, 86, 14, 33}, //E
					   		  { 5, 41, 21, 46, 55,  0, 21, 76, 54, 81}, //F
					   		  {23, 45, 12, 75, 22, 21,  0, 11, 57, 48}, //G
					   		  {56,  5, 48, 17, 86, 76, 11,  0, 63, 24}, //H
					   		  {12, 41, 14, 50, 14, 54, 57, 63,  0,  9}, //I
					   		  {11, 27, 29, 42, 33, 81, 48, 24,  9,  0}};//J


				       //       A   B   C   D   E   F   G   H    I   J
vector<vector<int>> matrixB = {{ 0, 22, 47, 15, 63, 21, 23, 16, 11,  9}, //A
					   		  { 22,  0, 18, 62, 41, 52, 13, 11, 26, 43}, //B
					   		  { 47, 18,  0, 32, 57, 44, 62, 20,  8, 36}, //C
					   		  { 15, 62, 32,  0, 62, 45, 75, 63, 14, 12}, //D
					   		  { 63, 41, 57, 62,  0,  9, 99, 42, 56, 23}, //E
					   		  { 21, 52, 44, 45,  9,  0, 77, 58, 22, 14}, //F
					   		  { 23, 13, 62, 75, 99, 77,  0, 30, 25, 60}, //G
					   		  { 16, 11, 20, 63, 42, 58, 30,  0, 66, 85}, //H
					   		  { 11, 26,  8, 14, 56, 22, 25, 66,  0, 54}, //I
					   		  {  9, 43, 36, 12, 23, 14, 60, 85,  54,  0}};//J

bool sortFun(Cromosoma a, Cromosoma b){
	return a.fit > b.fit;
}

Cromosoma getMenor(vector<Cromosoma> contrincantes){
	Cromosoma res;
	bool flag = false;
	FitVal sum = 0;
	FitVal actualRes = 0;
	for(Cromosoma c : contrincantes){
		sum = 0;
		for(FitVal f : c.fit){
			sum += f;
		}
		if(flag == false){
			res = c;
			actualRes = sum;
			flag = true;	
		} 
		else if(actualRes > sum){
			res = c;
			actualRes = sum;	
		} 
	}
	return res;
}

FitVal funcFitness1(Cromosoma crom){
	FitVal res = 0;
	for(int i = 0; i < crom.crom.size() - 1; i++){
		res += matrixA[crom.crom[i]][crom.crom[i+1]];
	}
	return res;
}

FitVal funcFitness2(Cromosoma crom){
	FitVal res = 0;
	for(int i = 0; i < crom.crom.size() - 1; i++){
		res += matrixB[crom.crom[i]][crom.crom[i+1]];
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

int funcCrossPBX(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
	int faltantes = 0;
	int numRandom = 0;
	int realProbCruz = probCruz * 100;
	Cromosoma padre;
	Cromosoma madre;
	CromVal hijo1;
	CromVal hijo2;
	vector<bool> elegidos;
	vector<bool> flagHijo1;
	vector<bool> flagHijo2;
	int valor = 0;
	for(ParCromo par : padres){
		tie(padre, madre) = par;
		cout<<"Padre: ";
		padre.printCrom();
		cout<<endl;
		cout<<"Madre: ";
		madre.printCrom();
		cout<<endl;
		numRandom = rand() % 101;
		if(numRandom > realProbCruz){
			cout<<"Sin Cruzamiento"<<endl;
			cout<<endl;
			faltantes++;
			continue;	
		}
		hijo1 = vector<int>(padre.crom.size(), -1);
		hijo2 = vector<int>(padre.crom.size(), -1);
		flagHijo1 = vector<bool>(padre.crom.size(), 0);
		flagHijo2 = vector<bool>(padre.crom.size(), 0);
		elegidos = vector<bool>(padre.crom.size(), 0);
		for(int i = 0; i < padre.crom.size(); i++){
			numRandom = rand() % 101;
			if(numRandom > 50) elegidos[i] = true;
		}
		cout<<"Puntos elegidos: ";
		for(int i = 0; i < padre.crom.size(); i++){
			if(elegidos[i] == true){
				cout<<i<<" ";
			}
		}
		cout<<endl;
		for(int i = 0; i < elegidos.size(); i++){
			if(elegidos[i] == true){
				hijo1[i] = madre.crom[i];
				flagHijo1[hijo1[i]] = true;
				hijo2[i] = padre.crom[i];
				flagHijo2[hijo2[i]] = true;
			}
		}
		int pos = 0;
		for(int i = 0; i < hijo1.size(); i++){
			if(hijo1[i] == -1){
				while(true){
					if(flagHijo1[padre.crom[pos]] == false){
						hijo1[i] = padre.crom[pos];
						flagHijo1[padre.crom[pos]] = true;
						pos++;
						break;
					}
					pos++;
				}
			}
		}
		pos = 0;
		for(int i = 0; i < hijo2.size(); i++){
			if(hijo2[i] == -1){
				while(true){
					if(flagHijo2[madre.crom[pos]] == false){
						hijo2[i] = madre.crom[pos];
						flagHijo2[madre.crom[pos]] = true;
						pos++;
						break;
					}
					pos++;
				}
			}
		}
		resPobl.push_back(Cromosoma(hijo1));
		resPobl.back().printCrom();
		cout<<endl;
		resPobl.push_back(Cromosoma(hijo2));
		resPobl.back().printCrom();
		cout<<endl<<endl;
	}
	return faltantes;
}

void mutFunc(Poblacion & pobl, float probMut){
	int realProbMut = probMut * 100;
	int numRandom = 0;
	int posRandom1 = 0;
	int posRandom2 = 0;
	for(int i = 0; i < pobl.size(); i++){
		numRandom = rand() % 101;
		if(numRandom > realProbMut) continue;
		cout<<"Mutacion ";
		pobl[i].printCrom();
		cout<<" -> ";
		posRandom1 = rand() % pobl.front().crom.size();
		posRandom2 = rand() % pobl.front().crom.size();
		if(posRandom1 == posRandom2) posRandom2 = (posRandom2 + 1) % pobl.front().crom.size();
		swap(pobl[i].crom[posRandom1], pobl[i].crom[posRandom2]);
		pobl[i].printCrom();
		cout<<endl;
	}
}

void setFronteras(Poblacion & pobl){
	int miFrontera = 0;
	bool flag = true;
	bool flag2 = false;
	while(flag){
		flag = false;
		for(int i = 0; i < pobl.size(); i++){
			miFrontera = 0;
			flag2 = false;
			for(int j = 0; j < pobl.size(); j++){
				if(i == j) continue;
				if(pobl[i].fit[0] > pobl[j].fit[0] and pobl[i].fit[1] > pobl[j].fit[1]){
					if(pobl[i].frontera < pobl[j].frontera + 1){
						flag = true;
						flag2 = true;
						pobl[i].frontera = pobl[j].frontera + 1;
					}
				}
			}	
			if(flag2 == false and pobl[i].frontera == -1){
				flag = true;
				pobl[i].frontera = 0;	
			} 
		}
	}
}

void sortVec(vector<Cromosoma> & vec){
	sort(vec.begin(), vec.end(), [](Cromosoma a, Cromosoma b){
		return a.fit[0] - a.fit[1] > b.fit[0] - b.fit[1];
	});
}

void setCDistance(vector<Cromosoma> & vec){
	sortVec(vec);
	for(int i = 1; i < vec.size() - 1; i++){
		vec[i].cDistance = 	abs(vec[i - 1].fit[0] - vec[i + 1].fit[0]) * 2 + abs(vec[i - 1].fit[1] - vec[i + 1].fit[1]) * 2;
	}
	sort(vec.begin(), vec.end(), [](Cromosoma a, Cromosoma b){
		return a.cDistance < b.cDistance;
	});
	sort(vec.begin() + 2, vec.end(), [](Cromosoma a, Cromosoma b){
		return a.cDistance > b.cDistance;
	});
}

Poblacion nextGen(Poblacion pobl, int tamP){
	Poblacion nextPobl;
	map<int, vector<Cromosoma>> fronteras;
	setFronteras(pobl);
	int actual = 1;
	cout<<endl<<"Fronteras"<<endl;
	for(Cromosoma c : pobl){
		cout<<actual++<<")\t";
		c.printCrom();
		cout<<endl;
	}
	map<int, vector<Cromosoma>>::iterator findRes;
	int maxFront = -1;
	for(Cromosoma c : pobl){
		findRes = fronteras.find(c.frontera);
		if(findRes == fronteras.end()){
			if(maxFront < c.frontera) maxFront = c.frontera;
			fronteras[c.frontera] = vector<Cromosoma>();
			findRes = fronteras.find(c.frontera);
		}
		findRes->second.push_back(c);
	}
	for(int i = 0; i < maxFront + 1; i++){
		if(nextPobl.size() == tamP) break;
		else if(nextPobl.size() + fronteras[i].size() <= tamP){
			sortVec(fronteras[i]);
			nextPobl.insert(nextPobl.begin(), fronteras[i].begin(), fronteras[i].end());
		}
		else{
			setCDistance(fronteras[i]);
			cout<<endl<<"Distancias"<<endl;
			actual = 1;
			for(Cromosoma c : fronteras[i]){
				cout<<actual++<<")\t";
				c.printCrom();
				cout<<endl;
			}
			int temp = nextPobl.size();
			for(int j = 0; j < tamP - temp; j++){
				nextPobl.push_back(fronteras[i][j]);
			}
			break;
		}
	}
	return nextPobl;
}

int main(int argc, char ** argv){
	srand(time(NULL));
	if(argc != 5){
		cout<<"Argumentos <Tam poblacion> <Iteraciones> <Prob Cross> <Prob Mut>"<<endl;
		return 0;
	}
	string sTamP(argv[1]);
	string sNumIter(argv[2]);
 	string sProbCruz(argv[3]);
	string sProbMut(argv[4]);
	int tamP = stoi(sTamP);
	int tamCrom = 10;
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);
	float probMut = stof(sProbMut);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;

	vector<AlgoGenFitnessFunc> funciones = {funcFitness1, funcFitness2};

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funciones, funcTournamentSelect, funcCrossPBX, mutFunc, nextGen);
	ag.run();
}
