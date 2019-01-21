#include <iostream>
#include <cstdlib>
#include <ctime>
#include "AlgoGen1.h"

using namespace std;

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
	float x = crom.crom[0];
	float y = crom.crom[1];
	float x2 = pow(x,2);
	float y2 = pow(y,2);
	res = 0.5 - ( (pow( (sin( sqrt(x2 + y2) )), 2 ) - 0.5) / pow(1.0 + 0.0001 * (x2 + y2), 2));
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
		resPobl.push_back(hijo);
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

Poblacion nextGen(Poblacion pobl){
	sort(pobl.begin(), pobl.end(), sortFun);
	return pobl;
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
	int tamCrom = 2;
	int numIter = stoi(sNumIter);
	float probCruz = stof(sProbCruz);
	float probMut = stof(sProbMut);

	cout<<"Tam de la poblacion: "<<tamP<<endl;
	cout<<"Tam de los cromosomas: "<<tamCrom<<endl;
	cout<<"Iteraciones: "<<numIter<<endl;
	cout<<"Probabilidad de cruzamiento: "<<probCruz<<endl;
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funcFitness, funcTournamentSelect, funcCrossBLX, mutUniformFunc, nextGen);
	ag.run();
}