#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "AlgoMem.h"

using namespace std;

				       //       A   B   C   D   E   F   G   H   I   J
vector<vector<int>> matrix = {{ 0, 12,  3, 23,  1,  5, 23, 56, 12, 11}, //A
					   		  {12,  0,  9, 18,  3, 41, 45,  5, 41, 27}, //B
					   		  { 3,  9,  0, 89, 56, 21, 12, 48, 14, 29}, //C
					   		  {23, 18, 89,  0, 87, 46, 75, 17, 50, 42}, //D
					   		  { 1,  3, 56, 87,  0, 55, 22, 86, 14, 33}, //E
					   		  { 5, 41, 21, 46, 55,  0, 21, 76, 54, 81}, //F
					   		  {23, 45, 12, 75, 22, 21,  0, 11, 57, 48}, //G
					   		  {56,  5, 48, 17, 86, 76, 11,  0, 63, 24}, //H
					   		  {12, 41, 14, 50, 14, 54, 57, 63,  0,  9}, //I
					   		  {11, 27, 29, 42, 33, 81, 48, 24,  9,  0}};//J

Cromosoma getMayor(vector<Cromosoma> contrincantes){
	Cromosoma res;
	bool flag = false;
	for(Cromosoma c : contrincantes){
		if(flag == false){
			res = c;
			flag = true;	
		} 
		else if(res.fit < c.fit) res = c;
	}
	return res;
}

FitVal funcFitness(Cromosoma crom){
	FitVal res = 0;
	for(int i = 0; i < crom.crom.size() - 1; i++){
		res += matrix[crom.crom[i]][crom.crom[i+1]];
	}
	res *= -1;
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
			padres[j] = getMayor(contrincantes);
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

Cromosoma simpleMutFunc(Cromosoma val){
	CromVal res = val.crom;
	Cromosoma temp;
	int posRandom1 = 0;
	int posRandom2 = 0;
	cout<<"Mutacion:(";
	posRandom1 = rand() % val.crom.size();
	posRandom2 = rand() % val.crom.size();
	if(posRandom1 == posRandom2) posRandom2 = (posRandom2 + 1) % val.crom.size();
	cout<<posRandom1<<","<<posRandom2<<") ";
	temp = Cromosoma(res);
	temp.printCrom();
	swap(res[posRandom1], res[posRandom2]);
	temp = Cromosoma(res);
	cout<<" -> ";
	temp.printCrom();
	return Cromosoma(res);
}

void optFuncEscalaPrimera(Poblacion pobl, Poblacion & resPobl){
	int H = 5;
	int actualCount = 0;
	Cromosoma pariente;
	Cromosoma actualCrom;
	for(Cromosoma crom : pobl){		
		actualCrom = crom;
		cout<<"------INIT----"<<endl;
		actualCrom.printCrom();
		cout<<" "<<actualCrom.fit;
		cout<<endl;
		while(actualCount != H){
			pariente = simpleMutFunc(actualCrom);
			pariente.fit = funcFitness(pariente);
			cout<<" "<<pariente.fit<<endl;
			if(pariente.fit > actualCrom.fit){
				actualCount = 0;
				actualCrom = pariente;
				cout<<endl;
				actualCrom.printCrom();
				cout<<" "<<actualCrom.fit<<endl;
			}
			else actualCount++;
		}
		cout<<"-------END:";
		actualCrom.printCrom();
		cout<<actualCrom.fit<<"--------"<<endl<<endl;
		resPobl.push_back(actualCrom);
		actualCount = 0;
	}
	cout<<endl;
}

Poblacion nextGen(Poblacion pobl){
	Poblacion res;
	sort(pobl.begin(), pobl.end(), sortFun);
	res.insert(res.begin(), pobl.begin(), pobl.begin() + (pobl.size() / 2));
	return res;
}

int main(int argc, char ** argv){
	srand(time(NULL));
	if(argc != 4){
		cout<<"Argumentos <Tam poblacion> <Iteraciones> <Prob Cross>"<<endl;
		return 0;
	}
	string sTamP(argv[1]);
	string sNumIter(argv[2]);
 	string sProbCruz(argv[3]);
	int tamP = stoi(sTamP);
	int tamCrom = 10;
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, funcFitness, funcTournamentSelect, funcCrossPBX, optFuncEscalaPrimera, nextGen);
	ag.run();
}