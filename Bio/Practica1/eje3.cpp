#include <iostream>
#include "AlgoGen.h"

using namespace std;

int to_int(string num){
	int res = 0;
	int actual = 0;
	char c = 0;
	for(int i = num.size() - 1; i >= 0; i--){
		c = num[i];
		if(c == '1') res += pow(2, actual);
		actual++;
	}
	return res;
}

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
	string sNum1 = "";
	string sNum2 = "";
	int num1 = 0;
	int num2 = 0;
	bool flag = false;
	for(int i = 0; i < crom.crom.size(); i++){
		if(i == crom.crom.size() / 2) flag = true;
		if(flag == false) sNum1.insert(sNum1.begin(), crom.crom[i]);
		else sNum2.insert(sNum2.begin(), crom.crom[i]);
	}
	num1 = to_int(sNum1);
	num2 = to_int(sNum2);
	res = num1 - num2;
	return res;
}

vector<ParCromo> funcTournamentSelect(Poblacion pobl, int n){
	cout<<"Seleccion de padres metodo torneo"<<endl;
	int tamTorneo = 2;
	cout<<"Tam de torneo: "<<tamTorneo + 1<<endl;
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

int funcCrossUniform(vector<ParCromo> padres, float probCruz, Poblacion & resPobl){
	int faltantes = 0;
	int realProbCruz = probCruz * 100;
	Cromosoma padre;
	Cromosoma madre;
	Cromosoma mascara;
	int numRandom = 0;
	string newCromA = "";
	string newCromB = "";
	char c = 0;
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
		cout<<"Cruzamiento uniforme"<<endl;
		newCromA.clear();
		newCromB.clear();
		mascara = Cromosoma(padre.crom.size());
		cout<<"Mascara: "<<mascara.crom<<endl;
		for(int i = 0; i < mascara.crom.size(); i++){
			c = mascara.crom[i];
			if(c == '0'){
				newCromA.push_back(padre.crom[i]);
				newCromB.push_back(madre.crom[i]);
			}
			else if(c == '1'){
				newCromA.push_back(madre.crom[i]);
				newCromB.push_back(padre.crom[i]);
			}
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
	cout<<"Probabilidad de Mutacion: "<<probMut<<endl<<endl;

	AlgoGen ag(tamP, tamCrom, numIter, probCruz, probMut, funcFitness, funcTournamentSelect, funcCrossUniform, mutFunc, nextGen);
	ag.run();
}