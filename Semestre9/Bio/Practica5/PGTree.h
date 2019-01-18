#ifndef PGTREE_H
#define PGTREE_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <tuple>

using namespace std;

typedef double TerminalType;
typedef double EntradaType;
typedef vector<string> Variables;
typedef vector<TerminalType> Constantes;
typedef vector<string> Funciones;
typedef vector<EntradaType> Entradas;
typedef tuple<vector<EntradaType>, EntradaType> FitnessTuple;

#define FUNCTION 0
#define VARIABLE 1
#define CONSTANT 2



class PGTree{
	public:
		class PGNode{
			public:
				PGNode(){

				}
				PGNode(int estado, TerminalType constante){
					this->estado = estado;
					this->constante = constante;
					hijos[0] = hijos[1] = nullptr;
				}
				PGNode(int estado, string varOrFunc){
					this->estado = estado;
					this->varOrFunc = varOrFunc;
					hijos[0] = hijos[1] = nullptr;
				}
				void deleteNode(){
					if(hijos[0] != nullptr) hijos[0]->deleteNode();
					if(hijos[1] != nullptr) hijos[1]->deleteNode();
					delete hijos[0];
					delete hijos[1];
				}
				PGNode * hijos[2];
				int estado;
				TerminalType constante;
				string varOrFunc;
		};
		PGTree(){};
		PGTree(Variables variables, Constantes constantes, Funciones funciones);
		PGTree(PGTree * otherThree);
		void deleteTree(){
			root->deleteNode();
			delete root;
		}
		void print(){
			cout<<"("<<root->varOrFunc<<" ";
			for(int i = 0; i < 2; i++){
				cout<<"("<<root->hijos[i]->varOrFunc<<" ";
				for(int j = 0; j < 2; j++){
					if(root->hijos[i]->hijos[j]->estado == VARIABLE) cout<<"x ";
					else cout<<root->hijos[i]->hijos[j]->constante<<" ";
				}
				cout<<") ";
			}
			cout<<error<<endl;
		}
		FitnessTuple fitness(Entradas entradas, Entradas deseados);
		Variables variables;
		Constantes constantes;
		Funciones funciones;
		PGNode * root;
		EntradaType error;
};

EntradaType useFunction(string fun, TerminalType a, TerminalType b){
	if(fun == "+") return a + b;
	else if(fun == "-") return a - b;
	else if(fun == "*") return a * b;
	else if(fun == "%") {
		if(b == 0) return 0;
		return fmod(a,b);
	}
	else if(fun == "/") {
		if(b == 0) return 0;
		return a / b;
	}
}

FitnessTuple PGTree::fitness(Entradas entradas, Entradas deseados){
	vector<EntradaType> salidas;
	EntradaType tempRes[2];
	EntradaType tempVar[2];
	for(EntradaType entrada : entradas){
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 2; j++){
				if(root->hijos[i]->hijos[j]->estado == VARIABLE) tempVar[j] = entrada;
				else tempVar[j] = root->hijos[i]->hijos[j]->constante;
			}
			tempRes[i] = useFunction(root->hijos[i]->varOrFunc, tempVar[0], tempVar[1]);
		}
		salidas.push_back(useFunction(root->varOrFunc, tempRes[0], tempRes[1]));
	}
	for(int i = 0; i < salidas.size(); i++){
		error += pow(salidas[i] - deseados[i], 2);
	}
	error /= (float) salidas.size();
	return make_tuple(salidas, error);
}


PGTree::PGTree(Variables variables, Constantes constantes, Funciones funciones){
	this->variables = variables;
	this->constantes = constantes;
	this->funciones = funciones;
	

	int	numRandom = rand() % funciones.size();
	root = new PGNode(FUNCTION, funciones[numRandom]);
	for(int i = 0; i < 2; i++){
		numRandom = rand() % funciones.size();	
		root->hijos[i] = new PGNode(FUNCTION, funciones[numRandom]);
		for(int j = 0; j < 2; j++){
			numRandom = rand() % 2;
			if(numRandom == 0){
				numRandom = rand() % constantes.size();
				root->hijos[i]->hijos[j] = new PGNode(CONSTANT, constantes[numRandom]);
			} 
			else{
				numRandom = rand() % variables.size();
				root->hijos[i]->hijos[j] = new PGNode(VARIABLE, variables[numRandom]);
			}
		}
	}
}

PGTree::PGTree(PGTree * otherThree){
	root = new PGNode();
	variables = otherThree->variables;
	funciones = otherThree->funciones;
	constantes = otherThree->constantes;
	error = otherThree->error;
	*root = *otherThree->root;
	for(int i = 0; i < 2; i++){
		root->hijos[i] = new PGNode();
		*root->hijos[i] = *otherThree->root->hijos[i];
		for(int j = 0; j < 2; j++){
			root->hijos[i]->hijos[j] = new PGNode();
			*root->hijos[i]->hijos[j] = *otherThree->root->hijos[i]->hijos[j];
		}
	}
}

#endif