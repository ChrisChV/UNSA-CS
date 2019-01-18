#include <iostream>
#include "Perceptron.h"
#include "FileOp.h"

using namespace std;

int main(){
	auto bd = getBd("iris", '\t');
	printRegistro(bd[0]);
	EntradasVec entVec;
	Entrada esperado;
	for(Registro reg : bd){
		entVec.push_back(Entrada());
		entVec.back().push_back(1);
		for(string s : reg){
			entVec.back().push_back(stof(s));
		}
		esperado.push_back(entVec.back().back());
		entVec.back().pop_back();
	}
	Pesos pesos = {0,0,0,0,0};
	Perceptron per(entVec, pesos, 0.5, esperado);
	per.run();
	

	/*
	EntradasVec entVec = {	{1,0,0},
							{1,0,1},
							{1,1,0},
							{1,1,1}};
	Pesos pesos = {0,0,0};
	Entrada esperado = {0,0,0,1};
	Perceptron per(entVec, pesos, 0.5, esperado);
	per.run(5);
	*/
}