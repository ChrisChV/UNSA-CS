#ifndef RN_H
#define RN_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <cmath>
#include <thread>
#include "BD.h"

#define RAND_CAPA_MAX 2
#define LIMITE 0.0001
#define NUM_THREADS 4

using namespace std;

double ALFA = 0.1;

class Capa{
	public:
		Capa(){}
		Capa(int numNeuro, int numInputs, ActsFuns actFuns);
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
		ActivationFunc actFun;
		ActivationFunc derivateActFun;

};


Capa::Capa(int nN, int nI, ActsFuns actFuns){
	this->numNeuro = nN;
	this->numInputs = nI;
	this->actFun = get<0>(actFuns);
	this->derivateActFun = get<1>(actFuns);
	srand(time(NULL));
	matrix = new Val*[nN];
	for(int i = 0; i < nN; i++){
		matrix[i] = new Val[nI];
	} 
	for(int i = 0; i < nN; i++){
		for(int j = 0; j < nI; j++){
			matrix[i][j] = rand() % RAND_CAPA_MAX;
		}
	}
}

void calcularSalidasParallel(Val ** matrix, Input * inp, int ini, int end, Input * salidas,
							 ActivationFunc actFun, int numInputs){
	Val sum = 0;
	for(int i = ini; i < end; i++){
		sum = 0;
		for(int j = 0; j < numInputs; j++){
			sum += matrix[i][j] * (*inp)[j];
		}
		(*salidas)[i + 1] = actFun(sum);
	}
}

/*
void Capa::calcularSalidas(Input inp){
	salidas = Input(numNeuro + 1);
	salidas[0] = 1.0;
	thread threads[NUM_THREADS];
	int h = numNeuro / NUM_THREADS;
	if(NUM_THREADS > numNeuro) h = 1;
	int ini = 0;
	int end = 0;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = numNeuro;
		else end = ini + h;
		if(i == numNeuro) break;
		threads[i] = thread(calcularSalidasParallel, matrix, &inp, ini, end, &salidas, actFun, numInputs);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == numNeuro) break;
		threads[i].join();
	}
}
*/


void Capa::calcularSalidas(Input inp){
	salidas = Input(numNeuro + 1);
	Val sum = 0;
	salidas[0] = 1.0;
	for(int i = 0; i < numNeuro; i++){
		sum = 0;
		for(int j = 0; j < numInputs; j++){
			sum += matrix[i][j] * inp[j];
		} 
		salidas[i + 1] = actFun(sum);
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
	deltas.vec = new Val[numNeuro];
	//(Val *) malloc(sizeof(Val) * numNeuro);
	for(int i = 0; i < numNeuro; i++){
		//deltas[i] = (salidas[i + 1] - salidasDeseadas[i]) * -1 *(1 - salidas[i + 1])  * salidas[i + 1];
		deltas[i] = (salidas[i + 1] - salidasDeseadas[i]) * derivateActFun(salidas[i + 1]);
	}
	for(int i = 0; i < numNeuro; i++){
		for(int j = 0; j < numInputs; j++){
			matrix[i][j] =  matrix[i][j] - ALFA * deltas[i] * salidasAnt[j];
		}
	}
}

void Capa::hiddenChange(Input salidasAnt, Capa capaSiguiente){
	free(deltas.vec);
	//deltas.vec = (Val *) malloc(sizeof(Val) * numNeuro);
	deltas.vec = new Val[numNeuro];
	Val sum = 0;
	for(int i = 0; i < numNeuro; i++){
		sum = 0;
		for(int j = 0; j < capaSiguiente.numNeuro - 1; j++){
			sum += capaSiguiente.deltas[j] * capaSiguiente[j][i + 1];
		}
		//deltas[i] = sum * -1 * (1 - salidas[i + 1]) * salidas[i + 1];
		deltas[i] = sum * derivateActFun(salidas[i + 1]);
	}
	for(int i = 0; i < numNeuro; i++){
		for(int j = 0; j < numInputs; j++){
			matrix[i][j] = matrix[i][j] - ALFA * deltas[i] * salidasAnt[j];
		}
	}
}


class RedNeuronal{
	public:
		RedNeuronal(int numInputs, vector<int> capas, vector<ActsFuns> actVec);
		void run(string file, int numClass, int iteraciones);
		void FP();
		void BP();
		void verify(string file, vector<string> classNames);
		int verifyClass(Input output);
		double error();

		Capa * capas;
		Input inputs;
		Input output;
		Input salidasDeseadas;
		int numCapas;
};

RedNeuronal::RedNeuronal(int numInputs, vector<int> capas, vector<ActsFuns> actVec){
	this->capas = new Capa[capas.size()];
	this->numCapas = capas.size();
	int input = numInputs;
	for(int i = 0; i < capas.size(); i++){
		this->capas[i] = Capa(capas[i],input, actVec[i]);
		input = capas[i] + 1; // +1 por el bias;
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
	int i_max = -1;
	Val max = -1;
	for(int i = 0; i < output.size; i++){
		if(max == -1 or output[i] > max) {
			max = output[i];
			i_max = i;
		}
	}
	return i_max;

}


/*
void RedNeuronal::verify(string file, int numClass){
	BD bd(file, numClass);
	vector<Input> salidasObtenidas;
	for(int i = 0; i < bd.bdI.size(); i++){
		inputs = bd.bdI[i];
		FP();
		salidasObtenidas.push_back(output);
	}

	for(int i = 0; i < bd.bdI.size(); i++){		
		cout<<verifyClass(salidasObtenidas[i])<<endl;
	}

}
*/

float getPorcentaje(int total, int porcion){
	return (float) (porcion) * 100 / (float) (total);
}

void RedNeuronal::verify(string file, vector<string> classNames){
	int numClass = classNames.size();
	BD bd(file, numClass);
	vector<Input> salidasObtenidas;
	for(int i = 0; i < bd.bdI.size(); i++){
		inputs = bd.bdI[i];
		FP();
		salidasObtenidas.push_back(output);
	}

	int ** conMatrix = new int*[numClass];
	int * total = new int[numClass];
	for(int i = 0; i < numClass; i++) conMatrix[i] = new int[numClass];
	for(int i = 0; i < numClass; i++) for(int j = 0; j < numClass + 1; j++) conMatrix[i][j] = 0;
	for(int i = 0; i < numClass; i++) total[i] = 0;

	for(int i = 0; i < bd.bdI.size(); i++){		
		if(verifyClass(salidasObtenidas[i]) == -1) conMatrix[verifyClass(bd.bdO[i])][numClass] += 1;
		else conMatrix[verifyClass(bd.bdO[i])][verifyClass(salidasObtenidas[i])] += 1;
	}

	for(int i = 0; i < numClass; i++) for(int j = 0; j < numClass + 1; j++) total[i] += conMatrix[i][j];

	cout<<setprecision(5);
	
	for(int i = 0; i < numClass; i++){
		if(i == 0) cout<<" \t\t\t "<<classNames[i];
		else cout<<" \t\t "<<classNames[i];
	}
	cout<<endl;

	//cout<<" \t\t\t Setosa \t VersiC \t\t Virginia"<<endl;
	for(int i = 0; i < numClass; i++){
		cout<<classNames[i]<<" (";
		/*switch(i){
			case 0: cout<<"Setosa ("; break;
			case 1: cout<<"VersiC ("; break;
			case 2: cout<<"Virginia ("; break;
		}
		*/
		cout<<total[i]<<") \t\t ";
		for(int j = 0; j < numClass; j++){
			cout<<conMatrix[i][j]<<"("<<getPorcentaje(total[i],conMatrix[i][j])<<"%) \t\t ";
		}
		cout<<endl;
	}
}

#endif
