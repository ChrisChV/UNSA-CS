#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <iostream>
#include <vector>

using namespace std;

typedef float EntradaType;
typedef float PesoType;
typedef float AprendizajeType;
typedef vector<EntradaType> Entrada;
typedef vector<Entrada> EntradasVec;
typedef vector<PesoType> Pesos;

PesoType productPoint(Entrada ent, Pesos pesos){
	PesoType res = 0;
	for(int i = 0; i < ent.size(); i++){
		res += ent[i] * pesos[i];
	}
	return res;
}

class Perceptron{
	public:
		Perceptron(EntradasVec entVec, Pesos pesos, AprendizajeType tazaApren, Entrada esperado);
		int outFun(PesoType peso);
		void changePesos(int entradaPos);
		void run();
	private:
		EntradasVec entVec;
		Entrada esperado;
		Pesos pesos;
		AprendizajeType tazaApren;
		PesoType actualOut;
};

Perceptron::Perceptron(EntradasVec entVec, Pesos pesos, AprendizajeType tazaApren, Entrada esperado){
	this->entVec = entVec;
	this->pesos = pesos;
	this->tazaApren = tazaApren;
	this->esperado = esperado;
}

int Perceptron::outFun(PesoType peso){
	if(peso > 0) return 1;
	return 0;
}

void Perceptron::changePesos(int entradaPos){
	for(int i = 0; i < pesos.size(); i++){
		pesos[i] = pesos[i] + tazaApren * (esperado[entradaPos] - actualOut) * entVec[entradaPos][i];
		cout<<"w"<<i<<" = "<<pesos[i]<<endl;
	}
}

void Perceptron::run(){
	bool flag = false;
	int i = 0;
	while(true){
		cout<<"Iter "<<i++<<endl;
		flag = false;
		for(int j = 0; j < entVec.size(); j++){
			cout<<"Entrada "<<j;
			PesoType tempRes = productPoint(entVec[j], pesos);
			cout<<" f("<<tempRes<<") = ";
			actualOut = outFun(tempRes);
			cout<<actualOut<<" d = "<<esperado[j]<<endl;
			if(actualOut != esperado[j]){
				flag = true;
				changePesos(j);	
			} 
		}
		if(flag == false) break;
	}
	cout<<endl<<"Pesos Finales"<<endl;
	for(int j = 0; j < pesos.size(); j++){
		cout<<"w"<<j<<" = "<<pesos[j]<<endl;
	}
}

#endif