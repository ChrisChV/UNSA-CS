#include <iostream>
#include "AlgoGen.h"

using namespace std;

vector<int> profits = {6,5,8,9,6,7,3};
vector<int> pesos = {2,3,6,7,5,9,4};
int pesoMaximo = 9;


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
	int x = 0;
	int sumaPesos = 0;
	int penalty = 0;
	for(int i = 0; i < crom.crom.size(); i++){
		if(crom.crom[i] == '0') x = 0;
		else x = 1;
		sumaPesos += pesos[i];
		res += x * profits[i];
		penalty += x * pesos[i];
	}
	penalty = sumaPesos * abs(penalty - pesoMaximo);
	res = res - penalty;
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
				cout<<k + 1<<")\t"<<contrincantes.back().crom<<"\t"<<contrincantes.back().fit<<endl;
			}
			padres[j] = getMayor(contrincantes);
			cout<<"Ganador:\t"<<padres[j].crom<<endl<<endl;
			cout<<endl;
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
		if(numRandom > realProbMut) continue;
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
	if(argc != 5){
		cout<<"Argumentos <Tam poblacion> <Iteraciones> <Prob Cross> <Prob Mut>"<<endl;
		return 0;
	}
	string sTamP(argv[1]);
	string sNumIter(argv[2]);
 	string sProbCruz(argv[3]);
	string sProbMut(argv[4]);
	int tamP = stoi(sTamP);
	int tamCrom = profits.size();
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);
	float probMut = stof(sProbMut);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funcFitness, funcTournamentSelect, funcCrossOnePoint, mutFunc, nextGen);
	ag.run();
}