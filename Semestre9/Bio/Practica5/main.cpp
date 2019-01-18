#include <iostream>
#include <cstdlib>
#include <ctime>
#include "AlgoGen.h"

using namespace std;

bool sortFun(Cromosoma a, Cromosoma b){
	return a.error < b.error;
}

Cromosoma funRep(Poblacion pobl){
	cout<<"Reproduccion"<<endl;
	int tamTorneo = 3;
	int numRandom = 0;
	vector<Cromosoma> contrincantes;
	for(int i = 0; i < tamTorneo; i++){
		numRandom = rand() % pobl.size();
		contrincantes.push_back(pobl[numRandom]);
	}
	sort(contrincantes.begin(), contrincantes.end(), sortFun);
	cout<<"Seleccionado: ";
	contrincantes.front().print();
	cout<<endl;
	Cromosoma res(&contrincantes.front());
	return res;
}

ParCromo selectFunc(Poblacion pobl){
	int tamTorneo = 3;
	int numRandom = 0;
	vector<Cromosoma> contrincantes;
	vector<Cromosoma> padres;
	for(int j = 0; j < tamTorneo; j++){
		contrincantes.clear();
		for(int i = 0; i < tamTorneo; i++){
			numRandom = rand() % pobl.size();
			contrincantes.push_back(pobl[numRandom]);
		}
		sort(contrincantes.begin(), contrincantes.end(), sortFun);
		padres.push_back(contrincantes.front());
	}
	cout<<"Seleccionados:"<<endl;
	padres[0].print();
	padres[1].print();
	cout<<endl;
	return make_tuple(padres[0], padres[1]);
}

ParCromo crossFunc(ParCromo padres){
	Cromosoma padre;
	Cromosoma madre;
	tie(padre, madre) = padres;
	Cromosoma hijo1(&padre);
	Cromosoma hijo2(&madre);
	int randomFun = rand() % 2;
	int randomConst = rand() % 3;
	PGTree::PGNode * temp = nullptr;
	cout<<"Cruzamiento: ";
	if(randomConst == 2){
		cout<<"Funcion "<<randomFun<<":"<<endl;
		temp = hijo1.root->hijos[randomFun];
		hijo1.root->hijos[randomFun] = hijo2.root->hijos[randomFun];
		hijo2.root->hijos[randomFun] = temp;
	}
	else{
		cout<<"Hijo "<<randomConst<<" de la funcion "<<randomFun<<":"<<endl;
		temp = hijo1.root->hijos[randomFun]->hijos[randomConst];
		hijo1.root->hijos[randomFun]->hijos[randomConst] = hijo2.root->hijos[randomFun]->hijos[randomConst];
		hijo2.root->hijos[randomFun]->hijos[randomConst] = temp;
	}
	hijo1.print();
	hijo2.print();
	cout<<endl<<endl;
	return make_tuple(hijo1, hijo2);
}

Cromosoma mutFunc(Cromosoma crom){
	Cromosoma res(&crom);
	int randomFun = rand() % 3;
	int randomConst = rand() % 3;
	int numRandom = 0;
	cout<<"Mutacion:"<<endl;
	cout<<"Original: ";
	res.print();
	cout<<randomFun<<endl;
	cout<<randomConst<<endl;
	if(randomFun == 2){
		numRandom = rand() % res.funciones.size();
		res.root->varOrFunc = res.funciones[numRandom];
	}
	else{
		if(randomConst == 2){
			numRandom = rand() % res.funciones.size();
			res.root->hijos[randomFun]->varOrFunc = res.funciones[numRandom];
		}
		else{
			numRandom = rand() % 2;
			if(numRandom == 0){
				numRandom = rand() % res.constantes.size();
				res.root->hijos[randomFun]->hijos[randomConst]->constante = res.constantes[numRandom];
			}
			else{
				numRandom = rand() % res.variables.size();
				res.root->hijos[randomFun]->hijos[randomConst]->varOrFunc = res.variables[numRandom];
			}
		}
	}
	cout<<"Mutado: ";
	res.print();
	cout<<endl;
	return res;
}

void nextFunc(Poblacion & pobl, int tamP){
	sort(pobl.begin(), pobl.end(), sortFun);
	while(pobl.size() > tamP){
		pobl.pop_back();
	}
}

int main(){
	srand(time(NULL));


	Variables variables = {"x"};
	Funciones funciones = {"+", "-", "*", "%", "/"};
	Constantes constantes = {-5,-4,-3,-2,-1,0,1,2,3,4,5};

	Entradas entradas = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
	Entradas deseados = {0, 0.005, 0.02, 0.045, 0.08, 0.125, 0.18, 0.245, 0.32, 0.405};

	float probRep = 0.1;
	float probCruz = 0.8;
	float probMut = 0.1;
	int tamP = 500;
	int numIter = 30;

	AlgoGen ag(tamP, numIter, probRep, probCruz, probMut, funRep, selectFunc, crossFunc, mutFunc, nextFunc,
				variables, constantes, funciones);
	ag.run(entradas, deseados);
}