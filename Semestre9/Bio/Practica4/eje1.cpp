#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "AlgoGen2.h"

using namespace std;

				       //       A   B    C   D   E
vector<vector<int>> matrix = {{-1,  2,  2,  1,  4}, //A
					   		  { 2, -1,  3,  2,  3}, //B
					   		  { 2,  3, -1,  2,  2}, //C
					   		  { 1,  2,  2, -1,  4}, //D
					   		  { 4,  3,  2,  4, -1}}; //E

bool sortFun(Cromosoma a, Cromosoma b){
	return a.fit > b.fit;
}

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

int funcCrossOBX(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
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
		hijo1 = vector<int>(padre.crom.size());
		hijo2 = vector<int>(padre.crom.size());
		elegidos.clear();
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
		flagHijo1 = vector<bool>(padre.crom.size(), 0);
		flagHijo2 = vector<bool>(padre.crom.size(), 0);
		for(int i = 0; i < padre.crom.size(); i++){
			if(elegidos[i] == false){
				hijo1[i] = padre.crom[i];
				flagHijo1[hijo1[i]] = true;
				hijo2[i] = madre.crom[i];
				flagHijo2[hijo2[i]] = true;
			}
		}		
		for(int i = 0; i < padre.crom.size(); i++){
			if(elegidos[i] == true){
				for(int j = 0; j < padre.crom.size(); j++){
					if(flagHijo1[madre.crom[j]] == false){
						flagHijo1[madre.crom[j]] = true;
						valor = madre.crom[j];
						break;
					}
				}
				hijo1[i] = valor; 
				for(int j = 0; j < padre.crom.size(); j++){
					if(flagHijo2[padre.crom[j]] == false){
						flagHijo2[padre.crom[j]] = true;
						valor = padre.crom[j];
						break;
					}
				}
				hijo2[i] = valor; 
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

int searchPos(CromVal crom, int val){
	for(int i = 0; i < crom.size(); i++){
		if(crom[i] == val) return i;
	}
	return -1;
}

int funcCrossCX(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
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
		int pos = 0;
		while(true){
			if(flagHijo1[padre.crom[pos]] == true) break;
			hijo1[pos] = padre.crom[pos];
			flagHijo1[hijo1[pos]] = true;
			pos = searchPos(padre.crom, madre.crom[pos]);
			for(int i = 0; i < hijo1.size(); i++){
				if(hijo1[i] == -1) hijo1[i] = madre.crom[i];
			}
		}
		while(true){
			if(flagHijo2[madre.crom[pos]] == true) break;
			hijo2[pos] = madre.crom[pos];
			flagHijo2[hijo2[pos]] = true;
			pos = searchPos(madre.crom, padre.crom[pos]);
			for(int i = 0; i < hijo2.size(); i++){
				if(hijo2[i] == -1) hijo2[i] = padre.crom[i];
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


int funcCrossPMX(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
	int faltantes = 0;
	int numRandom = 0;
	int point1 = 1;
	int point2 = 4;
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
		flagHijo1 = vector<bool>(padre.crom.size(), 0);
		flagHijo2 = vector<bool>(padre.crom.size(), 0);
		cout<<"Punto de corte 1: "<<point1<<endl;
		cout<<"Punto de corte 2: "<<point2<<endl;
		hijo1 = padre.crom;
		hijo2 = madre.crom;
		int pos1 = 0;
		int pos2 = 0;
		for(int i = point1; i < point2; i++){
			pos1 = searchPos(hijo1, hijo2[i]);
			pos2 = searchPos(hijo2, hijo1[i]);
			swap(hijo1[i], hijo2[i]);
			hijo1[pos1] = hijo2[i];
			hijo2[pos2] = hijo1[i];
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

Poblacion nextGen(Poblacion pobl){
	Poblacion res;
	sort(pobl.begin(), pobl.end(), sortFun);
	res.insert(res.begin(), pobl.begin(), pobl.begin() + (pobl.size() / 2));
	return res;
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
	int tamCrom = 5;
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);
	float probMut = stof(sProbMut);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funcFitness, funcTournamentSelect, funcCrossPBX, mutFunc, nextGen);
	ag.run();
}
