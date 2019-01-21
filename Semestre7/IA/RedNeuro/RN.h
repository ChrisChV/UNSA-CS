#ifndef RN_H
#define RN_H

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include "omp.h"

#define RAND_CAPA_MAX 2
#define LIMITE 0.0001
#define NUM_TREADS 4

using namespace std;

double ALFA = 0.5;

typedef double Val;

Val signoide(Val val){
	return  1.0 / (1.0 + exp(-val));
}


class Input{
	public:
		Input(int n);
		Input(vector<Val> v);
		void generateInput(Input in);
		void clear(){free(vec);}
		Input(){vec = NULL;}
		void print();
		Val& operator[](int i){return vec[i];}

		Val * vec;
		int size;
};

Input::Input(int n){
	srand(time(NULL));
	size = n;
	vec = (Val *) malloc(sizeof(Val) * n);
	for(int i = 0; i < n; i++){
		vec[i] = rand() % 2;
	}
	vec[0] = 1;
}

Input::Input(vector<Val> v){
	size = v.size();
	this->vec = (Val *) malloc(sizeof(Val) * size);
	for(int i = 0; i < size; i++){
		this->vec[i] = v[i];
	}
}

void Input::generateInput(Input in){
	size = in.size - 1;
	//free(vec);
	vec = (Val *) malloc(sizeof(Val) * size);
	if(size > 0) memcpy(vec,&in.vec[1],sizeof(Val) * size);
	else vec = in.vec;
}

void Input::print(){
	for(int i = 0; i < size; i++){
		cout<<vec[i]<<" ";
	}
	cout<<endl<<endl;
}


class BD{
	public:
		BD(string file, int numClass);
		tuple<vector<string>,vector<string>> splitLine(string line, int numClass);
		void print(){
			for(Input in : bdI) in.print();
			for(Input in : bdO) in.print();
		}

		vector<Input> bdI;
		vector<Input> bdO;

};

BD::BD(string file, int numClass){
	ifstream archivo(file.c_str());
	string line;
	while(archivo>>line){
		vector<string> SvalsI;
		vector<string> SvalsO;
		tie(SvalsI,SvalsO) = splitLine(line, numClass);
		vector<Val> valsI;
		vector<Val> valsO;
		valsI.push_back(1.0);
		for(string s : SvalsI) valsI.push_back(stod(s));
		bdI.push_back(Input(valsI));
		for(string s: SvalsO) valsO.push_back(stod(s));
		bdO.push_back(Input(valsO));
	}
}


tuple<vector<string>,vector<string>> BD::splitLine(string line, int numClass){
	vector<string> res1;
	vector<string> res2;
	string temp = "";
	string tempClass = "";
	bool flag = true;
	for(char c : line){
		if(c == ','){
			res1.push_back(temp);
			temp.clear();
		}
		else if(c == '\n'){
			tempClass = temp;
			flag = false;
			break;
		}
		else{
			temp.push_back(c);
		}
	}
	if(flag) tempClass = temp;
	for(int i = 0; i < numClass; i++){
		if(i == stoi(tempClass)) res2.push_back("1");
		else res2.push_back("0");
	}
	return make_tuple(res1,res2);
}


class Capa{
	public:
		Capa(int numNeuro, int numInputs);
		void print();
		void calcularSalidas(Input inp);
		void outputChange(Input salidasAnt, Input salidasDeseadas);
		void hiddenChange(Input salidasAnt, Capa capaSiguiente);
		Val* operator[](int i){return matrix[i];}

		Val ** matrix;
		int numNeuro;
		int numInputs;
		Input salidas;
		Input deltas;
};


Capa::Capa(int nN, int nI){
	numNeuro = nN;
	numInputs = nI;
	srand(time(NULL));
	matrix = (Val**) malloc(sizeof(void *) * nN);
	for(int i = 0; i < nN; i++){
		matrix[i] = (Val *) malloc(sizeof(Val) * nI);
	} 
	for(int i = 0; i < nN; i++){
		for(int j = 0; j < nI; j++){
			matrix[i][j] = rand() % RAND_CAPA_MAX;
		}
	}
}

void Capa::calcularSalidas(Input inp){
	salidas = Input(numNeuro + 1);
	Val sum = 0;
	salidas[0] = 1.0;
	for(int i = 0; i < numNeuro; i++){
		sum = 0;
		for(int j = 0; j < numInputs; j++){
			sum += matrix[i][j] * inp[j];
		} 
		salidas[i + 1] = signoide(sum);
	}
}

void Capa::print(){
	for (int i = 0; i < numNeuro; i++){
		for (int j = 0; j < numInputs; j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}


void Capa::outputChange(Input salidasAnt, Input salidasDeseadas){
	free(deltas.vec);
	deltas.vec = (Val *) malloc(sizeof(Val) * numNeuro);
	for(int i = 0; i < numNeuro; i++){
		deltas[i] = (salidas[i + 1] - salidasDeseadas[i]) * (1 - salidas[i + 1]);
	}
	for(int i = 0; i < numNeuro; i++){
		for(int j = 0; j < numInputs; j++){
			matrix[i][j] =  matrix[i][j] - ALFA * deltas[i] * salidasAnt[j];
		}
	}
}

void Capa::hiddenChange(Input salidasAnt, Capa capaSiguiente){
	free(deltas.vec);
	deltas.vec = (Val *) malloc(sizeof(Val) * numNeuro);
	Val sum = 0;
	for(int i = 0; i < numNeuro; i++){
		sum = 0;
		for(int j = 0; j < capaSiguiente.numNeuro - 1; j++){
			sum += capaSiguiente.deltas[j] * capaSiguiente[j][i + 1];
		}
		deltas[i] = sum * (1 - salidas[i + 1]) * salidas[i + 1];
	}
	for(int i = 0; i < numNeuro; i++){
		for(int j = 0; j < numInputs; j++){
			matrix[i][j] = matrix[i][j] - ALFA * deltas[i] * salidasAnt[j];
		}
	}
}


class RedNeuronal{
	public:
		RedNeuronal(Input in, Input salidasDeseadas, vector<int> capas);
		RedNeuronal(int numInputs, vector<int> capas);
		void run();
		void run(string file, int numClass, int iteraciones);
		void FP();
		void BP();
		void verify(string file, int numClass);
		int verifyClass(Input output);
		float getPorcentaje(int total, int porcion);
		double error();


		Capa * capas;
		Input inputs;
		Input output;
		Input salidasDeseadas;
		int numCapas;
};


RedNeuronal::RedNeuronal(Input in, Input salD, vector<int> capas){
	this->capas = (Capa *) malloc(sizeof(Capa) * capas.size());
	inputs = in;
	salidasDeseadas = salD;
	numCapas = capas.size();
	int input = inputs.size;
	for(int i = 0; i < capas.size(); i++){
		this->capas[i] = Capa(capas[i],input);
		input = capas[i] + 1;
	}
}

RedNeuronal::RedNeuronal(int numInputs, vector<int> capas){
	this->capas = (Capa *) malloc(sizeof(Capa) * capas.size());
	numCapas = capas.size();
	int input = numInputs;
	for(int i = 0; i < capas.size(); i++){
		this->capas[i] = Capa(capas[i],input);
		input = capas[i] + 1;
	}
}

void RedNeuronal::FP(){
	for(int i = 0; i < numCapas; i++){
		if(i == 0) capas[i].calcularSalidas(inputs);
		else capas[i].calcularSalidas(capas[i - 1].salidas);
	}
	output.generateInput(capas[numCapas - 1].salidas);
}

void RedNeuronal::BP(){
	for(int i = numCapas - 1; i > 0; i--){
		if(i == numCapas - 1){
			if(i == 0) capas[i].outputChange(inputs, salidasDeseadas);
			else capas[i].outputChange(capas[i - 1].salidas, salidasDeseadas);
		}
		else{
			if(i == 0) capas[i].hiddenChange(inputs, capas[i + 1]);
			else capas[i].hiddenChange(capas[i - 1].salidas, capas[i + 1]);
		}
	}
}

double RedNeuronal::error(){
	double res = 0;
	for(int i = 0; i < output.size; i++){
		res += pow(salidasDeseadas[i] - output[i],2);
	}
	return res / (double) (2.0);
}

void RedNeuronal::run(){
	FP();
	double error = this->error();
	cout<<error<<endl;
	while(error > LIMITE){
		BP();
		FP();
		error = this->error();
		cout<<error<<endl;
	}
	output.print();
}

void RedNeuronal::run(string file, int numClass, int iteraciones){
	BD bd(file, numClass);
	double error = 0;
	for(int i = 0; i < iteraciones; i++){
		for(int j = 0; j < bd.bdI.size(); j++){
			inputs = bd.bdI[j];
			salidasDeseadas = bd.bdO[j];
			FP();
			error = this->error();
			cout<<i<<"->"<<error<<endl;
			BP();
		}
	}
}

int RedNeuronal::verifyClass(Input output){
	for(int i = 0; i < output.size; i++){
		if(output[i] >= 0.5) return i;
	}
	return -1;
}

float RedNeuronal::getPorcentaje(int total, int porcion){
	return (float) (porcion) * 100 / (float) (total);
}

void RedNeuronal::verify(string file, int numClass){
	BD bd(file, numClass);
	vector<Input> salidasObtenidas;
	for(int i = 0; i < bd.bdI.size(); i++){
		inputs = bd.bdI[i];
		FP();
		salidasObtenidas.push_back(output);
	}

	int ** conMatrix = (int **) malloc(sizeof(void *) * numClass);
	int * total = (int*) malloc(sizeof(int) * numClass);
	for(int i = 0; i < numClass; i++) conMatrix[i] = (int *) malloc(sizeof(int) * (numClass + 1));
	for(int i = 0; i < numClass; i++) for(int j = 0; j < numClass + 1; j++) conMatrix[i][j] = 0;
	for(int i = 0; i < numClass; i++) total[i] = 0;

	for(int i = 0; i < bd.bdI.size(); i++){		
		if(verifyClass(salidasObtenidas[i]) == -1) conMatrix[verifyClass(bd.bdO[i])][numClass] += 1;
		else conMatrix[verifyClass(bd.bdO[i])][verifyClass(salidasObtenidas[i])] += 1;
	}

	for(int i = 0; i < numClass; i++) for(int j = 0; j < numClass + 1; j++) total[i] += conMatrix[i][j];

	cout<<setprecision(5);

	cout<<" \t\t\t Setosa \t VersiC \t\t Virginia \t\t Resagados"<<endl;
	for(int i = 0; i < numClass; i++){
		switch(i){
			case 0: cout<<"Setosa ("; break;
			case 1: cout<<"VersiC ("; break;
			case 2: cout<<"Virginia ("; break;
		}
		cout<<total[i]<<") \t\t ";
		for(int j = 0; j < numClass + 1; j++){
			cout<<conMatrix[i][j]<<"("<<getPorcentaje(total[i],conMatrix[i][j])<<"%) \t\t ";
		}
		cout<<endl;
	}
}


#endif
