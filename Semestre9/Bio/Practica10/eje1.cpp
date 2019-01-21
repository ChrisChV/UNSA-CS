#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include "AlgoGen1.h"

using namespace std;

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
	float x = crom.crom[0];
	float y = crom.crom[1];
	res = 4 * pow(x,2) + 4 * pow(y,2);
	return res;
}

FitVal funcFitness2(Cromosoma crom){
	FitVal res = 0;
	float x = crom.crom[0];
	float y = crom.crom[1];
	res = pow(x - 5,2) + pow(y - 5,2);
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

int funcCrossBLX(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
	int faltantes = 0;
	int numRandom = 0;
	int realProbCruz = probCruz * 100;
	int numRandFlag = rand() % 2;
	float beta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	if(numRandFlag == 0) beta += 0.5;
	else beta -= 0.5;
	Cromosoma padre;
	Cromosoma madre;
	CromVal hijo;
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
		hijo.clear();
		cout<<"Cruzamiento BLX-(0.5)"<<endl;
		cout<<"Beta: "<<beta<<endl;
		for(int i = 0; i < padre.crom.size(); i++){
			hijo.push_back(padre.crom[i] + beta * (madre.crom[i] - padre.crom[i]));
		}
		if(hijo[0] < 0 or hijo[0] > 5) hijo[0] = padre.crom[0];
		if(hijo[1] < 0 or hijo[1] > 3) hijo[1] = padre.crom[1];
		resPobl.push_back(Cromosoma(hijo));
		resPobl.back().printCrom();
		cout<<endl<<endl;
	}
	return faltantes;
}

void mutUniformFunc(Poblacion & pobl, float probMut){
	int realProbMut = probMut * 100;
	float signo = 0;
	float valRandom = 0;
	int posRandom = 0;
	int numRandom = 0;
	for(int i = 0; i < pobl.size(); i++){
		numRandom = rand() % 101;
		if(numRandom > realProbMut) continue;
		cout<<"Mutacion ";
		pobl[i].printCrom();
		cout<<" -> ";
		posRandom = rand() % pobl.front().crom.size();
		signo = rand() % 2;
		valRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));		
		if(signo == 1) valRandom *= -1;
		pobl[i].crom[posRandom] = valRandom;
		pobl[i].printCrom();
		cout<<endl<<endl;
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
	//srand(113);
	if(argc != 5){
		cout<<"Argumentos <Tam poblacion> <Iteraciones> <Prob Cross> <Prob Mut>"<<endl;
		return 0;
	}
	string sTamP(argv[1]);
	string sNumIter(argv[2]);
 	string sProbCruz(argv[3]);
	string sProbMut(argv[4]);
	int tamP = stoi(sTamP);
	int tamCrom = 2;
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);
	float probMut = stof(sProbMut);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;

	vector<AlgoGenFitnessFunc> funciones = {funcFitness1, funcFitness2};

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funciones, funcTournamentSelect, funcCrossBLX, mutUniformFunc, nextGen);
	ag.run();
}