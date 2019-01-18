#ifndef KOHONEN_H
#define KOHONEN_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>
#include <omp.h>

#define RANDOM_RANGE 2
#define NUM_THREADS 4

using namespace std;

omp_lock_t mutex;

typedef double Val;

typedef struct{
	int dim[3];
}Winner;

class NeuroMap{
	public:
		NeuroMap(int dimension, int numInputs, Val radioInicial, Val LearningInicial, int numIterations, 
			string file, string test);
		NeuroMap(string pesosFile, string clasesFile, string learning, string test);
		void changeColor(Val radio, Winner win, int index);  
		Winner getWin(Val * intput);
		Val getDistance(Val * a, Val * b);
		Val getDistance(Winner a, Winner b);
		void print();
		void printWin(Winner win);
		void printTraining();
		void printImg();
		void printTrainingImg(string *** vec);
		void changePesos(Winner win, Val * in, int index);
		void run();
		Winner test(int & winnerColor, int & testColor);
		Val * splitLine(string line);
		vector<Val *> getTraining(string file);
		Val getNewRadius();
		Val getNewLearning();
		Val getTheta(Winner win, Winner actual, Val radio);
		int *** allocMatrix();
		void runTests();
		void printClases();
		void save(string pesos, string clases);
		void load(string pesos, string clases);

		int NMsize;
		int Isize;
		int numIterations;
		int iteracionActual;
		Val LearningInicial;
		Val radioInicial;
		Val lambda;
		Val **** cube;
		vector<Val *> training;
		vector<Val *> tests;
		vector<int> colores;
		int *** clases;
};

NeuroMap::NeuroMap(string pesosFile, string clasesFile, string learning, string test){
	load(pesosFile,clasesFile);
	lambda = (double) (numIterations) / radioInicial;
	iteracionActual = 0;
	training = getTraining(learning);
	tests = getTraining(test);
}

NeuroMap::NeuroMap(int dimension, int numInputs, Val radioInicial, Val LearningInicial, int numIterations,
				 string file, string test){
	srand(time(NULL));
	NMsize = dimension;
	Isize = numInputs;
	this->radioInicial = radioInicial;
	this->numIterations = numIterations;
	this->LearningInicial = LearningInicial;
	this->training = training;
	lambda = (double)(numIterations) / radioInicial;
	iteracionActual = 0;
	training = getTraining(file);
	tests = getTraining(test);
	clases = allocMatrix();
	cube = (Val ****) malloc(sizeof(void ***) * dimension);
	for(int i = 0; i < NMsize; i++){
		cube[i] = (Val ***) malloc(sizeof(void **) * dimension);
		for(int j = 0; j < NMsize; j++){
			cube[i][j] = (Val **) malloc(sizeof(void *) * dimension);
			for(int k = 0; k < NMsize; k++){
				cube[i][j][k] = (Val *) malloc(sizeof(Val) * Isize);
			}
		}
	}
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				for(int w = 0; w < Isize; w++){
					cube[i][j][k][w] = rand() % RANDOM_RANGE;
				}
			}
		}
	}
}

void NeuroMap::save(string pesos, string clases){
	ofstream archivoPesos(pesos.c_str());
	ofstream archivoClases(clases.c_str());
	archivoPesos<<NMsize<<" "<<Isize<<" "<<radioInicial<<" "<<LearningInicial;
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				for(int w = 0; w < Isize; w++){
					archivoPesos<<cube[i][j][k][w]<<endl;
				}
			}
		}
	}
	archivoPesos.close();
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				archivoClases<<this->clases[i][j][k]<<endl;	
			}
		}
	}
	archivoClases.close();
}

void NeuroMap::load(string pesos, string clases){
	ifstream archivoPesos(pesos.c_str());
	ifstream archivoClases(clases.c_str());
	archivoPesos>>NMsize;
	archivoPesos>>Isize;
	archivoPesos>>radioInicial;
	archivoPesos>>LearningInicial;
	cube = (Val ****) malloc(sizeof(void ***) * NMsize);
	for(int i = 0; i < NMsize; i++){
		cube[i] = (Val ***) malloc(sizeof(void **) * NMsize);
		for(int j = 0; j < NMsize; j++){
			cube[i][j] = (Val **) malloc(sizeof(void *) * NMsize);
			for(int k = 0; k < NMsize; k++){
				cube[i][j][k] = (Val *) malloc(sizeof(Val) * Isize);
			}
		}
	}
	this->clases = allocMatrix();
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				for(int w = 0; w < Isize; w++){
					archivoPesos>>cube[i][j][k][w];
				}
			}
		}
	}
	archivoPesos.close();
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				archivoClases>>this->clases[i][j][k];
			}
		}
	}
	archivoClases.close();
}

vector<Val *> NeuroMap::getTraining(string file){
	vector<Val *> Vres;
	ifstream archivo(file.c_str());
	string temp = "";
	while(archivo>>temp){
		Val * res = (Val *) malloc(sizeof(Val) * Isize);
		res[0] = stod(temp);
		for(int i = 1; i < Isize; i++){
			archivo>>temp;
			res[i] =stod(temp);
		}
		Vres.push_back(res);
		archivo>>temp;
		colores.push_back(stod(temp));
	}
	archivo.close();
	return Vres;
}

void NeuroMap::print(){
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				for(int w = 0; w < Isize; w++){
					cout<<cube[i][j][k][w]<<" ";
				}
				cout<<"  ";
			}
			cout<<endl;
		}
		cout<<endl<<endl;
	}
	cout<<endl;
}

void NeuroMap::printWin(Winner win){
	cout<<"Dim1->"<<win.dim[0]<<endl;
	cout<<"Dim2->"<<win.dim[1]<<endl;
	cout<<"Dim3->"<<win.dim[2]<<endl;
	for(int i = 0; i < Isize; i++){
		cout<<cube[win.dim[0]][win.dim[1]][win.dim[2]][i]<<" ";
	}
	cout<<endl;
}

void NeuroMap::printImg(){
	for(int s : colores){
		cout<<s<<endl;
	}
}

void NeuroMap::printTraining(){
	for(Val * v : tests){
		for(int i = 0; i < Isize; i++){
			cout<<v[i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void NeuroMap::printClases(){
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				cout<<this->clases[i][j][k]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
}

void NeuroMap::printTrainingImg(string *** vec){
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				cout<<vec[i][j][k]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl;
}

Val NeuroMap::getDistance(Val * a, Val * b){
	Val res = 0;
	for(int i = 0; i < Isize; i++) res += pow(a[i] - b[i],2);
	return sqrt(res);
}

Val NeuroMap::getDistance(Winner a, Winner b){
	Val res = 0;
	for(int i = 0; i < 3; i++) res += pow(a.dim[i] - b.dim[i],2);
	return sqrt(res);
}

Winner NeuroMap::getWin(Val * inputs){
	Winner res;
	res.dim[0] = -1;
	res.dim[1] = -1;
	res.dim[2] = -1;
	Val menor = -1;
	Val temp = 0;
	#pragma omp parallel for num_threads(NUM_THREADS) private(temp)
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				temp = getDistance(inputs, cube[i][j][k]);
				omp_set_lock(&mutex);
				if(menor > temp or menor == -1){
					menor = temp;
					res.dim[0] = i;
					res.dim[1] = j;
					res.dim[2] = k;
				}
				omp_unset_lock(&mutex);
			}
		}
	}
	return res;
}


Val NeuroMap::getNewRadius(){
	return radioInicial * exp((double) (-iteracionActual) / lambda);
}

Val NeuroMap::getNewLearning(){
	return LearningInicial * exp((double) (-iteracionActual) / lambda);
}

Val NeuroMap::getTheta(Winner win, Winner actual, Val radio){
	Val distancia = getDistance(win, actual);
	return exp(-(distancia*distancia) / (2.0 * radio * radio));
}


void NeuroMap::changeColor(Val radio, Winner win, int index){
	Winner actual;
	int actualColor = colores[index];
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				actual.dim[0] = i;
				actual.dim[1] = j;
				actual.dim[2] = k;
				if(getDistance(win, actual) < radio){
					clases[i][j][k] = actualColor;
				}
			}
		}
	}
}

void NeuroMap::changePesos(Winner win, Val * in, int index){
	Val radio = getNewRadius();
	Val Learning = getNewLearning();
	Val thetha = 0;
	Winner actual;
	if(radio > 1) changeColor(radio,win,index);
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < NMsize; k++){
				actual.dim[0] = i;
				actual.dim[1] = j;
				actual.dim[2] = k;
				thetha = getTheta(win, actual, radio);
				if(getDistance(win, actual) <= radio){
					for(int w = 0; w < Isize; w++){
						cube[i][j][k][w] += thetha * Learning * (in[w] - cube[i][j][k][w]);
					}
				}
			}
		}
	}
	iteracionActual++;
	cout<<iteracionActual<<endl; 
}


int *** NeuroMap::allocMatrix(){
	int *** res = (int ***) malloc(sizeof(void**) * NMsize);
	for(int i = 0; i < NMsize; i++){
		res[i] = (int **) malloc(sizeof(void *) * NMsize);
		for(int j = 0; j < NMsize; j++){
			res[i][j] = (int *) malloc(sizeof(int) * NMsize);
			for(int k = 0; k < NMsize; k++){
				res[i][j][k] = 0;
			}
		}
	}
	return res;
}

void NeuroMap::run(){
	srand(time(NULL));
	Winner win;
	Val * randIn;
	int random = 0;

	for(int i = 0; i < numIterations; i++){
		random = rand() % training.size();
		randIn = training[random];
		win = getWin(randIn);
		changePesos(win, randIn, random);

	}
}

Winner  NeuroMap::test(int & winnerColor, int & testColor){
	srand(time(NULL));
	Val * randIn;
	int random = rand() % tests.size();
	randIn = tests[random];
	Winner win = getWin(randIn);
	testColor = colores[training.size() + random];
	winnerColor = clases[win.dim[0]][win.dim[1]][win.dim[2]];
	return win;
}

void NeuroMap::runTests(){
	Winner win;
	Val * in;
	int matrix[3][4];
	int por[4];
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			matrix[i][j] = 0;
		}
	}
	for(int i = 0; i < tests.size(); i++){
		in = tests[i];
		win = getWin(in);
		int indexI = colores[training.size() + i];
		int indexJ = clases[win.dim[0]][win.dim[1]][win.dim[2]];
		if(indexJ == 0) matrix[indexI - 1][4]++;
		else matrix[indexI - 1][indexJ - 1]++;
	}
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			if(i == j) por[i] = matrix[i][j];
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	for(int i = 0; i < 3; i++){
		por[i] = por[i] * 100 / 20;
		cout<<i+1<<"->"<<por[i]<<"%"<<endl;
	}


}

#endif