#include <iostream>
#include "AlgoGen.h"

using namespace std;

bool sortFun(Cromosoma a, Cromosoma b){
	return a.fit > b.fit;
}

FitVal funcFitness(Cromosoma crom){
	FitVal res = 0;
	int num = crom.bin_to_int();
	res = num;
	res = pow(num,2);
	return res;
}

vector<ParCromo> funcRouleteSelect(Poblacion pobl, int n){
	cout<<"Seleccion de Padres Metodo Ruleta"<<endl;
	vector<ParCromo> res;
	vector<int> probabilidades;
	int suma = 0;
	for(int i = 0; i < pobl.size(); i++){
		suma += pobl[i].fit;
	}
	for(int i = 0; i < pobl.size(); i++){
		pobl[i].probabilidad = (pobl[i].fit * 100) / suma;
		if(probabilidades.size() == 0){
			probabilidades.push_back(pobl[i].probabilidad);
		}
		else if(i != pobl.size() - 1){
			probabilidades.push_back(probabilidades.back() + pobl[i].probabilidad);
		}
		else probabilidades.push_back(100);
	}
	int actual = 1;
	for(Cromosoma c : pobl){
		cout<<actual++<<")\t"<<c.crom<<"\t"<<c.fit<<"\t"<<c.probabilidad<<endl;
	}
	cout<<endl;
	Cromosoma padres[2];
	int numRandom = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < 2; j++){
			numRandom = rand() % 101;
			for(int k = 0; k < probabilidades.size(); k++){
				if(probabilidades[k] >= numRandom){
					padres[j] = pobl[k];
					break;	
				}
			}
		}
		res.push_back(make_tuple(padres[0], padres[1]));
	}
	return res;
}

int funcCrossOnePoint(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
	int faltantes = 0;
	int numRandom = 0;
	int puntoRandom = 0;
	int realProbCruz = probCruz * 100;
	Cromosoma padre;
	Cromosoma madre;
	string newCromA = "";
	string newCromB = "";
	for(ParCromo par : padres){
		tie(padre, madre) = par;
		cout<<"Padre: "<<padre.crom<<endl;
		cout<<"Madre: "<<madre.crom<<endl;
		numRandom = rand() % 101;
		if(numRandom > realProbCruz){
			cout<<"Sin Cruzamiento"<<endl;
			cout<<endl;
			faltantes++;
			continue;	
		} 
		cout<<"Cruzamiento de un Punto"<<endl;
		puntoRandom = rand() % (padre.crom.size() - 1) + 1;
		cout<<"Punto de corte: "<<puntoRandom<<endl;
		newCromA.clear();
		newCromB.clear();
		if(padre.crom == madre. crom){
			newCromA = padre.crom;
			newCromB.insert(newCromB.end(), newCromA.begin() + puntoRandom, newCromA.end());
			newCromB.insert(newCromB.end(), newCromA.begin(), newCromA.begin() + puntoRandom);
		}
		else{
			newCromA.insert(newCromA.end(), padre.crom.begin(), padre.crom.begin() + puntoRandom);
			newCromA.insert(newCromA.end(), madre.crom.begin() + puntoRandom, madre.crom.end());
			newCromB.insert(newCromB.end(), madre.crom.begin(), madre.crom.begin() + puntoRandom);
			newCromB.insert(newCromB.end(), padre.crom.begin() + puntoRandom, padre.crom.end());	
		}
		cout<<newCromA<<endl;
		cout<<newCromB<<endl;
		cout<<endl;
		resPobl.push_back(Cromosoma(newCromA));
		resPobl.push_back(Cromosoma(newCromB));
	}
	return faltantes;
}


void mutFunc(Poblacion & pobl, float probMut){
	int realProbMut = probMut * 100;
	int numRandom = 0;
	int posRandom = 0;
	for(int i = 0; i < pobl.size(); i++){
		numRandom = rand() % 101;
		if(numRandom > probMut) continue;
		cout<<"Mutacion "<<pobl[i].crom<<" -> ";
		posRandom = rand() % pobl.front().crom.size();
		if(pobl[i].crom[posRandom] == '0') pobl[i].crom[posRandom] = '1';
		else pobl[i].crom[posRandom] = '0';
		cout<<pobl[i].crom<<endl;
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
	if(argc != 6){
		cout<<"Argumentos <Tam poblacion> <Tam cromososma> <Iteraciones> <Prob Cross> <Prob Mut>"<<endl;
		return 0;
	}
	string sTamP(argv[1]);
	string sTamCrom(argv[2]);
	string sNumIter(argv[3]);
 	string sProbCruz(argv[4]);
	string sProbMut(argv[5]);
	int tamP = stoi(sTamP);
	int tamCrom = stoi(sTamCrom);
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);
	float probMut = stof(sProbMut);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;;

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funcFitness, funcRouleteSelect, funcCrossOnePoint, mutFunc, nextGen);
	ag.run();
}